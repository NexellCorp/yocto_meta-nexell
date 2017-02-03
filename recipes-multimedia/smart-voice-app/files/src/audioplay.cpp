
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "audioplay.h"
#include "util_debug.h"

struct pcm {
	int fd;
	unsigned int flags;
	int running:1;
	int underruns;
	unsigned int buffer_size;
	unsigned int boundary;
	char error[128];
	struct pcm_config config;
	struct snd_pcm_mmap_status *mmap_status;
	struct snd_pcm_mmap_control *mmap_control;
	struct snd_pcm_sync_ptr *sync_ptr;
	void *mmap_buffer;
	unsigned int noirq_frames_per_msec;
};

CAudioPlayer::CAudioPlayer(void)
	: m_hPCM(NULL)
{
	memset(&m_PCMConfig, 0, sizeof(m_PCMConfig));
	memset(m_PcmName, 0, sizeof(m_PcmName));

	m_Card = -1, m_Device = -1;
	m_SampleBits = 0;
	m_Stream = 0;
	m_PeriodBytes = 0;
	m_Channels = 0;
	m_SampleRate = 0;
	m_Periods = 0;
	m_FrameBytes = 0;
}

CAudioPlayer::CAudioPlayer(const char *Name, int Card, int Device,
			int Channels, int SampleRate, int SampleBits,
			int Periods, int PeriodBytes,
			enum AUDIO_STREAM_DIR Direction)
{
	bool ret = Open(Name, Card, Device,
			Channels, SampleRate, SampleBits,
			Periods, PeriodBytes, Direction);

	if (!ret)
		m_hPCM = NULL;
}

CAudioPlayer::~CAudioPlayer(void)
{
	Close();
}

bool CAudioPlayer::Open(const char *Name, int Card, int Device,
			int Channels, int SampleRate, int SampleBits,
			int Periods, int PeriodBytes,
			enum AUDIO_STREAM_DIR Direction)
{
	int Stream;

	if (Direction == AUDIO_STREAM_PLAYBACK)
		Stream = PCM_OUT;
	else
		Stream = PCM_IN;

	m_PCMConfig.channels = Channels;
	m_PCMConfig.rate = SampleRate;
	m_PCMConfig.period_size = PeriodBytes/(Channels*(SampleBits/8));
	m_PCMConfig.period_count = Periods;

	pr_aplay("%s %s card:%d.%d ch.%d, %d hz, %d bits\n",
		Name, Stream == PCM_OUT ? "Playing" : "Capturing",
		Card, Device, Channels, SampleRate, SampleBits);
	pr_aplay("period bytes %d, period size %d, periods %d\n",
		PeriodBytes, m_PCMConfig.period_size, Periods);

	if (SampleBits == 32)
		m_PCMConfig.format = PCM_FORMAT_S32_LE;
	else if (SampleBits == 16)
		m_PCMConfig.format = PCM_FORMAT_S16_LE;
	else if (SampleBits == 8)
		m_PCMConfig.format = PCM_FORMAT_S8;
	else {
		pr_err("E: %s Unable to sample bits %d for card %d device %d\n",
			Stream == PCM_OUT ? "Playing":"Capturing",
			SampleBits, Card, Device);
		return false;
	}

	m_PCMConfig.start_threshold = 0;
	m_PCMConfig.stop_threshold = 0;
	m_PCMConfig.silence_threshold = 0;

	m_hPCM = pcm_open(Card, Device, Stream, &m_PCMConfig);
	if (!m_hPCM || !pcm_is_ready(m_hPCM)) {
		pr_err("E: %s Unable to open card %d.%d ch.%d, %d hz, %d bits\n",
			Stream == PCM_OUT ? "Playing":"Capturing",
			Card, Device, Channels, SampleRate, SampleBits);
		pr_err("E: period bytes %d, period size %d, periods %d\n",
			PeriodBytes, m_PCMConfig.period_size, Periods);
		pr_err("E:%s\n", pcm_get_error(m_hPCM));
		return false;
	}

	if (Name && strlen(Name))
		strcpy(m_PcmName, Name);

