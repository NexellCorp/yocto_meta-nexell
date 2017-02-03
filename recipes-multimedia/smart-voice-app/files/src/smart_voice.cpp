#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <assert.h>
#include <endian.h>

#include "qbuff.h"
#include "wav.h"
#include "resample.h"
#include "audioplay.h"
#include "audiostream.h"

#include "event.h"
#include "util.h"
#include "util_debug.h"
#include "nx_pdm.h"

#ifdef SUPPORT_AEC_PCMOUT
extern "C" {
#include "libPreproc1.h"
}
#endif

/******************************************************************************/
#define SUPPORT_RATE_DETECTOR
#define SUPPORT_STREAM_MONITOR

#define CPU_FREQ_KHZ		1200000
#define PCM_PMD_DATA_SWAP

#define	PCM_PLAY_PERIOD_BYTES	4096		/* if UAC fix to 1024 or 4096 */
#define	PCM_PLAY_PERIOD_SIZE	16		/* if UAC fix */

#define	PCM_AEC_PERIOD_BYTES	1024		/* PDM (256*L/R) * 2 = 2048 */
#define	PCM_AEC_PERIOD_SIZE	64

#define	PCM_I2S_PERIOD_BYTES	4096		/* 8192, 4096 (per 20ms) */
#define	PCM_I2S_PERIOD_SIZE	24		/* 16, 32 */

#define	PCM_RES_PERIOD_BYTES	4096		/* 8192, 4096 (per 20ms) */
#define	PCM_RES_PERIOD_SIZE	24		/* 16, 32 */

#define	PCM_PDM_PERIOD_BYTES	(4*160*8)	/* FIX: 5120 per 10ms */
#define	PCM_PDM_PERIOD_SIZE	16
#define	PCM_AGC_INPUT_BYTES	(8192)		/* FIX: 8120 AGC IN  (from PDM) */
#define	PCM_AGC_OUTPUT_BYTES	(2048)		/* FIX: 2048 AGC OUT (from PDM) */

#define	PCM_AGC_PERIOD_BYTES	(2048)
#define	PCM_AGC_PERIOD_SIZE	((PCM_PDM_PERIOD_BYTES * PCM_PDM_PERIOD_SIZE) / PCM_AGC_PERIOD_BYTES)

#define WAV_SAVE_PERIOD		(512 * 1024)	/* 51Kbyte */
#define STREAM_MONITOR_PERIOD	(3 * 1000)	/* US */
#define	PCM_RESAMPLE_BYTES	(8192)

#define PCM_AGC_DEFAULT_GAIN	-10	/* -10 */

/******************************************************************************/

static pthread_mutex_t 		stream_lock;
#define	STREAM_LC_INIT()	pthread_mutex_init(&stream_lock, NULL)
#define	STREAM_LC_LOCK(s) 	pthread_mutex_lock(&stream_lock)
#define	STREAM_LC_FREE(s) 	pthread_mutex_unlock(&stream_lock)

#define	CMD_STREAM_EXIT		(1<<0)
#define	CMD_STREAM_REINIT	(1<<1)
#define	CMD_STREAM_NO_I2S	(1<<2)
#define	CMD_AEC_PROCESS		(1<<3)
#define	CMD_FILE_WRITE		(1<<4)
#define	CMD_FILE_STOP		(1<<5)
#define	CMD_FILE_PDM_IN		(1<<6)
#define	CMD_FILE_I2S_IN		(1<<7)

#define	SET_COMMAND(s, c)	(s->SetCommand(c, c))
#define	SET_COMMAND_MASK(s, c, m)	(s->SetCommand(c, m))
#define	CLR_COMMAND(s, c)	(s->SetCommand(~c, c))
#define	GET_COMMAND(s, c)	(s->GetCommand() & (c))
#define	GET_COMMAND_ALL(s, c)	((c) == (s->GetCommand() & (c)))

enum STREAM_TYPE {
	STREAM_PLAY,
	STREAM_I2S,
	STREAM_PDM,
	STREAM_AEC,	/* EC/NR */
	STREAM_AGC,	/* PDM */
	STREAM_RESAMPLE,
};

struct tm_stat {
	long long min;
	long long max;
	long long tot;
	long long cnt;
};

struct tm_stat tm_aec = { 0, };
struct tm_stat tm_agc = { 0, };

static inline void format_path(char *buf, const char *fmt, ...)
{
	assert(buf);
	va_list args;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
}

static void *audio_playback(void *data)
{
	CAudioStream *Stream = (CAudioStream *)data;
	unsigned int i_type = Stream->GetInputStreamType();
	int bytes = Stream->GetPeriodBytes();

	unsigned char *InPtr =  NULL;
	int ret;

	Stream->AudioOpen(AUDIO_STREAM_PLAYBACK);

__reinit:
	Stream->InBufWaitForCleanAll();
	CLR_COMMAND(Stream, CMD_STREAM_REINIT);

	while (!GET_COMMAND(Stream, CMD_STREAM_EXIT | CMD_STREAM_REINIT)) {

		InPtr = Stream->InBufPop(i_type, bytes, 1);
		if (!InPtr)
			continue;

		ret = Stream->AudioPlayBack(InPtr, bytes);
		if (ret < 0)
			MSLEEP(1);

		Stream->InBufPopRel(i_type, bytes);
	}

	if (GET_COMMAND(Stream, CMD_STREAM_REINIT))
		goto __reinit;

	pr_main("%10s: Exit\n", Stream->GetName());
	Stream->AudioClose();

	return NULL;
}

