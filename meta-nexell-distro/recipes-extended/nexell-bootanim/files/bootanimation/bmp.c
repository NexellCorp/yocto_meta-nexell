/*
 * bmplib.c : bmp reading library
 *            only little endian machine supported
 *
 * Copyright(C) 2002 holelee
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

/*
* for compression field
* BI_RGB : no compression (0)
* BI_RLE4 : RLE 8-bit/pixel (1)
* BI_RLE8 : RLE 4-bit/pixel (2)
* BI_BITFIELDS : bitfields (3)
*/
#define BI_RGB 0
#define BI_RLE4 1
#define BI_RLE8 2
#define BI_BITFIELD 3


/**
 * getpixel_1bpp:
 */
static rgbpixel_t getpixel_1bpp(bmp_t *bmp, int x, int y)
{
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	rgbpixel_t rgb;
	unsigned char *pdata;
	int offset = (bmpi->height - y) * bmp->bytes_per_line + (x / 8);
	int pixel1;
	int boffset;

	pdata = bmp->data + offset;
	boffset = x % 8;

	pixel1 = (*pdata & (0x80U >> boffset)) >> (7-boffset);

	/* palette lookup */
	rgb.b = bmp->palette[pixel1].blue;
	rgb.g = bmp->palette[pixel1].green;
	rgb.r = bmp->palette[pixel1].red;

	return rgb;
}


/**
 * getpixel_4bpp:
 */
static rgbpixel_t getpixel_4bpp(bmp_t *bmp, int x, int y)
{
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	rgbpixel_t rgb;
	unsigned char *pdata;
	int offset = (bmpi->height - y) * bmp->bytes_per_line + (x >> 1);
	int pixel4;
	int boffset;

	pdata = bmp->data + offset;
	boffset = (x & 0x01) << 2;

	pixel4 = (*pdata & (0xF0U >> boffset)) >> (4 - boffset);

	/* palette lookup */
	rgb.b = bmp->palette[pixel4].blue;
	rgb.g = bmp->palette[pixel4].green;
	rgb.r = bmp->palette[pixel4].red;

	return rgb;
}

/**
 * getpixel_8bpp:
 */
static rgbpixel_t getpixel_8bpp(bmp_t *bmp, int x, int y)
{
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	rgbpixel_t rgb;
	unsigned char *pdata;
	int offset = (bmpi->height - y - 1) * bmp->bytes_per_line + x;
	int pixel8;

	pdata = bmp->data + offset;
	pixel8 = *pdata;

	/* palette lookup */
	rgb.b = bmp->palette[pixel8].blue;
	rgb.g = bmp->palette[pixel8].green;
	rgb.r = bmp->palette[pixel8].red;

	return rgb;
}

/**
 * getpixel_16bpp:
 */
static rgbpixel_t getpixel_16bpp(bmp_t *bmp, int x, int y)
{
	/* BI_RGB case */
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	rgbpixel_t rgb;
	unsigned short *pdata;
	unsigned *mask = (unsigned *) (bmp->palette);
	int offset = (bmpi->height - y - 1) * bmp->bytes_per_line + (x << 1);

	pdata = (unsigned short *) (bmp->data + offset);
	rgb.b = ((*pdata & mask[2]) >> bmp->boffset_blue)
			<< (8 - bmp->bsize_blue);
	rgb.g = ((*pdata & mask[1]) >> bmp->boffset_green)
			<< (8 - bmp->bsize_green);
	rgb.r = ((*pdata & mask[0]) >> bmp->boffset_red)
			<< (8 - bmp->bsize_red);

	return rgb;
}

/**
 * getpixel_24bpp:
 */
static rgbpixel_t getpixel_24bpp(bmp_t *bmp, int x, int y)
{
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	rgbpixel_t rgb;
	unsigned char *pdata;
	int offset = (bmpi->height - y - 1) * bmp->bytes_per_line + x * 3;

	if (offset < 0 || offset > (int)bmpi->size_image - 3 ||
		x < 0 || y < 0 ||
		x >= (int)bmpi->width || y >= (int)bmpi->height) {
		rgb.r = 0;
		rgb.g = 0;
		rgb.b = 0;
		return rgb;
	}

	pdata = bmp->data + offset;
	rgb.b = *pdata;
	rgb.g = *(pdata + 1);
	rgb.r = *(pdata + 2);

	return rgb;
}

/**
 * getpixel_32bpp:
 */