	m_Card = Card;
	m_Device = Device;
	m_Channels = Channels;
	m_SampleRate = SampleRate;
	m_SampleBits = SampleBits;
	m_PeriodBytes = PeriodBytes;
	m_Periods = Periods;
	m_FrameBytes = Channels*(SampleBits/8);
	m_Stream = Stream;

	return true;
}

void CAudioPlayer::Close(void)
{
	if (m_hPCM)
		pcm_close(m_hPCM);
	m_hPCM = NULL;
}

int CAudioPlayer::Capture(unsigned char *Buffer, int Byte)
{
	if (!m_hPCM)
		return 0;

	int ret = pcm_read(m_hPCM, Buffer, Byte);

	if (ret) {
		pr_err("E: Capturing, %s %s\n",
			m_PcmName, pcm_get_error(m_hPCM));
		return -1;
	}
	return Byte;
}

int CAudioPlayer::PlayBack(unsigned char *Buffer, int Byte)
{
	if (!m_hPCM)
		return 0;

	int ret = pcm_write(m_hPCM, Buffer, Byte);

	if (ret) {
	#if 0
		pr_err("E: Playing, %s %s\n",
			m_PcmName, pcm_get_error(m_hPCM));
	#endif
		return -1;
	}

	return Byte;
}

bool CAudioPlayer::Start(void)
{
	if (!m_hPCM)
		return false;

	struct pcm *pcm = m_hPCM;
	int ret = pcm_start(pcm);

	if (ret < 0)
		pr_err("E: cannot start channel <%s> (%s) ...\n",
			m_PcmName, pcm_get_error(pcm));

	return ret < 0 ? false  : true;
}

bool CAudioPlayer::Stop(bool Drop)
{
	if (NULL == m_hPCM)
		return -1;

	struct pcm *pcm = m_hPCM;
	int ret = pcm_stop(pcm);

	if (0 > ret)
		pr_err("E: cannot stop channel <%s> (%s) ...\n",
			m_PcmName, pcm_get_error(pcm));

    return ret < 0 ? false  : true;
}

bool CAudioPlayer::Stop(void)
{
	return Stop(true);
}

void CAudioPlayer::PcmInfo(int Card, int Device,
			enum AUDIO_STREAM_DIR Direction)
{
	struct pcm_params *params;
	unsigned int min;
	unsigned int max;
	int Stream = Direction == AUDIO_STREAM_PLAYBACK ?
		PCM_OUT : PCM_IN;

	pr_info("\nPCM %s:\n", Stream == PCM_OUT ? "out" : "in");

	params = pcm_params_get(Card, Device, Stream);
	if (params == NULL) {
		pr_info("E: card %d, device %d does not exist.\n",
			m_Card, m_Device);
		return;
	}

	min = pcm_params_get_min(params, PCM_PARAM_RATE);
	max = pcm_params_get_max(params, PCM_PARAM_RATE);
	pr_info("        Rate:\tmin=%uHz\tmax=%uHz\n", min, max);
	min = pcm_params_get_min(params, PCM_PARAM_CHANNELS);
	max = pcm_params_get_max(params, PCM_PARAM_CHANNELS);
	pr_info("    Channels:\tmin=%u\t\tmax=%u\n", min, max);
	min = pcm_params_get_min(params, PCM_PARAM_SAMPLE_BITS);
	max = pcm_params_get_max(params, PCM_PARAM_SAMPLE_BITS);
	pr_info(" Sample bits:\tmin=%u\t\tmax=%u\n", min, max);
	min = pcm_params_get_min(params, PCM_PARAM_PERIOD_SIZE);
	max = pcm_params_get_max(params, PCM_PARAM_PERIOD_SIZE);
	pr_info(" Period size:\tmin=%u\t\tmax=%u\n", min, max);
	min = pcm_params_get_min(params, PCM_PARAM_PERIODS);
	max = pcm_params_get_max(params, PCM_PARAM_PERIODS);
	pr_info("Period count:\tmin=%u\t\tmax=%u\n", min, max);

	pcm_params_free(params);
}