static void audio_clean(void *arg)
{
	CAudioStream *Stream = (CAudioStream *)arg;

	if (!Stream)
		return;

	pr_main("%10s: clean\n", Stream->GetName());

	for (int i = 0; i < Stream->WavFileNum; i++) {
		CWAVFile *pWav = Stream->WavFile[i];
		if (pWav)
			pWav->Close();
	}
}

static inline void agc_split(int *s, int *d0, int *d1, int size)
{
	int *S = s;
	int *D0 = d0, *D1 = d1;
	for (int i = 0; i < size; i += 8) {
		*d0++ = *s++;
		*d1++ = *s++;
	}

	assert(0 == (((long)s  - (long)S)  - size));
	assert(0 == (((long)d0 - (long)D0) - size/2));
	assert(0 == (((long)d1 - (long)D1) - size/2));
}

#ifdef PCM_PMD_DATA_SWAP
static inline void swap_16(void *s, int size)
{
	short *buf = (short *)s;

	for (int i = 0; i < size/2; i++, buf++)
		*buf = htobe16(*buf);
}
#else
#define	swap_16(s, l)
#endif

static void *audio_agc_stream(void *data)
{
	CAudioStream *Stream = (CAudioStream *)data;
	char *path = Stream->GetPath();
	unsigned int type = Stream->GetType();
	unsigned int i_type = Stream->GetInputStreamType();

	int chs = Stream->GetChannels();
	int rate = Stream->GetSampleRate();
	int bits = Stream->GetSampleBits();

	int o_bytes = Stream->GetPeriodBytes();
	int i_bytes = PCM_AGC_INPUT_BYTES;

	unsigned char *InPtr, *OutPtr;

	int *tmp[2] = { new int[o_bytes], new int[o_bytes] };
	bool is_DumpFile = false;

	CWAVFile *pIWav = new CWAVFile(WAV_SAVE_PERIOD);
	Stream->WavFile[0] = pIWav;
	Stream->WavFileNum = 1;

	struct tm_stat *tm = &tm_agc;
	long long ts = 0, td = 0, lp = 0;
	long long t_min = 1000 * 1000;
	long long t_max = 0, t_tot = 0;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_cleanup_push(audio_clean, (void *)Stream);

	/*
	 * Initialize PDM-AGC
	 */
	pdm_STATDEF pdm_st;
	int agc_dB = PCM_AGC_DEFAULT_GAIN;	/* -10 */

	pdm_Init(&pdm_st);

__reinit:
	pr_main("%10s: Init\n", Stream->GetName());

	Stream->OutBufClear(type);
	Stream->InBufWaitForCleanAll();
	CLR_COMMAND(Stream, CMD_STREAM_REINIT);

	while (!GET_COMMAND(Stream, CMD_STREAM_EXIT | CMD_STREAM_REINIT)) {

		/*
 		 * save data to files
		 */
		if (GET_COMMAND_ALL(Stream, CMD_FILE_WRITE | CMD_FILE_PDM_IN)) {
			char buf[256];

			format_path(buf, "%s/%s", path, "pdm_i.wav");
			pIWav->Open(chs, rate, bits, buf);
			is_DumpFile = true;
			CLR_COMMAND(Stream, CMD_FILE_WRITE);
		}

		if (GET_COMMAND_ALL(Stream, CMD_FILE_STOP | CMD_FILE_PDM_IN)) {
			pIWav->Close();
			is_DumpFile = false;
			CLR_COMMAND(Stream, CMD_FILE_STOP);
		}

		/*
		 * Get buffer with PDM resampling size (2048)
		 * 2048 : o_bytes
		 */
		OutPtr = Stream->OutBufPush(type, o_bytes, TIMEOUT_INFINITE);
		if (!OutPtr)
			continue;

		/*
		 * Get buffer from PDM output (5120 -> 8192)
		 * 5120 -> 8192 : PCM_AGC_INPUT_BYTES
		 */
		InPtr = Stream->InBufPop(i_type, i_bytes, TIMEOUT_INFINITE);
		if (!InPtr)
			continue;

		if (is_DumpFile)
			pIWav->Write(InPtr, i_bytes);

		/*
		 * PDM data AGC and split save output buffer
		 */
		RUN_TIMESTAMP_US(ts);

		swap_16(InPtr, i_bytes);
		pdm_Run(&pdm_st, (short int*)OutPtr, (int*)InPtr, agc_dB);

		END_TIMESTAMP_US(ts, td);

		/* for SPLIT copy */
		int length = o_bytes/2;
		agc_split((int*)OutPtr, tmp[0], tmp[1], o_bytes);

		/* Realignment PDM OUT [L0/R0/L1/R1] -> [L0/R0 ..... L1/R1 ....] */
		for (int i = 0; i < 2; i++) {
			unsigned char *dst = OutPtr + (i * length);

			memcpy(dst, tmp[i], length);
		}

		Stream->InBufPopRel(i_type, i_bytes);
		Stream->OutBufPushRel(type, o_bytes);

		if (t_min > td)
			t_min = td;
		if (td > t_max)
			t_max = td;

		t_tot += td, lp++;
		tm->min = t_min, tm->max = t_max;
		tm->tot = t_tot, tm->cnt = lp;
	}

	if (GET_COMMAND(Stream, CMD_STREAM_REINIT))
		goto __reinit;

	pr_main("%10s: Exit\n", Stream->GetName());

	pIWav->Close();
	pthread_cleanup_pop(1);
	pthread_exit(0);
}

