#ifndef _NX_PDM_H_
#define _NX_PDM_H_

#if __cplusplus
extern "C" {
#endif

typedef struct {
	int GeneratedFilterBlock_states[63];
} pdmLPF_STATDEF;

void PDM_LPF_Init(pdmLPF_STATDEF *lpf_st);
void PDM_LPF_Run(pdmLPF_STATDEF *lpf_st, short *pOutBuf,
			short *pInBuf, int len);

/*
 * Static Variables
 */
typedef struct {
	unsigned int start_sign;
	int frames;
	long int sum;
	int iGain;
	int IntpGain;
	int max1, max2, max3, max4;
	int dc_offset;
	int sum_table[20];
	pdmLPF_STATDEF lpf_st;
	unsigned int end_sign;
} agc_STATDEF;

#define	START_AGC_SIGN	0x12345678
#define	END_AGC_SIGN	0xDEADDEAD

/*
 * function prototypes
 */
void  agc_Init(agc_STATDEF  *agc_st);

/*
 * agc_st	: address of the stativ variable structure
 * plta		: signal
 */
#ifdef AGC_DEBUG
void agc_Agc(agc_STATDEF *agc_st, short *signal, short *plta, int dB);
#else
void agc_Run(agc_STATDEF *agc_st, short *signal, int dB);
#endif

typedef struct {
	agc_STATDEF agc_st[4];
	int Sigma1[4], Sigma2[4], Sigma3[4], Sigma4[4], Sigma5[4];
	int Delta1[4], Delta2[4], Delta3[4], Delta4[4];
	int OldDelta1[4], OldDelta2[4], OldDelta3[4], OldDelta4[4];
	int OldSigma5[4];
} pdm_STATDEF;

void pdm_Init(pdm_STATDEF *pdm_STATDEF);

/*
 * outbuf	: 2048 byte (256 sample : L/R | L/R)
 * inbuf	: 2048  * 4 = 8 KB : 8byte (L) -> 2byte (L) (64KHz->16KHz)
 */
void pdm_Run(pdm_STATDEF *pdm_st, short *outbuf, int *inbuf, int agc_dB);

#define SAMPLE_LENGTH_PDM	256

#if __cplusplus
}
#endif

#endif	/* _NX_PDM_H_ */
