#ifndef _PNG_H_
#define _PNG_H_

#include <png.h>

/**
 * png_t:
 */
typedef struct {
	png_struct *png;
	png_info *info;
	png_uint_32 width;
	png_uint_32 height;
	int color_type;
	int bit_depth;
	int interlace_type;
	png_byte **rows;
	unsigned char *memory;
	unsigned long offset;
	double screen_gamma;
	int compression_level;
} png_t;

png_t *png_load(const char *filename, unsigned char *memory);
void png_release(png_t *png);

#endif