static void *audio_aec_stream(void *data)
{
	CAudioStream *Stream = (CAudioStream *)data;
	char *path = Stream->GetPath();
	const char *name = Stream->GetName();
	unsigned int type = Stream->GetType();
	int inputs = Stream->GetInputStreamNums();

	unsigned char *InPtr[inputs] = { NULL, };
	unsigned char *OutPtr = NULL;

	int chs = Stream->GetChannels();
	int rate = Stream->GetSampleRate();
	int bits = Stream->GetSampleBits();

	int file_no = 0;
	int i = 0;
	int WAIT_TIME = 1;

#ifdef SUPPORT_AEC_PCMOUT
	struct tm_stat *tm = &tm_aec;
	long long ts = 0, td = 0, lp = 0;
	long long t_min = 1000 * 1000;
	long long t_max = 0, t_tot = 0;
#endif

	int aec_buf_bytes = PCM_AEC_PERIOD_BYTES; /* For Ref */
	int buf_out_bytes = PCM_AGC_OUTPUT_BYTES; /* <--- must be 2048 : Split 1024 * 2 */
	bool is_DumpFile = false;

	CWAVFile *pOWav = new CWAVFile(WAV_SAVE_PERIOD);
	CWAVFile *pIWav = new CWAVFile(WAV_SAVE_PERIOD);
	Stream->WavFile[0] = pOWav;
	Stream->WavFile[1] = pIWav;
	Stream->WavFileNum = 2;

	int i_ch = (4 * 2);

#ifdef SUPPORT_AEC_PCMOUT
	int *Out_PCM_aec1 = new int[256], *Out_PCM_aec2 = new int[256];
#endif
	int *Out_PCM = new int[256];
	int *Dummy = new int[256];	/* L/R : 256 Frame */
	int  ISYNC[1024];
	int n = 0;

	int *In_Buf[2] = { Dummy, Dummy }; /* PDM : L1/R1, L1/R1, L1/R1, ... */
	int *In_Ref[2] = { Dummy, Dummy }; /* I2S */

	memset(Dummy, 0, 256*sizeof(int));

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_cleanup_push(audio_clean, (void *)Stream);

__reinit:
	pr_main("%10s: Init\n", Stream->GetName());

	Stream->OutBufClear(type);
	Stream->InBufWaitForCleanAll();
	CLR_COMMAND(Stream, CMD_STREAM_REINIT);

	while (!GET_COMMAND(Stream, CMD_STREAM_EXIT | CMD_STREAM_REINIT)) {

		/*
 		 * save data to files
		 */
		if (GET_COMMAND(Stream, CMD_FILE_WRITE)) {
			char i_buf[256], o_buf[256];

			format_path(i_buf, "%s/capt%d.wav", path, file_no);
			format_path(o_buf, "%s/outp%d.wav", path, file_no);

			pIWav->Open(i_ch, rate, bits, i_buf);
			pOWav->Open(chs, rate, bits, o_buf);

			file_no++;	/* next file */
			is_DumpFile = true;
			CLR_COMMAND(Stream, CMD_FILE_WRITE);
		}

		if (GET_COMMAND(Stream, CMD_FILE_STOP)) {
			pIWav->Close();
			pOWav->Close();
			is_DumpFile = false;
			CLR_COMMAND(Stream, CMD_FILE_STOP);
		}

		/* Get InBuffers */
		for (i = 0, n = 0; i < inputs; i++) {
			unsigned int i_type = Stream->GetInputStreamType(i);
			switch (i_type) {
			case STREAM_AGC:
				/* Wait PDM buffer */
				do {
					InPtr[i] = Stream->InBufPop(i_type, buf_out_bytes, WAIT_TIME);
				} while (!InPtr[i] &&
					!GET_COMMAND(Stream, CMD_STREAM_EXIT | CMD_STREAM_REINIT));

				In_Buf[0] = (int*)(InPtr[i]);
				In_Buf[1] = (int*)(InPtr[i] + (buf_out_bytes/2));
				break;

			case STREAM_RESAMPLE:
				/* Wait I2S buffer */
				do {
					InPtr[i] = Stream->InBufPop(i_type, aec_buf_bytes, WAIT_TIME);
				} while (NULL == InPtr[i] &&
					!GET_COMMAND(Stream, CMD_STREAM_EXIT |
						CMD_STREAM_REINIT |
						CMD_STREAM_NO_I2S));

				In_Ref[n] = InPtr[i] ? (int*)InPtr[i] : Dummy;
				n++;	/* Next */
				break;

			default:
				pr_err("*** %10s: Unknown Buffer In ***\n", name);
				break;
			}

		#if 1
			if (NULL == InPtr[i] &&
			!GET_COMMAND(Stream, CMD_STREAM_NO_I2S))
				Stream->InBufIsEmpty(i_type);
		#endif
		}

		if (GET_COMMAND(Stream, CMD_STREAM_NO_I2S)) {
			In_Ref[0] = Dummy;
			In_Ref[1] = Dummy;
		}

		if (GET_COMMAND(Stream, CMD_STREAM_REINIT))
			goto __reinit;

		if (GET_COMMAND(Stream, CMD_STREAM_EXIT))
			goto __exit;

		/*
		 * save to file MIXED PDM0/1 & I2S0/1
		 */
		if (is_DumpFile) {
	#if 1
			int *d  = (int *)ISYNC;
			int *s0 = (int *)In_Buf[0];	/* INP: PDM0 */
			int *s1 = (int *)In_Buf[1];	/* INP: PDM1 */
			int *s2 = (int *)In_Ref[0];	/* REF: I2S0 */
			int *s3 = (int *)In_Ref[1];	/* REF: I2S1 */

			for (i = 0; i < aec_buf_bytes; i += 4) {
				*d++ = *s0++;
				*d++ = *s1++;
				*d++ = *s2++;
				*d++ = *s3++;
			}
	#endif
			assert(0 == ((long)d - (long)ISYNC) - sizeof(ISYNC));
			pIWav->Write((void*)ISYNC, aec_buf_bytes * (i_ch/2));
		}

		/*
		 * AEC process
		 */
		if (GET_COMMAND(Stream, CMD_AEC_PROCESS)) {
#ifdef SUPPORT_AEC_PCMOUT
			RUN_TIMESTAMP_US(ts);

			assert(In_Buf[0]), assert(In_Buf[1]);
			assert(In_Ref[0]), assert(In_Ref[1]);

			preProc((short int*)In_Buf[0], (short int*)In_Buf[1],
				(short int*)In_Ref[0], (short int*)In_Ref[1],
				(short int*)Out_PCM_aec1, (short int*)Out_PCM_aec2,
				(short int*)Out_PCM);

			END_TIMESTAMP_US(ts, td);
			if (is_DumpFile)
				pOWav->Write((void*)Out_PCM, aec_buf_bytes);
#endif
		}

		/* Release InBuffers */
		for (i = 0; i < inputs; i++) {
			if (InPtr[i]) {
				unsigned int i_type = Stream->GetInputStreamType(i);
				switch (i_type) {
					case STREAM_AGC:
						Stream->InBufPopRel(i_type, buf_out_bytes);
						break;
					case STREAM_RESAMPLE:
						Stream->InBufPopRel(i_type, aec_buf_bytes);
						break;
					default:
						break;
				}
			}
		}

		/* Copy to OutBuffer */
		OutPtr = Stream->OutBufPush(type, aec_buf_bytes, 1);
		if (OutPtr) {
			int *src;
			switch(Stream->GetValue()) {
				case  0:
					src = (int*)Out_PCM;
					break;
				case  1:
					src = (int*)In_Buf[0];
					break;
				case  2:
					src = (int*)In_Buf[1];
					break;
				case  3:
					src = (int*)In_Ref[0];
					break;
				case  4:
					src = (int*)In_Ref[1];
					break;
				case  5:
				default:
					src = (int*)Out_PCM;
					Stream->SetValue(0);
					break;
			}
			memcpy(OutPtr, src, aec_buf_bytes);
			Stream->OutBufPushRel(type, aec_buf_bytes);
		}

#ifdef SUPPORT_AEC_PCMOUT
		if (t_min > td)
			t_min = td;
		if (td > t_max)
			t_max = td;

		t_tot += td, lp++;
		tm->min = t_min, tm->max = t_max;
		tm->tot = t_tot, tm->cnt = lp;
#endif
	}

#ifdef SUPPORT_AEC_PCMOUT
	pr_main("AEC : min %3llu.%03llu ms, max %3llu.%03llu ms, avr %3llu.%03llu ms\n",
		t_min/1000, t_min%1000, t_max/1000, t_max%1000,
		(t_tot/lp)/1000, (t_tot/lp)%1000);
#endif

	if (GET_COMMAND(Stream, CMD_STREAM_REINIT))
		goto __reinit;

__exit:
	pr_main("%10s: Exit\n", Stream->GetName());

	pIWav->Close();
	pOWav->Close();
	pthread_cleanup_pop(1);
	pthread_exit(0);
}

static void *audio_capture(void *data)
{
	CAudioStream *Stream = (CAudioStream *)data;
	const char *name = Stream->GetName();
	unsigned int type = Stream->GetType();
	int bytes = Stream->GetPeriodBytes();

	int ch = Stream->GetChannels();
	int card = Stream->GetCard();
	int device = Stream->GetDevice();
	int rate = Stream->GetSampleRate();
	int bits = Stream->GetSampleBits();
	int periods = Stream->GetPeriods();
	bool err;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

__reinit:
	pr_main("%10s: Init\n", Stream->GetName());

	Stream->OutBufClear(type);
	CLR_COMMAND(Stream, CMD_STREAM_REINIT);

	/* re-open for resample */
	err = Stream->AudioOpen(card, device, ch, rate, bits,
				bytes, periods, AUDIO_STREAM_CAPTURE);
	if (!err) {
		pr_err("%10s: Failed audio open %s\n", __func__, name);
		return NULL;
	}

	err = Stream->AudioStart();
	if (!err) {
		pr_err("%10s: Failed audio start %s\n", __func__, name);
		return NULL;
	}

	pr_main("%10s audio:%d.%d, ch.%d, rate:%d, bits:%d, %d, %d\n",
		name, card, device, ch, rate, bits, bytes, periods);

	while (!GET_COMMAND(Stream, CMD_STREAM_EXIT | CMD_STREAM_REINIT)) {

		if (Stream->GetType() == STREAM_I2S &&
			GET_COMMAND(Stream, CMD_STREAM_NO_I2S)) {
			pr_debug("%10s: no data\n", name);
			usleep(100);
			continue;
		}

		Stream->OutBufIsFull(type);

		unsigned char *Ptr =
			Stream->OutBufPush(type, bytes, TIMEOUT_INFINITE);
		if (!Ptr)
			continue;

		int ret = Stream->AudioCapture(Ptr, bytes);
		if (ret < 0)
			continue;

		Stream->OutBufPushRel(type, bytes);
	}

	if (GET_COMMAND(Stream, CMD_STREAM_REINIT)) {
		Stream->AudioStop();
		goto __reinit;
	}

	pr_main("%10s: Exit\n", Stream->GetName());
	Stream->AudioClose();

	return NULL;
}