static rgbpixel_t getpixel_32bpp(bmp_t *bmp, int x, int y)
{
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	rgbpixel_t rgb;
	unsigned *pdata;
	unsigned *mask = (unsigned *) (bmp->palette);
	int offset = (bmpi->height - y - 1) * bmp->bytes_per_line + (x << 2);

	pdata = (unsigned *) (bmp->data + offset);
	rgb.b = ((*pdata & mask[2]) >> bmp->boffset_blue)
			<< (8 - bmp->bsize_blue);
	rgb.g = ((*pdata & mask[1]) >> bmp->boffset_green)
			<< (8 - bmp->bsize_green);
	rgb.r = ((*pdata & mask[0]) >> bmp->boffset_red)
			<< (8 - bmp->bsize_red);

	return rgb;
}

/**
 * bmp_readheader:
 */
static int bmp_readheader(bmp_t *bmp)
{
	bmpfileheader_t *bmpf = &bmp->bmpfile_h;
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;
	unsigned char ID[2];
	int size, remnant;

	if (!bmp->fp && !bmp->memory)
		return -1;

	if (bmp->fp) {
		ID[0] = fgetc(bmp->fp);
		ID[1] = fgetc(bmp->fp);
	} else {
		ID[0] = bmp->memory[0];
		ID[1] = bmp->memory[1];
	}

	/* check ID */
	if (ID[0] != 'B' || ID[1] != 'M')
		return -1;

	/* reading header */
	if (bmp->fp) {
		size = fread(&bmpf->size, 1, 52, bmp->fp);
		if (size != 52)
			return -1;
	} else {
		memcpy(&bmpf->size, bmp->memory + 2, 52);
	}

	bmp->npalette = (bmpf->off_bits - 54) >> 2;

	/* dword boundary on line end */
	size = bmpi->width * bmpi->bit_count / 8;
	remnant = size % 4;

	if (remnant == 0)
		bmp->bytes_per_line = size;
	else
		bmp->bytes_per_line = size + (4 - remnant);

	return 0;
}

/**
 * bmp_readdata:
 */
static int bmp_readdata(bmp_t *bmp)
{
	bmpfileheader_t *bmpf = &bmp->bmpfile_h;
	bmpinfoheader_t *bmpi = &bmp->bmpinfo_h;

	if (!bmp->fp && !bmp->memory)
		return -1;

	if ((int)bmpf->off_bits < 0 || (int)bmpi->size_image < 0)
		return -1;

	if (bmpi->size_image < bmp->bytes_per_line * bmpi->height)
		return -1;

	bmpi->size_image = bmp->bytes_per_line * bmpi->height;

	if (bmpi->compression != BI_RGB && bmpi->compression != BI_BITFIELD) {
		fprintf(stderr, "not support compressioin BMP!\n");
		return -1;
	}

	bmp->data = (unsigned char *) malloc(bmpi->size_image);

	if (bmp->fp)
		fread(bmp->data, 1, bmpi->size_image, bmp->fp);
	else
		memcpy(bmp->data,
			bmp->memory + bmpf->off_bits, bmpi->size_image);

	return 0;
}

/**
 * bmp_readpalette:
 */
static int bmp_readpalette(bmp_t *bmp)
{
	if (!bmp->fp && !bmp->memory)
		return -1;

	if (bmp->fp) {
		/* alread fp point where palette is */
		int size = fread(bmp->palette,
				sizeof(struct palette_s),
				bmp->npalette, bmp->fp);
		if (size != bmp->npalette)
			return -1;
	} else {
		memcpy(bmp->palette, bmp->memory + 54,
			sizeof(struct palette_s) * bmp->npalette);
	}

	return 0;
}

/**
 * calculate_boffset:
 */
static void calculate_boffset(bmp_t *bmp)
{
	int i;
	unsigned *mask = (unsigned *)(bmp->palette);
	unsigned temp;

	/* red */
	temp = mask[0];
	for (i = 0; i < 32; i++) {
		if (temp & 0x01)
			break;
		temp >>= 1;
	}
	bmp->boffset_red = i;

	for (i = 0; i < 32; i++) {
		if (temp & 0x800000UL)
			break;
		temp <<= 1;
	}
	bmp->bsize_red = 32-i;

	/* green */
	temp = mask[1];
	for (i = 0; i < 32; i++) {
		if (temp & 0x01)
			break;
		temp >>= 1;
	}
	bmp->boffset_green = i;

	for (i = 0; i < 32; i++) {
		if (temp & 0x800000UL)
			break;
		temp <<= 1;
	}
	bmp->bsize_green = 32-i;

	/* blue */
	temp = mask[2];
	for (i = 0; i < 32; i++) {
		if (temp & 0x01)
			break;
		temp >>= 1;
	}
	bmp->boffset_blue = i;

	for (i = 0; i < 32; i++) {
		if (temp & 0x800000UL)
			break;
		temp <<= 1;
	}
	bmp->bsize_blue = 32-i;
}

