/*
 * bmplib.h : header for bmplib(simple bmp reading library)
 *
 * Copyright(C) 2002 holelee
 *
 */

#ifndef __BMP_H
#define __BMP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

/**
 * bmpfileheader_t:
 */
typedef struct {
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t off_bits;
} bmpfileheader_t;

/**
 * bmpinfoheader_t:
 */
typedef struct {
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t size_image;
	uint32_t x_pels_per_meter;
	uint32_t y_pels_per_meter;
	uint32_t clr_used;
	uint32_t clr_important;
} bmpinfoheader_t;

/**
 * palette_s:
 */
struct palette_s {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char filter;
};

/**
 * rgbpixel_t:
 */
typedef struct {
	unsigned char r, g, b;
} rgbpixel_t;

/**
 * bmp_t:
 */
typedef struct bmp_s bmp_t;
struct bmp_s {
	/* bmp header contents */
	bmpfileheader_t bmpfile_h;
	bmpinfoheader_t bmpinfo_h;

	FILE *fp;
	char *memory;
	int npalette;
	int bytes_per_line;
	struct palette_s *palette;
	unsigned char *data;
	unsigned int bsize_blue;
	unsigned int bsize_green;
	unsigned int bsize_red;
	unsigned int boffset_blue;
	unsigned int boffset_green;
	unsigned int boffset_red;
	rgbpixel_t (*getpixel)(bmp_t *, int, int);
};

bmp_t *bmp_load(const char *bmpfile, void *data);
void bmp_release(bmp_t *bmp);
int bmp_height(bmp_t *bmp);
int bmp_width(bmp_t *bmp);

rgbpixel_t bmp_getpixel(bmp_t *bmp, int x, int y);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif

