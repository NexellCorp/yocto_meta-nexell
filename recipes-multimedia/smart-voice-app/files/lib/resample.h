/*
 * copyright (c) 2001 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file avcodec.h
 * external API header
 */
#ifdef	__KERNEL__
#include <linux/kernel.h>
#include <linux/slab.h>

#ifndef assert
#define assert(expr) \
        if(unlikely(!(expr))) {                                   \
        printk(KERN_ERR "Assertion failed! %s,%s,%s,line=%d\n", \
    		#expr, __FILE__, __func__, __LINE__);          \
        }
#endif
#endif

#ifndef CNULL
#define CNULL	NULL
#endif

#ifdef	__KERNEL__
#define printf( str )			printk( str )
#define av_log( p, t, str )		printk( str )
#define MES_ASSERT				assert
#define MES_realloc(p, s)		krealloc(p, s, GFP_KERNEL)
#define MES_malloc(s)			kmalloc(s, GFP_KERNEL)
#define MES_free(p)				kfree(p)
#else
#define av_log( p, t, str )		printf( str )
#define MES_ASSERT				assert
#define MES_realloc				realloc
#define MES_malloc				malloc
#define MES_free					free
#endif


void *av_malloc(unsigned int size);
void *av_realloc(void *ptr, unsigned int size);
void av_free(void *ptr);
void *av_mallocz(unsigned int size);
void av_freep(void *arg);

typedef signed char			int8_t;
typedef	signed short int	int16_t;
typedef signed int			int32_t;
typedef	signed long long	int64_t;

typedef unsigned char		uint8_t;
typedef	unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

#ifndef INT16_MAX
#define INT16_MAX		(0x7FFF)
#endif
#ifndef INT16_MIN
#define INT16_MIN		(-0x7FFF-1)
#endif

#ifndef INT32_MAX
#define INT32_MAX		(0x7FFFFFFF)
#endif
#ifndef INT32_MIN
#define INT32_MIN		(-0x7FFFFFFF-1)
#endif

#ifndef INT_MAX
#define INT_MAX			INT32_MAX
#endif

#define FFABS(a) ((a) >= 0 ? (a) : (-(a)))

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))



/* resample.c */

struct ReSampleContext;
struct AVResampleContext;

typedef struct ReSampleContext ReSampleContext;

ReSampleContext *audio_resample_init(int output_channels, int input_channels,
                                     float output_rate, float input_rate);
int audio_resample(ReSampleContext *s, short *output, short *input, int nb_samples);
void audio_resample_close(ReSampleContext *s);

struct AVResampleContext *av_resample_init(int out_rate, int in_rate, int filter_length, int log2_phase_count, int linear, double cutoff);
int av_resample(struct AVResampleContext *c, short *dst, short *src, int *consumed, int src_size, int dst_size, int update_ctx);
void av_resample_compensate(struct AVResampleContext *c, int sample_delta, int compensation_distance);
void av_resample_close(struct AVResampleContext *c);

#ifdef __cplusplus
}
#endif


#endif /* RESAMPLE_H */