static void *audio_resample(void *data)
{
	CAudioStream *Stream = (CAudioStream *)data;
	CAudioStream *IStream = Stream->GetInputStreamObj();
	char *path = Stream->GetPath();

	unsigned int type = Stream->GetType();
	unsigned int i_type = Stream->GetInputStreamType();

	int o_rate = Stream->GetSampleRate();
	int o_bytes = Stream->GetPeriodBytes(); /* resample out 48000 -> 16000 */

	int chs = Stream->GetChannels();
	int bits = Stream->GetSampleBits();

	unsigned char *InPtr, *OutPtr;
	unsigned char *r_Buffer = new unsigned char[PCM_RESAMPLE_BYTES];
	int r_offset = 0;

	CWAVFile *pIWav = new CWAVFile(WAV_SAVE_PERIOD);
	Stream->WavFile[0] = pIWav;
	Stream->WavFileNum = 1;
	bool is_DumpFile = false;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_cleanup_push(audio_clean, (void *)Stream);

__reinit:
	int i_bytes = IStream->GetPeriodBytes();
	int i_rate = IStream->GetSampleRate();

	ReSampleContext *Resampler =
		audio_resample_init(chs, chs, (float)o_rate, (float)i_rate);
	assert(Resampler);

	Stream->OutBufClear(type);
	Stream->InBufWaitForCleanAll();

	CLR_COMMAND(Stream, CMD_STREAM_REINIT);

	pr_info("Resample %d ch, %d bit, %d -> %d\n", chs, bits, i_rate, o_rate);

	while (!GET_COMMAND(Stream, CMD_STREAM_EXIT | CMD_STREAM_REINIT)) {

		if (GET_COMMAND(Stream, CMD_STREAM_NO_I2S)) {
			pr_debug("%10s: no data\n", Stream->GetName());
			usleep(100);
			continue;
		}

		/*
 		 * save data to files
		 */
		if (GET_COMMAND(Stream, CMD_FILE_WRITE | CMD_FILE_I2S_IN)) {
			char buf[256];

			format_path(buf, "%s/%s", path, "i2s.wav");
			pIWav->Open(chs, i_rate, bits, buf);
			is_DumpFile = true;
			CLR_COMMAND(Stream, CMD_FILE_WRITE);
		}

		if (GET_COMMAND(Stream, CMD_FILE_STOP | CMD_FILE_I2S_IN)) {
			pIWav->Close();
			is_DumpFile = false;
			CLR_COMMAND(Stream, CMD_FILE_STOP);
		}

		/*
		 * Get buffer from I2S output
		 */
		InPtr = Stream->InBufPop(i_type, i_bytes, TIMEOUT_INFINITE);
		if (!InPtr)
			continue;

		if (is_DumpFile)
			pIWav->Write(InPtr, i_bytes);

		/*
		 * Resample
		 */
		int framebytes = chs * (bits/8);
		int r_frames;

		r_frames = audio_resample(Resampler, (short*)(r_Buffer + r_offset),
				(short*)InPtr, (i_bytes / framebytes));
		r_offset += (r_frames * framebytes);

		/*
		 * Copy resample data to out buffer
		 */
		if (r_offset >= o_bytes) {
			OutPtr = Stream->OutBufPush(type, o_bytes, TIMEOUT_INFINITE);
			if (!OutPtr)
				continue;

			int remain = r_offset - o_bytes;

			memcpy(OutPtr, r_Buffer, o_bytes);
			memmove(r_Buffer, r_Buffer + o_bytes, remain);
			r_offset = remain;

			Stream->OutBufPushRel(type, o_bytes);
		}
		Stream->InBufPopRel(i_type, i_bytes);
	}

	if (Resampler)
		audio_resample_close(Resampler);

	if (GET_COMMAND(Stream, CMD_STREAM_REINIT))
		goto __reinit;

	pr_main("%10s: Exit\n", Stream->GetName());

	pIWav->Close();
	pthread_cleanup_pop(1);
	pthread_exit(0);
}

