#ifndef _Q_BUFF_H_
#define _Q_BUFF_H_

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <assert.h>
#include <sys/time.h>

#include "util_debug.h"

#if __cplusplus
extern "C" {
#endif

#define	ATOMIC_LOCK
#define	TIMEOUT_INFINITE	(0xFFFFFFFF)

class CQueueBuffer {
public:
	CQueueBuffer(int Frames, int FrameBytes)
	{
		qbuffer(Frames, FrameBytes);
		memset(m_qName, 0, sizeof(m_qName));
		m_Type = 0;
	}

	virtual ~CQueueBuffer(void) { delete m_pBuffer;	};

	CQueueBuffer(int Frames, int FrameBytes,
			const char *Name, unsigned int Type)
	{
		qbuffer(Frames, FrameBytes);
		if (Name && strlen(Name))
			strcpy(m_qName, Name);
		m_Type = Type;
	}

public:
	int GetFrameCounts(void) { return m_FrameCounts; }
	int GetFrameBytes(void) { return m_FrameBytes; }
	int GetBufferBytes(void) { return m_BufferBytes; }
	void *GetBufferBase(void) { return (void *)m_pBuffer; }

	char *GetBufferName(void) { return m_qName; }
	int GetBufferType(void) { return m_Type; }

	int GetAvailSize(void) { return m_PushAvailSize; }
	bool Is_PushAvail(void) { return m_bPushAvail; }
	bool Is_PopAvail(void) { return m_bPopAvail;  }

public:

	bool  Is_Full(void)
	{
		pthread_mutex_lock(&m_qLock);
		bool full = (!m_PushAvailSize ? true : false);
		pthread_mutex_unlock(&m_qLock);

		return full;
	}

	bool  Is_Empty(void)
	{
		pthread_mutex_lock(&m_qLock);
		bool empty = (m_BufferBytes == m_PushAvailSize ? true : false);
		pthread_mutex_unlock(&m_qLock);

		return empty;
	}

	/* with frame bytes */
	unsigned char *PushBuffer(void)
	{
		unsigned char *Buffer = NULL;

		if (!m_pBuffer)
			return NULL;

		pthread_mutex_lock(&m_qLock);

		if (!m_bPushAvail)
			pthread_cond_wait(&m_PopEvent, &m_qLock);

		Buffer = (unsigned char *)(m_pBuffer + m_HeadPos);
		pthread_mutex_unlock(&m_qLock);

		return Buffer;
	}

	bool PushRelease(void)	/* with frame bytes */
	{
		bool bPushAvail = false;

		if (!m_pBuffer)
			return false;

		pthread_mutex_lock(&m_qLock);

		if (!m_bPushAvail)
			pthread_cond_wait(&m_PopEvent, &m_qLock);

		m_HeadPos += m_FrameBytes;

		if (m_HeadPos >= m_BufferBytes)
			m_HeadPos = 0;

		m_bPushAvail = m_HeadPos == m_TailPos ? false : true;
		m_bPopAvail = true;
		bPushAvail = m_bPushAvail;

		pthread_cond_signal(&m_PushEvent);
		pthread_mutex_unlock(&m_qLock);

		return bPushAvail;
	}

	unsigned char *PopBuffer(void)	/* with frame bytes unit */
	{
		unsigned char *Buffer = NULL;

		if (!m_pBuffer)
			return NULL;

		pthread_mutex_lock(&m_qLock);

		if (!m_bPopAvail)
			pthread_cond_wait(&m_PushEvent, &m_qLock);

		Buffer = (unsigned char *)(m_pBuffer + m_TailPos);
		pthread_mutex_unlock(&m_qLock);

		return Buffer;
	}

	bool PopRelease(void)	/* with frame bytes unit */
	{
		bool bPopAvail = false;

		if (!m_pBuffer)
			return false;

		pthread_mutex_lock(&m_qLock);

		if (!m_bPopAvail)
			pthread_cond_wait(&m_PushEvent, &m_qLock);

		m_TailPos += m_FrameBytes;

		if (m_TailPos >= m_BufferBytes)
			m_TailPos = 0;

		m_bPushAvail = true;
		m_bPopAvail = m_HeadPos == m_TailPos ? false : true;
		bPopAvail = m_bPopAvail;

		pthread_cond_signal(&m_PopEvent);
		pthread_mutex_unlock(&m_qLock);

		return bPopAvail;
	}

	unsigned char *PushBuffer(int size, int wait)	/* ms */
	{
		unsigned char *Buffer = NULL;

		if (!m_pBuffer || !m_bPushValid || 0 >= size)
			return NULL;

		pthread_mutex_lock(&m_qLock);

		if ((m_HeadPos + size) > m_BufferBytes) {
			pr_err("%s : Error %d size %d pos, over buffer: %d, check align!!!\n",
				__func__, size, m_HeadPos, m_BufferBytes);
			assert((m_HeadPos + size) <= m_BufferBytes);
		}

		if (!wait) {
			Buffer = (unsigned char *)(m_pBuffer + m_HeadPos);
			pthread_mutex_unlock(&m_qLock);
			return Buffer;
		}

		while (size > m_PushAvailSize && m_bPushValid) {
			if ((unsigned int)wait != TIMEOUT_INFINITE) {
				struct timespec ts;
				int ret;

				get_pthread_wait_time(&ts, wait);
				ret = pthread_cond_timedwait(
						&m_PopEvent, &m_qLock, &ts);
				if (ret == ETIMEDOUT) {
					pthread_mutex_unlock(&m_qLock);
					return NULL;
				}
			} else {
				pthread_cond_wait(&m_PopEvent, &m_qLock);
			}
		}

		Buffer = (unsigned char *)(m_pBuffer + m_HeadPos);
		pthread_mutex_unlock(&m_qLock);

		return Buffer;
	}

	bool PushRelease(int size)
	{
		bool bPushAvail = false;

		if (!m_pBuffer || !m_bPushValid || 0 >= size)
			return false;

		pthread_mutex_lock(&m_qLock);

		while (m_PushAvailSize < size && m_bPushValid)
			pthread_cond_wait(&m_PopEvent, &m_qLock);

		if ((m_HeadPos + size) > m_BufferBytes) {
			pr_err("%s : Error %d size %d pos, over buffer: %d, check align!!!\n",
				__func__, size, m_HeadPos, m_BufferBytes);
			assert((m_HeadPos + size) <= m_BufferBytes);
		}

		m_PushAvailSize -= size;
		m_HeadPos = (m_HeadPos + size) % m_BufferBytes;

		m_bPushAvail = m_PushAvailSize > 0 ? true : false;
		m_bPopAvail = true;
		bPushAvail = m_bPushAvail;

		pthread_cond_signal(&m_PushEvent);
		pthread_mutex_unlock(&m_qLock);

		return bPushAvail;
	}

	unsigned char *PopBuffer(int size, int wait)
	{
		unsigned char *Buffer = NULL;

		if (!m_pBuffer || !m_bPopValid)
			return NULL;

		pthread_mutex_lock(&m_qLock);

		if ((m_TailPos + size) > m_BufferBytes) {
			pr_err("%s : Error %d size %d pos, over buffer: %d, check align!!!\n",
				__func__, size, m_TailPos, m_BufferBytes);
			assert((m_TailPos + size) <= m_BufferBytes);
		}

		if (!wait) {
			Buffer = (unsigned char *)(m_pBuffer + m_TailPos);
			pthread_mutex_unlock(&m_qLock);
			return Buffer;
		}

		while ((m_BufferBytes - m_PushAvailSize) < size &&
			m_bPopValid) {

			if ((unsigned int)wait != TIMEOUT_INFINITE) {
				struct timespec ts;
				int ret;

				get_pthread_wait_time(&ts, wait);
				ret = pthread_cond_timedwait(
						&m_PushEvent, &m_qLock, &ts);
				if (ret == ETIMEDOUT) {
					pthread_mutex_unlock(&m_qLock);
					return NULL;
				}
			} else {
				pthread_cond_wait(&m_PushEvent, &m_qLock);
			}
		}

		Buffer = (unsigned char *)(m_pBuffer + m_TailPos);
		pthread_mutex_unlock(&m_qLock);

		return Buffer;
	}

	bool PopRelease(int size)
	{
		bool bPopAvail = false;

		if (!m_pBuffer || !m_bPopValid || 0 >= size)
			return false;

		pthread_mutex_lock(&m_qLock);

		while ((m_BufferBytes - m_PushAvailSize) < size
			&& m_bPopValid)
			pthread_cond_wait(&m_PushEvent, &m_qLock);

		if ((m_TailPos + size) > m_BufferBytes) {
			pr_err("%s : Error %d size %d pos, over buffer: %d, check align!!!\n",
				__func__, size, m_TailPos, m_BufferBytes);
			assert((m_TailPos + size) <= m_BufferBytes);
		}

		m_PushAvailSize += size;
		m_TailPos = (m_TailPos + size) % m_BufferBytes;

		m_bPushAvail = true;
		m_bPopAvail = (m_BufferBytes - m_PushAvailSize) > 0 ?
				true : false;
		bPopAvail = m_bPopAvail;
		pthread_cond_signal(&m_PopEvent);
		pthread_mutex_unlock(&m_qLock);

		return bPopAvail;
	}

	/*
	 * PushBuffer side :
	 * must be call 'WaitForClear' at PopBuffer side
	 */
	void ClearBuffer(void)
	{
		pthread_mutex_lock(&m_qLock);

		m_bPopValid = false;
		pthread_cond_signal(&m_PushEvent);
		pthread_cond_signal(&m_PopEvent);

		/* initialize */
		m_HeadPos = 0, m_TailPos = 0;
		m_bPushAvail = true, m_bPopAvail = false;
		m_PushAvailSize = m_BufferBytes;
		memset(m_pBuffer, 0, m_AlignBufferBytes);

		assert(m_pBuffer);
		assert(m_AlignBufferBytes);

		m_bWaitValid = true;
		m_bPushValid = true;
		pthread_cond_signal(&m_ClearEvent);

		pthread_mutex_unlock(&m_qLock);
	}

	/*
	 * PopBuffer side:
	 * must be call 'ClearBuffer' at PushBuffer side
	 */
	void WaitForClear(void)
	{
		pthread_mutex_lock(&m_qLock);

		if (!m_bWaitValid) {
			m_bPushValid = false;
			pthread_cond_signal(&m_PushEvent);
			pthread_cond_signal(&m_PopEvent);
			/* wait clear event */
			pthread_cond_wait(&m_ClearEvent, &m_qLock);
		}

		m_bWaitValid = false;
		m_bPopValid = true;

		pthread_mutex_unlock(&m_qLock);
	}

private:
	void qbuffer(int Frames, int FrameBytes)
	{
		int align_bytes = (((Frames * FrameBytes) + 3) / 4) * 4;

		m_pBuffer = (unsigned char *)new unsigned int[align_bytes/4];

		m_FrameCounts = Frames;
		m_FrameBytes = FrameBytes;
		m_BufferBytes = Frames * FrameBytes;
		m_AlignBufferBytes = align_bytes;

		m_HeadPos = 0, m_TailPos = 0;
		m_bPushAvail = true, m_bPopAvail = false;
		m_PushAvailSize = m_BufferBytes;
		m_bWaitValid = false;

		m_bPushValid = true;
		m_bPopValid = true;

		assert(m_pBuffer);
		assert(m_AlignBufferBytes);

		memset(m_pBuffer, 0, m_AlignBufferBytes);

		pthread_mutex_init(&m_qLock, NULL);

		pthread_cond_init(&m_PushEvent, NULL);
		pthread_cond_init(&m_PopEvent, NULL);
		pthread_cond_init(&m_ClearEvent, NULL);
	}

	static void get_pthread_wait_time(struct timespec *ts, int ms)
	{
		struct timeval tv;

		gettimeofday(&tv, NULL);

		ts->tv_sec = time(NULL) + ms / 1000;
		ts->tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (ms % 1000);
		ts->tv_sec += ts->tv_nsec / (1000 * 1000 * 1000);
		ts->tv_nsec %= (1000 * 1000 * 1000);
	}

private:
	unsigned char *m_pBuffer;
	int m_FrameCounts;
	int m_FrameBytes;
	int m_BufferBytes;
	int m_AlignBufferBytes;
	char m_qName[256];
	unsigned int m_Type;

	/* std::atomic<xxx> */
	int m_HeadPos;	/* push */
	int m_TailPos;	/* Pop */
	bool m_bPushAvail;
	bool m_bPopAvail;
	int m_PushAvailSize;

	pthread_mutex_t m_qLock;
	pthread_cond_t  m_PushEvent, m_PopEvent, m_ClearEvent;
	bool m_bWaitValid, m_bPushValid, m_bPopValid;
};

#if __cplusplus
}
#endif
#endif /* _Q_BUFF_H_ */

