#ifndef _AUDIO_PLAY_H_
#define _AUDIO_PLAY_H_

#include "tinyalsa_asoundlib.h"

enum AUDIO_STREAM_DIR {
	AUDIO_STREAM_PLAYBACK,
	AUDIO_STREAM_CAPTURE,
};

class CAudioPlayer {
public:
	CAudioPlayer(void);
	CAudioPlayer(const char *Name, int Card, int Device,
			int Channel, int SampleRate, int SampleBits,
			int Periods, int PeriodBytes,
			enum AUDIO_STREAM_DIR Direction);

	virtual ~CAudioPlayer(void);

public:
	bool Open(const char *Name, int Card, int Device,
			int Channel, int SampleRate, int SampleBits,
			int Periods, int PeriodBytes,
			enum AUDIO_STREAM_DIR Direction);
	void Close(void);

	bool Start(void);
	bool Stop(void);
	bool Stop(bool Drop);

	/* get audio data */
	int Capture(unsigned char *Buffer, int Byte);
	int PlayBack(unsigned char *Buffer, int Byte);

	int GetChannels(void) { return m_Channels; }
	int GetSamplRate(void) { return m_SampleRate; }
	int GetPeriods(void) { return m_Periods; }
	int GetPeriodBytes(void) { return m_PeriodBytes; }
	int GetFrameBytes(void) { return m_FrameBytes; }

	void PcmInfo(int Card, int Device,
			enum AUDIO_STREAM_DIR Direction);

private:
	struct pcm_config m_PCMConfig;
	struct pcm *m_hPCM;
	int m_Card, m_Device, m_SampleBits;
	int m_Stream;	/* Stream Direction : PCM_OUT/PCM_IN */
	int m_PeriodBytes;
	char m_PcmName[256];

	int m_Channels;
	int m_SampleRate;
	int m_Periods;
	int m_FrameBytes;	/* samplebytes * channel */
};

#endif /* _AUDIO_PLAY_H_ */
