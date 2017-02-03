
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "audioplay.h"
#include "util_debug.h"


#define	DUMMY_STREAM_TIME_MS	30

CAudioPlayer::CAudioPlayer(enum AUDIO_STREAM_DIR stream)
	: m_hPCM(NULL)
	, m_Channels(2)
	, m_SampleBytes(2)
{
	m_Stream = stream == AUDIO_STREAM_PLAYBACK ? PCM_OUT : PCM_IN;
}

CAudioPlayer::~CAudioPlayer(void)
{
	if (m_hPCM)
		Close();
}

bool CAudioPlayer::Open(const char *pcm_name, int card, int device,
			int channels, int samplerate, int samplebits,
			int periods, int periodbytes)
{
    m_PCMConfig.channels = channels;
    m_PCMConfig.rate = samplerate;
    m_PCMConfig.period_size = periodbytes/(channels*(samplebits/8));
    m_PCMConfig.period_count = periods;

	pr_aplay("%s %s card:%d.%d ch.%d, %d hz, %d bits, period bytes %d(%d), periods %d \n",
		pcm_name, m_Stream == PCM_OUT ? "Playing":"Capturing",
		card, device, channels, samplerate,
		samplebits, periodbytes, m_PCMConfig.period_size, periods);

    if (samplebits == 32)
        m_PCMConfig.format = PCM_FORMAT_S32_LE;
    else if (samplebits == 16)
        m_PCMConfig.format = PCM_FORMAT_S16_LE;
	else {
  		pr_err("E: %s Unable to open card %d.%d ch.%d, %d hz, %d bits period %d(%d) bytes, periods %d\n",
			m_Stream == PCM_OUT ? "Playing":"Capturing",
			card, device, channels, samplerate, samplebits,
			periodbytes, m_PCMConfig.period_size, periods);
        return false;
	}

	if (pcm_name && strlen(pcm_name))
		strcpy(m_pcmName, pcm_name);

    m_PCMConfig.start_threshold = 0;
    m_PCMConfig.stop_threshold = 0;
    m_PCMConfig.silence_threshold = 0;

    m_hPCM = (struct pcm *)1;

	m_SampleRate = samplerate;
	m_Card = card;
	m_Device = device;
	m_Channels = channels;
    m_SampleBits = samplebits;
    m_PeriodBytes = periodbytes;
    m_Periods = periods;
	m_FrameBytes = channels*(samplebits/8);

	return true;
}

void CAudioPlayer::Close(void)
{
	m_hPCM = NULL;
}

int CAudioPlayer::Capture(unsigned char *buffer, int bytes)
{
	int us = 0;

	if (!strncmp(m_pcmName, "default:CARD=SNDNULL",
			strlen("default:CARD=SNDNULL"))) {
		us = 42666;	/* (1000000/48000) * (8192/4) */
		usleep(us);
	}

	if (!strncmp(m_pcmName, "default:CARD=PDMRecorder",
			strlen("default:CARD=PDMRecorder"))) {
		us = 10*1000;
		usleep(us);
	}

	for (int i = 0; bytes > i; i++)
		buffer[i] = random() & 0xff;

    return bytes;
}

int CAudioPlayer::PlayBack(unsigned char *buffer, int bytes)
{
	usleep(DUMMY_STREAM_TIME_MS * 1000);
	return bytes;
}

void CAudioPlayer::Stop(bool drop)
{
}

void CAudioPlayer::Stop(void)
{
}

int CAudioPlayer::GetChannels(void)
{
	return m_Channels;
}

int CAudioPlayer::GetSamplRate(void)
{
	return m_SampleRate;
}

int CAudioPlayer::GetPeriods(void)
{
	return m_Periods;
}

int CAudioPlayer::GetPeriodBytes(void)
{
	return m_PeriodBytes;
}

int CAudioPlayer::GetFrameBytes(void)
{
	return m_FrameBytes;
}

void CAudioPlayer::PcmInfo(void)
{
}