/******************************************************************************/
struct audio_stream {
	CAudioStream **streams;
	int num_streams;
};

#ifdef SUPPORT_RATE_DETECTOR
static void audio_event_parse(const char *msg, struct udev_message *udev)
{
	const char *index = NULL;

    	udev->sample_frame = "";
    	udev->sample_rate = 0;

   	/* currently ignoring SEQNUM */
    	while(*msg) {

		index = "SAMPLERATE_CHANGED=";
        	if(!strncmp(msg, index, strlen(index))) {
            		msg += strlen(index);
            		udev->sample_rate = strtoll(msg, NULL, 10);
            		pr_main("[%s] %d\n", index, udev->sample_rate);
        	}

		index = "SAMPLE_NO_DATA=";
        	if(!strncmp(msg, index, strlen(index))) {
            		msg += strlen(index);
            		udev->sample_frame = msg;
            		pr_main("[%s] %s\n", index, udev->sample_frame);
        	}

        	/* advance to after the next \0 */
        	while(*msg++) { ; }
    }
}

static void *audio_event(void *arg)
{
	struct audio_stream *astream = (struct audio_stream *)arg;
	CAudioStream **streams = astream->streams;
	int size = astream->num_streams;

	CAudioStream *Stream = streams[0];
	struct udev_message udev;
	char buffer[256];
	int buf_sz = sizeof(buffer)/sizeof(buffer[0]);
	int msg_sz;
	int fd, i;
	static int sample_rate = 0;

	fd = audio_event_init();
	if (fd < 0)
		return 0;

	do {
		memset(buffer, 0, sizeof(buffer));
		msg_sz = audio_event_msg(fd, buffer, buf_sz);
		if (!msg_sz)
			continue;

		bool changed = false, no_lrck = false;

		audio_event_parse(buffer, &udev);

		if (udev.sample_rate && udev.sample_rate != sample_rate) {
			pr_info("Changed Rate [%dhz -> %dhz]\n",
				sample_rate, udev.sample_rate);
			changed = true;
			sample_rate = udev.sample_rate;
		}

		if (udev.sample_frame && !strcmp(udev.sample_frame, "YES")) {
			pr_info("Sample NO LRCLK\n");
			no_lrck = true;
			sample_rate = 0;
		}

		for (i = 0; size > i; i++) {
			Stream = streams[i];
			if (changed)
				SET_COMMAND_MASK(Stream, CMD_STREAM_REINIT,
					CMD_STREAM_REINIT | CMD_STREAM_NO_I2S);

			if (no_lrck)
				SET_COMMAND(Stream, CMD_STREAM_NO_I2S);

			if (GET_COMMAND(Stream, CMD_STREAM_EXIT))
				goto exit_event;
		}
	} while (1);

exit_event:
	audio_event_close(fd);
	return NULL;
}

static int stream_event(void *arg, pthread_t *thr)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
   	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if (0 != pthread_create(thr, &attr,  audio_event, arg)) {
   		pr_err("failed event thread %s \n", strerror(errno));
   		return errno;
   	}
   	return 0;
}
#endif

/******************************************************************************/
#ifdef SUPPORT_STREAM_MONITOR
static void *audio_monitor(void *arg)
{
	struct audio_stream *astream = (struct audio_stream *)arg;
	CAudioStream **streams = astream->streams;
	int size = astream->num_streams;

	while (1) {
		MSLEEP(STREAM_MONITOR_PERIOD);

		pr_info("================================================================\n");

		#ifdef SUPPORT_AEC_PCMOUT
		struct tm_stat *aec = &tm_aec;
		struct tm_stat *agc = &tm_agc;

		if (aec->cnt)
			pr_info("AEC : min %3llu.%03llu ms, max %3llu.%03llu ms, avr %3llu.%03llu ms\n",
				aec->min/1000, aec->min%1000, aec->max/1000, aec->max%1000,
				(aec->tot/aec->cnt)/1000, (aec->tot/aec->cnt)%1000);
		if (agc->cnt)
			pr_info("AGC : min %3llu.%03llu ms, max %3llu.%03llu ms, avr %3llu.%03llu ms\n",
				agc->min/1000, agc->min%1000, agc->max/1000, agc->max%1000,
				(agc->tot/agc->cnt)/1000, (agc->tot/agc->cnt)%1000);
		#endif

		for(int i = 0 ; i < size; i++) {
			CAudioStream *Stream = streams[i];
			CQueueBuffer *pBuf = Stream->GetBufferObj();
			if (pBuf){
				pr_info("[%10s] AvailSize = %7d/%7d\n",
					Stream->GetName(), pBuf->GetAvailSize(),
					pBuf->GetBufferBytes());
			}
		}
		pr_info("================================================================\n");
	}
	return NULL;
}

static int stream_monitor(void *arg, pthread_t *thr)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
   	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if(0 != pthread_create(thr, &attr, audio_monitor, arg)) {
   		pr_err("failed mointor thread %s \n", strerror(errno));
   		return errno;
   	}
	return 0;
}
#endif

/******************************************************************************/
enum CMD_TYPE {
	CMD_SET,
	CMD_CLR,
};

#define	check_argc(s, c)	(0 == strncmp(s, c, strlen(c)-1))

