#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <sys/stat.h>

#include "png.h"

#define	PNGSIGSIZE	8

#ifdef SUPPORT_GAMMA
/**
 * png_screen_gamma:
 *  Get screen gamma value one of three ways.
 */
static double png_screen_gamma(png_t *png)
{
	char *gamma_str;

	if (png->screen_gamma != -1)
		return png->screen_gamma;

	/* Use the environment variable if available.
	 * 2.2 is a good guess for PC monitors.
	 * 1.1 is good for my laptop.
	 */
	gamma_str = getenv("SCREEN_GAMMA");

	return (gamma_str) ? atof(gamma_str) : 2.2;
}
#endif

/**
 * png_read_fn:
 */
static void png_read_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_t *png = (png_t *)png_get_io_ptr(png_ptr);

	memcpy(data, png->memory + png->offset, length);
	png->offset += length;
}

/**
 * png_load:
 */
png_t *png_load(const char *filename, unsigned char *memory)
{
	FILE *fp = NULL;
	png_t *png = NULL;
	const size_t png_sig_size = PNGSIGSIZE;
	unsigned char png_sig[PNGSIGSIZE] = { 0, };
	char *buf = NULL;
	int rowbytes;
	int i, ret = 0;
#ifdef SUPPORT_GAMMA
	double image_gamma, screen_gamma;
	int intent;
#endif
	assert(filename || memory);

	if (filename) {
		fp = fopen(filename, "r");
		if (!fp) {
			fprintf(stderr,
				"Failed fopen %s\n", filename);
			return NULL;
		}
	}

	png = (png_t *)malloc(sizeof(png_t));
	if (!png) {
		fprintf(stderr, "Failed png malloc %d\n", (int)sizeof(png_t));
		goto cleanup_fp;
	}
	memset(png, 0, sizeof(png_t));

	png->offset = png_sig_size;
	png->memory = memory;
	png->screen_gamma = -1;
	png->compression_level = Z_BEST_COMPRESSION;

	/* read from file or memory */
	if (fp)
		fread(png_sig, 1, png_sig_size, fp);
	else
		memcpy(png_sig, memory, png_sig_size);

	ret = png_sig_cmp(png_sig, 0, png_sig_size);
	if (ret) {
		fprintf(stderr, "not png %s\n",
			filename ? filename : "memory");
		goto cleanup_fp;
	}

	/*
	 * Create and initialize the png_struct with the desired error handler
	 * functions.
	 * If you want to use the default stderr and longjump method,
	 * you can supply NULL for the last three parameters.
	 * We also supply the
	 * the compiler header file version, so that we know if the application
	 * was compiled with a compatible version of the library.
	 */
	png->png = png_create_read_struct(
			PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png->png) {
		fprintf(stderr,
			"create libpng read structure %s\n",
			filename ? filename : "memory");
		ret = -1;
		goto cleanup_fp;
	}

	/* Allocate/initialize the memory for image information. */
	png->info = png_create_info_struct(png->png);
	if (!png->info) {
		fprintf(stderr,
			"create libpng info structure %s\n",
			filename ? filename : "memory");
		ret = -1;
		goto cleanup_read;
	}

	/*
	 * Set error handling if you are using the setjmp/longjmp method
	 * (this is the normal method of doing things with libpng).
	 * REQUIRED unless you set up your own error handlers in the
	 * png_create_read_struct() earlier.
	 */
	if (setjmp(png_jmpbuf(png->png))) {
		fprintf(stderr, "initializing libpng's I/O subsystem\n");
		ret = -1;
		goto cleanup_info;
	}

	/* set png load fun */
	if (fp)
		png_init_io(png->png, fp);
	else
		png_set_read_fn(png->png, png, png_read_fn);

	/* We have already read some of the signature. */
	png_set_sig_bytes(png->png, png_sig_size);

	/* The call to png_read_info() gives us all of the information from the
	 * PNG file before the first IDAT (image data chunk).
	 */
	png_read_info(png->png, png->info);

	png_get_IHDR(png->png, png->info,
		&png->width, &png->height, &png->bit_depth,
		&png->color_type, &png->interlace_type, NULL, NULL);

	/* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
	 * byte into separate bytes (useful for paletted and grayscale images).
	 */
	png_set_packing(png->png);

	/*
	 * Expand grayscale images to the full 8 bits
	 * from 1, 2, or 4 bits/pixel
	 */
	if ((png->color_type == PNG_COLOR_TYPE_GRAY) && (png->bit_depth < 8))
		png_set_expand(png->png);

	/*
	 * Adds a full alpha channel if there is transparency information
	 * in a tRNS chunk.
	 */
	if (png_get_valid(png->png, png->info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png->png);

	/* Convert 16-bit to 8-bit. */
	if (png->bit_depth == 16)
		png_set_strip_16(png->png);

	switch (png->color_type) {
	/* Convert grayscale to RGB triplets */
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		png_set_gray_to_rgb(png->png);
		break;

	/* Convert palette images to RGB */
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(png->png);
		break;
	default:
		break;
	}

	#ifdef SUPPORT_GAMMA
	/* Tell libpng to handle the gamma conversion for us. */
	screen_gamma = png_screen_gamma(png);

	if (png_get_sRGB(png->png, png->info, &intent)) {
		png_set_sRGB(png->png, png->info, intent);
	} else {
	    if (png_get_gAMA(png->png, png->info, &image_gamma))
		png_set_gamma(png->png, screen_gamma, image_gamma);
	    else
		png_set_gamma(png->png, screen_gamma, 0.50);
	}
	#endif

	/*
	 * update info structure.
	 */
	png_read_update_info(png->png, png->info);

	/* get rowbytes after read update info */
	rowbytes = png_get_rowbytes(png->png, png->info);

#ifdef DEBUG
	fprintf(stdout, "PNG: %dx%d, depth: %d color: %s row %d\n",
		png->width, png->height, png->bit_depth,
		png->color_type == PNG_COLOR_TYPE_PALETTE ? "PALETTE" :
		png->color_type == PNG_COLOR_TYPE_RGB ? "RGB" :
		png->color_type == PNG_COLOR_TYPE_RGBA ? "RGBA" :
		png->color_type == PNG_COLOR_TYPE_GRAY ? "GRAY" :
		png->color_type == PNG_COLOR_TYPE_GA ? "GRAYA" :
		"unknown",
		rowbytes);
#endif

	if (setjmp(png_jmpbuf(png->png))) {
		fprintf(stderr, "Failed while reading png image %s\n",
			filename ? filename : "memory");
		ret = -1;
		goto cleanup_info;
	}

	png->rows = (png_byte **)calloc(png->height, sizeof(png_byte *));
	if (!png->rows) {
		fprintf(stderr, "Failed png rows calloc %d - %d\n",
			png->height, (int)sizeof(png_byte *));
		ret = -1;
		goto cleanup_info;
	}

	buf = calloc(png->height, rowbytes);
	if (!buf) {
		fprintf(stderr, "Failed png row buf malloc %d\n",
			png->height * rowbytes);
		ret = -1;
		goto cleanup_info;
	}

	for (i = 0; i < png->height; i++)
		png->rows[i] = (png_byte *)(buf + (i * rowbytes));

	png_read_image(png->png, png->rows);
	png_read_end(png->png, NULL);

cleanup_info:
	png_destroy_info_struct(png->png, &png->info);

cleanup_read:
	/* Clean up after the read, and free any memory allocated. */
	png_destroy_read_struct(&png->png, NULL, NULL);

cleanup_fp:
	if (fp)
		fclose(fp);

	if (ret) {
		free(buf);
		free(png->rows);
		free(png);
	}

	return ret ? NULL : png;
}

/**
 * png_release:
 */
void png_release(png_t *png)
{
	if (!png)
		return;

	/* calloc free */
	free(png->rows[0]);
	free(png->rows);
	free(png);
}
