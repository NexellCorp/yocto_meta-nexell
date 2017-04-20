#ifndef _AUDIOSTREAM_H_
#define _AUDIOSTREAM_H_

#include "audioplay.h"
#include "qbuff.h"
#include "wav.h"
#include "util.h"
#include "util_debug.h"

enum STREAM_CMD_TYPE {
	STREAM_CMD_GET,
	STREAM_CMD_SET,
	STREAM_CMD_CLR,
};

class CAudioStream {
public:
	CAudioStream(void);
	CAudioStream(const char *Name, unsigned int Type, bool AllocBuffer);
	virtual ~CAudioStream(void);

	void Init(int Card, int Device,
		int Channels, int SampleRate, int SampleBits,
		int PeriodBytes, int Periods);
	void Release(void);

	/* Audio */
	bool AudioOpen(enum AUDIO_STREAM_DIR Direction);
	bool AudioOpen(int Card, int Device,
			int Channels, int SampleRate, int SampleBits,
			int PeriodBytes, int Periods,
			enum AUDIO_STREAM_DIR Direction);
	void AudioClose(void);
	bool AudioStart(void);
	void AudioStop(void);
	int  AudioPlayBack(unsigned char *Buffer, int Bytes);
	int  AudioCapture(unsigned char *Buffer, int Bytes);

	/* buffer */
	void OutBufIsFull(unsigned int Type);
	void OutBufClear(unsigned int Type);
	unsigned char *OutBufPush(unsigned int Type, int Bytes, int WaitTime);
	bool OutBufPushRel(unsigned int Type, int Bytes);

	void InBufIsEmpty(unsigned int Type);
	void InBufWaitForClean(unsigned int Type);
	void InBufWaitForCleanAll(void);
	unsigned char *InBufPop(unsigned int Type, int Bytes, int WaitTime);
	bool InBufPopRel(unsigned int Type, int Bytes);

	void AddInputStream(CAudioStream *Stream);
	CAudioStream *GetInputStreamObj(void);
	CAudioStream *GetInputStreamObj(int Index);

	unsigned int GetCommand(void);
	unsigned int SetCommand(unsigned int Cmd, unsigned int Mask);

	void SetPath(const char *path);
	void SetValue(int value) { m_Value = value; }


	/* audio stream info */
	char *GetPath(void) { return m_FileDir; }
	const char *GetName(void) { return m_Name; }
	unsigned int GetType(void) {return m_Type; }

	int GetCard(void) { return m_Card; }
	int GetDevice(void) { return m_Device; }
	int GetPeriodBytes(void) { return m_PeriodBytes; }
	int GetPeriods(void) { return m_Periods; }
	int GetChannels(void) { return m_Channels; }
	int GetSampleRate(void) { return m_SampleRate; }
	int GetSampleBits(void) { return m_SampleBits; }
	int GetValue(void) { return m_Value; }

	CQueueBuffer *GetBufferObj(void) { return m_Buffer; }
	unsigned int GetInputStreamType(void);
	unsigned int GetInputStreamType(int index);
	int GetInputStreamNums(void) { return m_InStreamNum; }

public:
	CWAVFile *WavFile[5];
	int WavFileNum;

private:
	const char *m_Name;
	char m_FileDir[256];
	int m_Card;
	int m_Device;
	pthread_mutex_t m_Lock, m_CmdLock;
	bool m_bValid;
	int m_Channels;
	int m_SampleRate;
	int m_SampleBits;
	int m_Periods;
	int m_PeriodBytes;
	bool m_AllocBuffer;

	unsigned int m_Type;
	unsigned int m_Command;

	CAudioStream *m_InStreams[10];
	int m_InStreamNum;

	CQueueBuffer *m_Buffer;
	int m_Value;

	CAudioPlayer *m_AudioPlayer;
};

#endif /* _AUDIOSTREAM_H_ */