static void streams_command(enum CMD_TYPE cmd_type, unsigned int cmd,
			CAudioStream *streams[], int size)
{
	for (int i = 0; i < size; i++) {
		CAudioStream *Stream = streams[i];

		if (cmd_type == CMD_SET)
			SET_COMMAND(Stream, cmd);
		else
			CLR_COMMAND(Stream, cmd);

		pr_main("%10s: CMD 0x%x (%s) -> Stream[%d]\n",
			Stream->GetName(), cmd,
			cmd_type == CMD_SET ? "set" : "clr", i);
	}
}

static void streams_value(CAudioStream *streams[], int size, int value)
{
	for (int i = 0; i < size; i++) {
		CAudioStream *Stream = streams[i];
		Stream->SetValue(value);
	}
}

/******************************************************************************/
struct stream_thread {
	CAudioStream *Stream;
	void *(*func)(void *);
	pthread_t hTh;
};

struct args_option {
	int khz;
	bool no_in_arg;
	bool skip_aec;
	bool capt_path;
	bool filewrite;
	bool fc_pdm_in;
	bool fc_i2s_in;
	char path[256];
};

static void print_usage(void)
{
	pr_info("\n");
	pr_info("usage: options\n");
	pr_info("\t-s : set run cpu frequency\n");
	pr_info("\t-e : skip AEC process\n");
	pr_info("\t-i : not wait input argument\n");
	pr_info("\t-c : file save path\n");
	pr_info("\t-w : start file capture\n");
	pr_info("\t-r : file capture pdm input data\n");
	pr_info("\t-p : file capture i2s input data\n");
}

static void print_cmd_usage(void)
{
	pr_info("\n");
	pr_info("usage: options\n");
	pr_info("\t'q'   : Quiet\n");
	pr_info("\t's'   : start file write\n");
	pr_info("\t't'   : stop  file write\n");
	pr_info("\t'n'   : re initialize devices (for test)\n");
	pr_info("\t'd'   : no I2S data (for test)\n");
	pr_info("\t'num' : play out channel (0:AEC, 1:PDM0, 2:PDM1, 3:I2S0, 4:I2S1)\n");
}

static int parse_options(int argc, char **argv,
			struct args_option *option, unsigned int *cmd)
{
	int opt;

	option->no_in_arg = true;
	option->skip_aec = false;
	option->capt_path = false;
	option->filewrite = false;
	option->fc_pdm_in = false;
	option->fc_i2s_in = false;
	option->khz = CPU_FREQ_KHZ;
	strcpy(option->path, ".");

	*cmd = CMD_AEC_PROCESS;

	while (-1 != (opt = getopt(argc, argv, "hieprwc:s:"))) {
		switch(opt) {
        	case 's':
        		option->khz = strtoul(optarg, NULL, 10);
        		break;
        	case 'i':
        		option->no_in_arg = false;
        		break;
        	case 'e':
        		option->skip_aec = true;
        		*cmd &= ~CMD_AEC_PROCESS;
        		break;
       		case 'w':
       			option->filewrite = true;
       			*cmd |= CMD_FILE_WRITE;
       			break;
       		case 'r':
       			option->fc_pdm_in = true;
       			*cmd |= CMD_FILE_PDM_IN;
       			break;
       		case 'p':
       			option->fc_i2s_in = true;
       			*cmd |= CMD_FILE_I2S_IN;
       			break;
       		case 'c':
       			strcpy(option->path, optarg);
       			break;
        	default:
        		print_usage();
        		exit(0);
        		break;
      		}
	}
	return 0;
}

static void test_command(CAudioStream *Streams[], int size)
{
	char cmd[256];

	for (;;) {
		pr_info("#>");
		fflush(stdout);
		fgets(cmd, sizeof(cmd), stdin);

		if (0 == (strlen(cmd)-1))
			continue;

		if (check_argc("h", cmd)) {
			print_cmd_usage();
			continue;
		}

		if (check_argc("q", cmd))
			break;

		if (check_argc("s", cmd)) {
			streams_command(CMD_SET,
				CMD_FILE_WRITE, Streams, size);
			continue;
		}

		if (check_argc("t", cmd)) {
			streams_command(CMD_SET,
				CMD_FILE_STOP, Streams, size);
			continue;
		}

		if (check_argc("n", cmd)) {
			streams_command(CMD_SET,
				CMD_STREAM_REINIT, Streams, size);
			continue;
		}

		if (check_argc("d", cmd)) {
			streams_command(CMD_SET,
				CMD_STREAM_NO_I2S, Streams, size);
			continue;
		}

		int ch = strtol(cmd, NULL, 10);
		streams_value(Streams, size, ch);
	}
}

static struct audio_stream *s_astream;
static void (*sig_int_fun)(int);

static void sig_int_handler(int sig)
{
	struct audio_stream *astream = s_astream;
	CAudioStream **streams = astream->streams;
	int size = astream->num_streams;

	for (int i = 0; i < size; i++)
		audio_clean(streams[i]);

	sig_int_fun(sig);
}

