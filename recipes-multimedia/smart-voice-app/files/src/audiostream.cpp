

#include "audiostream.h"

#define	STREAM_BUFF_SIZE_MULTI	2

CAudioStream::CAudioStream(void)
	: m_Buffer(NULL),
  	  m_AudioPlayer(NULL)
{
	memset(m_FileDir, 0, sizeof(m_FileDir));
	memset(m_InStreams, 0, ARRAY_SIZE(m_InStreams));
	memset(WavFile, 0, ARRAY_SIZE(WavFile));

	WavFileNum = 0;
	m_InStreamNum = 0;

	m_Name = NULL;
	m_Card = 0;
	m_Device = 0;
	m_Type = 0;
	m_Channels = 0;
	m_SampleRate = 0;
	m_SampleBits = 0;
	m_Periods = 0;
	m_PeriodBytes = 0;
	m_Buffer = NULL;
	m_bValid = false;
	m_Command = 0;
	m_Value = 0;
	m_AllocBuffer = false;
}

CAudioStream::CAudioStream(const char *Name, unsigned int Type, bool AllocBuffer)
	: m_Buffer(NULL),
  	  m_AudioPlayer(NULL)
{
	m_Name = Name;
	m_Type = Type;
	m_AllocBuffer = AllocBuffer;
}

CAudioStream::~CAudioStream(void)
{
}

void CAudioStream::Init(int Card, int Device,
			int Channels, int SampleRate, int SampleBits,
			int PeriodBytes, int Periods)
{
	pthread_mutex_init(&m_Lock, NULL);
	pthread_mutex_init(&m_CmdLock, NULL);

	memset(m_FileDir, 0, sizeof(m_FileDir));
	memset(m_InStreams, 0, ARRAY_SIZE(m_InStreams));
	memset(WavFile, 0, ARRAY_SIZE(WavFile));

	WavFileNum = 0;
	m_InStreamNum = 0;

	m_Card = Card;
	m_Device = Device;
	m_Channels = Channels;
	m_SampleRate = SampleRate;
	m_SampleBits = SampleBits;
	m_Periods = Periods;
	m_PeriodBytes = PeriodBytes;
	m_bValid = true;
	m_Command = 0;
	m_Value = 0;

	Periods = Periods * (0 == STREAM_BUFF_SIZE_MULTI ?
			1 : STREAM_BUFF_SIZE_MULTI);

	pthread_mutex_lock(&m_Lock);
	if (m_AllocBuffer) {
		if (m_Buffer)
			delete m_Buffer;

		m_Buffer = new CQueueBuffer(Periods, PeriodBytes, m_Name, m_Type);
	}
	pthread_mutex_unlock(&m_Lock);

	pr_stream("Stream: %s, Buffer[%p] [%d:%d]\n",
		m_Name, m_Buffer ? m_Buffer : NULL, m_PeriodBytes, m_Periods);

}

void CAudioStream::Release(void)
{
	pthread_mutex_lock(&m_Lock);
	/* close wav files */
	for (int i = 0; WavFileNum > i; i++) {
		CWAVFile *pWav = WavFile[i];
		if (pWav)
			pWav->Close();
	}

	if (m_Buffer)
		delete m_Buffer;

	m_Buffer = NULL;

	pthread_mutex_unlock(&m_Lock);

	return;
}

void CAudioStream::AddInputStream(CAudioStream *Stream)
{
	if (!Stream)
		return;

	m_InStreams[m_InStreamNum] = Stream;
	m_InStreamNum++;
}

CAudioStream *CAudioStream::GetInputStreamObj(int Index)
{
	return m_InStreams[Index];
}

CAudioStream *CAudioStream::GetInputStreamObj(void)
{
	return GetInputStreamObj(0);
}

unsigned int CAudioStream::GetInputStreamType(int Index)
{
	CAudioStream *Stream = m_InStreams[Index];

	if (Stream)
		return Stream->m_Type;

	return -1;
}

unsigned int CAudioStream::GetInputStreamType(void)
{
	return GetInputStreamType(0);
}