/**
 * bmp_width:
 */
int bmp_width(bmp_t *bmp)
{
	return (int)(!bmp ? 0 : bmp->bmpinfo_h.width);
}

/**
 * bmp_height:
 */
int bmp_height(bmp_t *bmp)
{
	return (int)(!bmp ? 0 : bmp->bmpinfo_h.height);
}

/**
 * bmp_getpixel:
 */
rgbpixel_t bmp_getpixel(bmp_t *bmp, int x, int y)
{
	return bmp->getpixel(bmp, x, y);
}

/**
 * bmp_load:
 */
bmp_t *bmp_load(const char *bmpfile, void *memory)
{
	FILE *fp = NULL;
	bmp_t *bmp = NULL;
	bmpinfoheader_t *bmpi;

	if (!bmpfile && !memory)
		return NULL;

	if (bmpfile) {
		fp = fopen(bmpfile, "r");
		if (!fp) {
			fprintf(stderr,
				"Failed fopen %s\n", bmpfile);
			return NULL;
		}
	}

	bmp = (bmp_t *)malloc(sizeof(bmp_t));
	if (!bmp) {
		fprintf(stderr, "Failed bmp malloc %d\n", (int)sizeof(bmp_t));
		goto cleanup_fp;
	}
	memset(bmp, 0, sizeof(*bmp));

	bmp->fp = fp;
	bmp->memory = (char *)memory;
	bmpi = &bmp->bmpinfo_h;

	if (bmp_readheader(bmp))
		goto cleanup_bmp;

	if (bmp->npalette) {
		bmp->palette = (struct palette_s *)
			malloc(sizeof(struct palette_s) * bmp->npalette);
		memset(bmp->palette, 0,
			sizeof(struct palette_s) * bmp->npalette);

		if (bmp_readpalette(bmp))
			goto cleanup_bmp;
	}

	if (bmp_readdata(bmp))
		goto cleanup_bmp;

	switch (bmpi->bit_count) {
	case 1:
		bmp->getpixel = getpixel_1bpp;
		break;
	case 4:
		bmp->getpixel = getpixel_4bpp;
		break;
	case 8:
		bmp->getpixel = getpixel_8bpp;
		break;
	case 16:
		bmp->getpixel = getpixel_16bpp;
		if (bmpi->compression == BI_RGB) {
			unsigned *mask;

			if (bmp->palette != NULL) /* something wrong */
				goto cleanup_bmp;

			mask = (unsigned *)malloc(sizeof(unsigned)*3);
			mask[2] = 0x001F; /* blue mask */
			mask[1] = 0x03E0; /* green mask */
			mask[0] = 0x7C00; /* red mask */
			bmp->palette = (struct palette_s *)mask;
			bmp->boffset_blue = 0;
			bmp->boffset_green = 5;
			bmp->boffset_red = 10;
			bmp->bsize_blue = 5;
			bmp->bsize_green = 5;
			bmp->bsize_red = 5;
		/* BI_BITFIELD */
		} else {
			if (bmp->palette == NULL) /* something wrong */
				goto cleanup_bmp;
			calculate_boffset(bmp);
		}
		break;
	case 24:
		bmp->getpixel = getpixel_24bpp;
		break;
	case 32:
		bmp->getpixel = getpixel_32bpp;
		if (bmpi->compression == BI_RGB) {
			unsigned *mask;

			if (bmp->palette != NULL) /* something wrong */
				goto cleanup_bmp;

			mask = (unsigned *)malloc(sizeof(unsigned)*3);
			mask[2] = 0x000000FF; /* blue mask */
			mask[1] = 0x0000FF00; /* green mask */
			mask[0] = 0x00FF0000; /* red mask */
			bmp->palette = (struct palette_s *)mask;
			bmp->boffset_blue = 0;
			bmp->boffset_green = 8;
			bmp->boffset_red = 16;
			bmp->bsize_blue = 8;
			bmp->bsize_green = 8;
			bmp->bsize_red = 8;
		/* BI_BITFILED */
		} else {
			if (bmp->palette == NULL) /* something wrong */
				goto cleanup_bmp;

			calculate_boffset(bmp);
		}
		break;
	default:
		goto cleanup_bmp;
	}

	fclose(fp);

	return bmp;

cleanup_bmp:
	if (bmp->data)
		free(bmp->data);

	if (bmp->palette)
		free(bmp->palette);

	free(bmp);

cleanup_fp:
	fclose(fp);

	return NULL;
}

/**
 * bmp_close:
 */
void bmp_release(bmp_t *bmp)
{
	if (!bmp)
		return;

	if (bmp->palette)
		free(bmp->palette);

	if (bmp->data)
		free(bmp->data);

	free(bmp);
}