int main(int argc, char **argv)
{
	struct args_option option;
	int khz = cpu_get_frequency();
	unsigned int command = 0;
	char buffer[32] = "NO DFS";

	parse_options(argc, argv, &option, &command);
	if (!(khz < 0))
		sprintf(buffer, "%d", khz/1000);

	pr_info("===========================================================\n");
	pr_info(" RUN[%d][%s -> %d Mhz]\n", getpid(), buffer, option.khz);
	pr_info("===========================================================\n");

	/* cpu speed */
	if (!(khz < 0))
		cpu_set_frequency(option.khz);

	/* for debugging */
	signal(SIGSEGV, sig_handler);
	signal(SIGILL, sig_handler);
	sig_int_fun = signal(SIGINT, sig_int_handler);
	sig_int_fun = signal(SIGTERM, sig_int_handler);

	/* create audio streams */
	CAudioStream *PlayStream = new CAudioStream();
	CAudioStream *AECStream = new CAudioStream();
	CAudioStream *I2SStream = new CAudioStream();
	CAudioStream *PDMStream = new CAudioStream();
	CAudioStream *AGCStream = new CAudioStream();
	CAudioStream *RESStream = new CAudioStream();

	PlayStream->Init(-1, -1, "Play"    , STREAM_PLAY    , 2, 16000, 16, PCM_PLAY_PERIOD_BYTES, PCM_PLAY_PERIOD_SIZE, false);
	AECStream->Init (-1, -1, "AEC"     , STREAM_AEC     , 2, 16000, 16, PCM_AEC_PERIOD_BYTES , PCM_AEC_PERIOD_SIZE, true);
	I2SStream->Init ( 0,  2, "I2S"     , STREAM_I2S     , 2, 48000, 16, PCM_I2S_PERIOD_BYTES , PCM_I2S_PERIOD_SIZE, true);
	PDMStream->Init ( 0,  4, "PDM"     , STREAM_PDM     , 4, 64000, 16, PCM_PDM_PERIOD_BYTES , PCM_PDM_PERIOD_SIZE, true);
	AGCStream->Init (-1, -1, "AGC"     , STREAM_AGC     , 4, 16000, 16, PCM_AGC_PERIOD_BYTES , PCM_AGC_PERIOD_SIZE, true);
	RESStream->Init ( 0,  0, "Resample", STREAM_RESAMPLE, 2, 16000, 16, PCM_RES_PERIOD_BYTES , PCM_RES_PERIOD_SIZE, true);

	struct stream_thread stream_ths[] = {
		{ PlayStream, audio_playback	},
		{ AECStream , audio_aec_stream	},
		{ I2SStream , audio_capture	},
		{ PDMStream , audio_capture    	},
		{ AGCStream , audio_agc_stream 	},
		{ RESStream , audio_resample  	},
	};

	PlayStream->AddInputStream(AECStream);	/* AEC      -> playback */
	AECStream->AddInputStream (RESStream);	/* Resample -> AEC */
	AECStream->AddInputStream (AGCStream);	/* AGC 	    -> AEC */
	RESStream->AddInputStream (I2SStream);	/* I2S 	    -> Resample */
	AGCStream->AddInputStream (PDMStream);	/* PDM 	    -> AGC */

	int size = (int)ARRAY_SIZE(stream_ths);
	CAudioStream *Streams[size];
	struct audio_stream astream = { Streams, size };
	int i, ret;

   	STREAM_LC_INIT();
   	s_astream = &astream;

	for (i = 0; i < size; i++)
		Streams[i] = stream_ths[i].Stream;

#ifdef SUPPORT_AEC_PCMOUT
	pr_info("<<*** AEC ***>>\n");
	if (!option.skip_aec)
		aec_mono_init();
#endif

	/* capture/play threads */
	pthread_attr_t attr;

	pthread_attr_init(&attr);
   	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < size; i++) {
		CAudioStream *Stream = stream_ths[i].Stream;

		SET_COMMAND(Stream, command);
		Stream->SetPath(option.path);

		if (0 != pthread_create(&stream_ths[i].hTh,
				&attr, stream_ths[i].func, (void*)Stream)) {
	    		pr_err("Failed thread.%d create, %s (%s)!\n",
	    			i, Stream->GetName(), strerror(errno));
  			goto err_threads;
		}
	}

#ifdef SUPPORT_RATE_DETECTOR
	pthread_t th_event;
	stream_event(&astream, &th_event);
#endif
#ifdef SUPPORT_STREAM_MONITOR
	pthread_t th_monitor;
	stream_monitor(&astream, &th_monitor);
#endif
	if (!option.no_in_arg) {
		while (1)
			sleep(1);
	}
	MSLEEP(500);

	test_command(Streams, size);

	/* Exit All threads */
	pr_info("EXIT...\n");
	streams_command(CMD_SET, CMD_STREAM_EXIT, Streams, size);

err_threads:

#ifdef SUPPORT_STREAM_MONITOR
	ret = pthread_cancel(th_monitor);
	pthread_join(th_monitor, NULL);
#endif
#ifdef SUPPORT_RATE_DETECTOR
	ret = pthread_cancel(th_event);
	pthread_join(th_event, NULL);
#endif

 	/* Free attribute and wait for the other threads */
	for (i = 0; i < size; i++) {
		ret = pthread_cancel(stream_ths[i].hTh);
		pthread_join(stream_ths[i].hTh, (void **)&ret);
		pr_main("EXIT %d:%10s\n",
			i, stream_ths[i].Stream->GetName());
	}

	return 0;
}