bool CAudioStream::AudioOpen(enum AUDIO_STREAM_DIR Direction)
{
	if (m_AudioPlayer)
		return true;

	if (m_Card == -1 || m_Device == -1)
		return false;

	CAudioPlayer *Audio = new CAudioPlayer;

	pr_stream("%s, card:%d.%d %d ch %d hz %2d bits periods %3d bytes %4d, %s\n",
		m_Name, m_Card, m_Device, m_Channels,
		m_SampleRate, m_SampleBits, m_Periods, m_PeriodBytes,
		Direction == AUDIO_STREAM_PLAYBACK ? "PLAY" : "CAPT");

	bool err = Audio->Open(m_Name, m_Card, m_Device, m_Channels,
			m_SampleRate, m_SampleBits, m_Periods, m_PeriodBytes,
			Direction);
	if (err)
		m_AudioPlayer = Audio;

	return err;
}

bool CAudioStream::AudioOpen(int Card, int Device,
			int Channels, int SampleRate, int SampleBits,
			int PeriodBytes, int Periods,
			enum AUDIO_STREAM_DIR Direction)
{
	if (m_AudioPlayer)
		return true;

	if (m_Card == -1 || m_Device == -1)
		return false;

	CAudioPlayer *Audio = new CAudioPlayer;

	pr_stream("%s, card:%d.%d %d ch %d hz %2d bits periods %3d bytes %4d, %s\n",
		m_Name, m_Card, m_Device, m_Channels,
		m_SampleRate, m_SampleBits, m_Periods, m_PeriodBytes,
		Direction == AUDIO_STREAM_PLAYBACK ? "PLAY" : "CAPT");

	bool err = Audio->Open(m_Name, m_Card, m_Device, m_Channels,
			m_SampleRate, m_SampleBits, m_Periods, m_PeriodBytes,
			Direction);
	if (err)
		m_AudioPlayer = Audio;

	return err;
}

void CAudioStream::AudioClose(void)
{
	if (m_AudioPlayer) {
		m_AudioPlayer->Close();
		delete m_AudioPlayer;
		m_AudioPlayer = NULL;
	}
}

bool CAudioStream::AudioStart(void)
{
	if (!m_AudioPlayer)
		return false;

	return m_AudioPlayer->Start();
}

void CAudioStream::AudioStop(void)
{
	if (m_AudioPlayer)
		m_AudioPlayer->Stop();
}

int CAudioStream::AudioPlayBack(unsigned char *Buffer, int Bytes)
{
	if (m_AudioPlayer)
		return m_AudioPlayer->PlayBack(Buffer, m_PeriodBytes);
	return 0;
}

int CAudioStream::AudioCapture(unsigned char *Buffer, int Bytes)
{
	if (m_AudioPlayer)
		return m_AudioPlayer->Capture(Buffer, Bytes);
	return 0;
}

void CAudioStream::OutBufIsFull(unsigned int Type)
{
	pthread_mutex_lock(&m_Lock);

	if (!m_Buffer) {
		pthread_mutex_unlock(&m_Lock);
		return;
	}

	if (!m_Buffer->Is_PushAvail())
		pr_info("[FULL ][%s:%d] (%d, %dx%d)\n",
			m_Buffer->GetBufferName(),
			m_Buffer->GetAvailSize(), m_Buffer->GetBufferBytes(),
			m_Buffer->GetFrameBytes(), m_Buffer->GetFrameCounts());

	pthread_mutex_unlock(&m_Lock);
}

void CAudioStream::OutBufClear(unsigned int Type)
{
	pthread_mutex_lock(&m_Lock);

	if (m_Buffer)
		m_Buffer->ClearBuffer();

	pthread_mutex_unlock(&m_Lock);
}

unsigned char *CAudioStream::OutBufPush(
			unsigned int Type, int Bytes, int WaitTime)
{
	unsigned char *Ptr = NULL;

	pthread_mutex_lock(&m_Lock);

	if (m_Buffer)
		Ptr = m_Buffer->PushBuffer(Bytes, WaitTime);

	pthread_mutex_unlock(&m_Lock);

	return Ptr;
}

