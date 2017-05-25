#ifndef NX_PDM_H
#define NX_PDM_H

#if __cplusplus
extern "C" {
#endif

typedef struct {
	int GeneratedFilterBlock_states[63];
} pdmLPF_STATDEF;

typedef struct {
	int frames;
	long int sum;
	int iGain;
	int IntpGain;
	int max1, max2, max3, max4;
	int dc_offset;
	int sum_table[20];
	pdmLPF_STATDEF lpf_st;
} agc_STATDEF;

typedef struct {
	agc_STATDEF agc_st[4];
	int Sigma1[4], Sigma2[4], Sigma3[4], Sigma4[4], Sigma5[4];
	int Delta1[4], Delta2[4], Delta3[4], Delta4[4];
	int OldDelta1[4], OldDelta2[4], OldDelta3[4], OldDelta4[4];
	int OldSigma5[4];
} pdm_STATDEF;

/*
 * pdm_Init - initialize pdm to pcm function
 * @pdm_STATDEF: address of the stativ variable structure
 */
void pdm_Init(pdm_STATDEF *pdm_STATDEF);

/**
 * pdm_Run - covert 4 channel pdm raw data to 4 channel pcm data
 *
 * @pdm_st: pointer to an pdm_STATDEF structure
 *
 * @outbuf: converted pcm data, data buffer size must be 2048 bytes,
 *	    pcm out data format : 4 channel pcm
 *		L0[16B] / R0[16B]
 *		L1[16B] / R1[16B]
 *		.....
 *
 * @inbuf: pdm raw data, data buffer size must be 8192 bytes,
 *	    pdm raw input data format :
 *		L0[1B]/R0[1B]/L1[1B]/R1[1B]/......
 *		.....
 *
 * @agc_dB: pcm data output gain value
 *
 * Convert pdm raw data (8192bytes) to interleved pcm format data(2048bytes)
 */
void pdm_Run(pdm_STATDEF *pdm_st, short *outbuf, int *inbuf, int agc_dB);

/**
 * pdm_Run_channel - covert pdm raw data to pcm data with channel
 *
 * @pdm_st: pointer to an pdm_STATDEF structure
 *
 * @outbuf: converted pcm data
 *	    pcm out data format :
 *	    if channels == 4, data buffer size must be 2048 bytes
 *		L0[16B] / R0[16B]
 *		L1[16B] / R1[16B]
 *		.....
 *	    if channels == 3, data buffer size must be 2048 bytes
 *		L0[16B] / R0[16B]
 *		L1[16B] / NULL
 *		.....
 *	    if channels == 2, data buffer size must be 1024 bytes
 *		L0[16B] / R0[16B]
 *		.....
 *	    if channels == 1, data buffer size must be 1024 bytes
 *		L0[16B] / NULL
 *		.....
 *
 * @inbuf: pdm raw data, data buffer size must be 8192 bytes,
 *	    pdm raw input data format :
 *		L0[1B]/R0[1B]/L1[1B]/R1[1B]/......
 *		.....
 *
 * @agc_dB: pcm data output gain value, if 0 disabled auto gain control
 *
 * @channels: pdm raw data channel count to convert pcm data, support 1 ~ 4 channel
 *
 * @be: swap to big-endian
 *
 * Convert pdm raw data (8192bytes) to interleved pcm format data(2048bytes)
 */
void pdm_Run_channel(pdm_STATDEF *pdm_st, short *outbuf, int *inbuf,
			int agc_dB, int ch, int swap);
#if __cplusplus
}
#endif

#endif	/* NX_PDM_H */
