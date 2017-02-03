#ifndef _WAV_FILE_H_
#define _WAV_FILE_H_

#include <stdio.h>
#include <stdarg.h>
#include <cstdint> /* include this header for uint64_t */
#include "util_debug.h"

#if __cplusplus
extern "C" {
#endif

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

struct wav_header {
	uint32_t riff_id;
	uint32_t riff_sz;
	uint32_t riff_fmt;
	uint32_t fmt_id;
	uint32_t fmt_sz;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
	uint32_t data_id;
	uint32_t data_sz;
};

class CWAVFile {
public:
	~CWAVFile(void)	{
		if (m_hFile)
			Close();
	}

	CWAVFile(void) {
		m_hFile = NULL;
		m_SavePeriodBytes = 0;
		m_SavePeriodAvail = 0;
		m_Channels = 0;
		m_SampleRate = 0;
		m_SampleBits = 0;
		m_WriteBytes = 0;
		m_bWavFormat = true;

		memset(m_strName, 0, sizeof(m_strName));
	}

	CWAVFile(long save_period_size) {
		m_hFile = NULL;
		m_SavePeriodBytes = 0;
		m_SavePeriodAvail = 0;
		m_Channels = 0;
		m_SampleRate = 0;
		m_SampleBits = 0;
		m_WriteBytes = 0;
		m_bWavFormat = true;

		memset(m_strName, 0, sizeof(m_strName));
	}
public:
	bool Open(int channels,
		int samplerate, int samplebits, const char *path)
	{
		struct wav_header *header = &m_WavHeader;

		if (m_hFile) {
			pr_err("E: Already opened (%s) ...\n", m_strName);
			return false;
		}

		m_hFile = fopen(path, "wb");
		if (!m_hFile) {
			pr_err("E: Unable to create file '%s' !!!\n", path);
			return false;
		}

		strcpy(m_strName, path);
		m_bWavFormat = true;

		if (m_bWavFormat) {
			memset(header, 0, sizeof(struct wav_header));

			header->riff_id = ID_RIFF;
			header->riff_sz = 0;
			header->riff_fmt = ID_WAVE;
			header->fmt_id = ID_FMT;
			header->fmt_sz = 16;
			header->audio_format = FORMAT_PCM;
			header->num_channels = channels;
			header->sample_rate = samplerate;
			header->bits_per_sample = samplebits;
			header->byte_rate = (header->bits_per_sample / 8) *
				channels * samplerate;
			header->block_align = channels *
				(header->bits_per_sample / 8);
			header->data_id = ID_DATA;

			/* write defaut header */
			fseek(m_hFile, 0, SEEK_SET);
			fwrite(header, sizeof(struct wav_header), 1, m_hFile);

			/* leave enough room for header */
			fseek(m_hFile, sizeof(struct wav_header), SEEK_SET);
		}

		pr_info("%s open : %s %d ch, %d hz, %d bits\n",
			m_bWavFormat ? "WAV" : "RAW", m_strName, channels,
			samplerate, samplebits);

		m_Channels = channels;
		m_SampleRate = samplerate;
		m_SampleBits = samplebits;
		m_WriteBytes = 0;

		return true;
	}

	bool Write(void *buffer, size_t size)
	{
		if (!m_hFile)
			return false;

		if (size != fwrite(buffer, 1, size, m_hFile)) {
			pr_err("E: write wave file to %s size %d !!!\n",
				m_strName, (int)size);
			return false;
		}

		m_WriteBytes += size;

		if (m_bWavFormat && m_SavePeriodBytes) {
			m_SavePeriodAvail -= m_WriteBytes;
			if (m_SavePeriodAvail < 0) {
				struct wav_header *header = &m_WavHeader;
				long long frames;

				frames = m_WriteBytes /
					((m_SampleBits / 8) * m_Channels);
				header->data_sz = frames * header->block_align;
				header->riff_sz = header->data_sz +
					sizeof(header) - 8;
				fseek(m_hFile, 0, SEEK_SET);
				fwrite(header,
					sizeof(struct wav_header), 1, m_hFile);
				fseek(m_hFile, 0, SEEK_END);
				m_SavePeriodAvail = m_SavePeriodBytes;
			}
		}
		return true;
	}

	void	Close(void)
	{
		struct wav_header *header = &m_WavHeader;

		if (!m_hFile)
			return;

		pr_info("%s close: %s %d ch, %d hz, %d bits, %lld bytes\n",
			m_bWavFormat ? "WAV" : "RAW", m_strName,
			m_Channels, m_SampleRate, m_SampleBits, m_WriteBytes);

		if (m_bWavFormat) {
			long long frames = m_WriteBytes /
				((m_SampleBits / 8) * m_Channels);
			/* write header now all information is known */
			header->data_sz = frames * header->block_align;
			header->riff_sz = header->data_sz + sizeof(header) - 8;

			fseek(m_hFile, 0, SEEK_SET);
			fwrite(header, sizeof(struct wav_header), 1, m_hFile);
		}

		fclose(m_hFile);

		m_hFile = NULL;
		m_WriteBytes = 0;
		m_SavePeriodAvail = 0;

		memset(m_strName, 0, sizeof(m_strName));
	}

private:
	FILE *m_hFile;
	struct wav_header m_WavHeader;

	char m_strName[512];
	long long m_WriteBytes;
	int  m_Channels, m_SampleRate, m_SampleBits;
	bool m_bWavFormat;
	long long m_SavePeriodBytes;
	long long m_SavePeriodAvail;
};

#if 0
static FILE *open_wave(char *path, int channels,
			int rate, int bits, struct wav_header *header)
{
	FILE *file;

	if (!header)
		return NULL;

	file = fopen(path, "wb");
	if (!file) {
		pr_err("E: Unable to create file '%s'\n", path);
		return NULL;
	}
	memset(header, 0, sizeof(*header));

	header->riff_id = ID_RIFF;
	header->riff_sz = 0;
	header->riff_fmt = ID_WAVE;
	header->fmt_id = ID_FMT;
	header->fmt_sz = 16;
	header->audio_format = FORMAT_PCM;
	header->num_channels = channels;
	header->sample_rate = rate;
	header->bits_per_sample = bits;
	header->byte_rate = (header->bits_per_sample / 8) *
			channels * rate;
	header->block_align = channels * (header->bits_per_sample / 8);
	header->data_id = ID_DATA;

	/* leave enough room for header */
	fseek(file, sizeof(struct wav_header), SEEK_SET);
	pr_info("wave open %s: %u ch, %d hz, %u bits\n",
		path, channels, rate, bits);

	return file;
}

static int write_wave(FILE *file, void *buffer, int size)
{
	if (!file)
		return -1;

	int ret = fwrite(buffer, 1, size, file);

	if (ret != size) {
		pr_err("E: Error capturing sample ....\n");
		return ret;
	}

	return ret;
}

static void close_wave(FILE *file, struct wav_header *header,
				int channels, int bits, unsigned long bytes)
{
	long long frames;

	if (!file || bits == 0 || channels == 0)
		return;

	pr_info("wave close: %u ch, %d hz, %u bits, %lu bytes\n",
		channels, header->sample_rate, bits, bytes);

	frames = bytes / ((bits / 8) * channels);

	/* write header now all information is known */
	header->data_sz = frames * header->block_align;
	header->riff_sz = header->data_sz + sizeof(header) - 8;

	fseek(file, 0, SEEK_SET);
	fwrite(header, sizeof(struct wav_header), 1, file);
	fclose(file);
	pr_info("[done]\n");
}
#endif

#if __cplusplus
}
#endif

#endif /* _WAV_FILE_H_ */