bool CAudioStream::OutBufPushRel(unsigned int Type, int Bytes)
{
	bool ret = false;

	pthread_mutex_lock(&m_Lock);

	if (m_Buffer)
		ret = m_Buffer->PushRelease(Bytes);

	pthread_mutex_unlock(&m_Lock);

	return ret;
}

void CAudioStream::InBufIsEmpty(unsigned int Type)
{
	CAudioStream *Stream;
	CQueueBuffer *Buffer;

	pthread_mutex_lock(&m_Lock);
	for (int i = 0; i < m_InStreamNum; i++) {
		Stream = m_InStreams[i];
		if (Stream && Stream->m_Buffer && Stream->m_Type == Type) {
			Buffer = Stream->m_Buffer;
			if (!Buffer->Is_PopAvail())
				pr_info("[EMPTY][%s:%d] (%d, %dx%d)\n",
					Buffer->GetBufferName(),
					Buffer->GetAvailSize(),
					Buffer->GetBufferBytes(),
					Buffer->GetFrameBytes(),
					Buffer->GetFrameCounts());
		}
	}
	pthread_mutex_unlock(&m_Lock);
}

void CAudioStream::InBufWaitForClean(unsigned int Type)
{
	CAudioStream *Stream;
	CQueueBuffer *Buffer;

	pthread_mutex_lock(&m_Lock);
	for (int i = 0; i < m_InStreamNum; i++) {
		Stream = m_InStreams[i];
		if (Stream && Stream->m_Buffer &&
			Stream->m_Type == Type) {
			Buffer = Stream->m_Buffer;
			Buffer->WaitForClear();
		}
	}
	pthread_mutex_unlock(&m_Lock);
}

void CAudioStream::InBufWaitForCleanAll(void)
{
	CAudioStream *Stream;
	CQueueBuffer *Buffer;

	pthread_mutex_lock(&m_Lock);
	for (int i = 0; i < m_InStreamNum; i++) {
		Stream = m_InStreams[i];
		if (Stream && Stream->m_Buffer) {
			Buffer = Stream->m_Buffer;
			Buffer->WaitForClear();
		}
	}
	pthread_mutex_unlock(&m_Lock);
}

unsigned char *CAudioStream::InBufPop(
			unsigned int Type, int Bytes, int WaitTimes)
{
	CAudioStream *Stream;
	CQueueBuffer *Buffer;
	unsigned char *Ptr = NULL;

	pthread_mutex_lock(&m_Lock);
	for (int i = 0; i < m_InStreamNum; i++) {
		Stream = m_InStreams[i];
		if (Stream && Stream->m_Buffer &&
		Stream->m_Type == Type) {
			Buffer = Stream->m_Buffer;
			Ptr = Buffer->PopBuffer(Bytes, WaitTimes);
			break;
		}
	}
	pthread_mutex_unlock(&m_Lock);

	return Ptr;
}

bool CAudioStream::InBufPopRel(unsigned int Type, int Bytes)
{
	CAudioStream *Stream;
	CQueueBuffer *Buffer;
	bool ret = false;

	pthread_mutex_lock(&m_Lock);
	for (int i = 0; i < m_InStreamNum; i++) {
		Stream = m_InStreams[i];
		if (Stream && Stream->m_Buffer
		&& Stream->m_Type == Type) {
			Buffer = Stream->m_Buffer;
			ret = Buffer->PopRelease(Bytes);
			break;
		}
	}
	pthread_mutex_unlock(&m_Lock);

	return ret;
}

void CAudioStream::SetPath(const char *path)
{
	if (strlen(path) > sizeof(m_FileDir)) {
		pr_err("%s: Over File Path Size %d (%s)!\n",
			__func__, sizeof(m_FileDir), path);
		return;
	}

	strcpy(m_FileDir, path);
}

unsigned int CAudioStream::GetCommand(void)
{
	return m_Command;
}

unsigned int CAudioStream::SetCommand(unsigned int Cmd, unsigned int Mask)
{
	pthread_mutex_lock(&m_CmdLock);
	m_Command = (m_Command & ~Mask) | (Cmd & Mask);
	pthread_mutex_unlock(&m_CmdLock);

	return m_Command;
}
