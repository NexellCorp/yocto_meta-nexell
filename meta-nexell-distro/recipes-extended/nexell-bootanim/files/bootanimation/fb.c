#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "fb.h"

/**
 * fb_initialize:
 */
fb_t *fb_initialize(const char *devname)
{
	fb_t *fb;
	int file;
	int hto, vto, hz, pixclk;

	assert(devname);

	file = open(devname, O_RDWR);
	if (file < 0) {
		fprintf(stderr,
			"Unable to open framebuffer %s\n",
			devname);
		return NULL;
	}

	fb = (fb_t *)malloc(sizeof(fb_t));
	if (!fb) {
		fprintf(stderr,
			"Failed fb malloc %d\n", (int)sizeof(fb_t));
		goto cleanup_fb;
	}

	fb->fd = file;
	fb->pixel = NULL;
	fb->device_name = devname;

	memset(&fb->finfo, 0, sizeof(fb->finfo));
	memset(&fb->vinfo, 0, sizeof(fb->vinfo));

	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->finfo)) {
		fprintf(stderr, "Unable to read fixed screen information\n");
		goto cleanup_fb;
	}

	if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vinfo)) {
		fprintf(stderr, "Unable to read variable screen information\n");
		goto cleanup_fb;
	}

	fb->pixel = (uint8_t *)mmap(0, fb->finfo.smem_len,
			PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);

	if ((intptr_t) fb->pixel == -1) {
		fprintf(stderr, "Failed to map framebuffer in to memory\n");
		goto cleanup_fb;
	}

	hto = fb->vinfo.left_margin + fb->vinfo.right_margin;
	vto = fb->vinfo.upper_margin + fb->vinfo.lower_margin;
	pixclk = fb->vinfo.pixclock;
	hz = pixclk / (hto * vto);

	fb->pans = fb->vinfo.yres_virtual/fb->vinfo.yres;
	fb->refresh = hz;

#ifdef DEBUG
	fprintf(stdout,
		"FB : %s, %p(0x%lx),%d (%d * %d : %d * %d, %d bpp) pans:%d\n",
		fb->device_name, fb->pixel, fb->finfo.smem_start,
		fb->finfo.smem_len, fb->vinfo.xres_virtual, fb->vinfo.yres,
		fb->vinfo.xres_virtual, fb->vinfo.yres_virtual,
		fb->vinfo.bits_per_pixel, fb->pans);
	fprintf(stdout,
		"FB : %d HZ %d (%d:%d) - %d (%d:%d) - %d hz\n",
		hz, hto, fb->vinfo.left_margin, fb->vinfo.right_margin,
		vto, fb->vinfo.upper_margin, fb->vinfo.lower_margin,
		pixclk);
#endif

	return fb;

cleanup_fb:

	close(fb->fd);
	return NULL;
}

/**
 * fb_release:
 */
void fb_release(fb_t *fb)
{
	if (!fb)
		return;

	/* restore */
	fb_flip(fb, 0);
	munmap(fb->pixel, fb->finfo.smem_len);
	close(fb->fd);
}

/**
 * fb_flip:
 */
void fb_flip(fb_t *fb, int nr)
{
	int avail;

	assert(fb);

	avail = fb->vinfo.yres_virtual / fb->vinfo.yres;
//	if (avail == 1)
//		return;

	if (nr > avail) {
		fprintf(stderr,
			"pan buffer %d is over frame buffers %d\n",
			nr, avail);
		return;
	}

	/* set buffer */
	fb->vinfo.yoffset = (fb->vinfo.yres * nr);

	if (ioctl(fb->fd, FBIOPAN_DISPLAY, &fb->vinfo))
		fprintf(stderr, "Unable to pan %d display\n", nr);
}

/**
 * fb_bmp_draw:
 * support FB : 24bpp/32bpp
 */
void fb_bmp_draw(fb_t *fb, bmp_t *bmp, int pan)
{
	char *pixel;
	int j, i, n, m;
	int pixel_byte;
	int xres, yres, width, height;
	int x = 0, y = 0; /* screen */
	int sx, sy, ex, ey; /* bmp */

	pixel_byte = fb->vinfo.bits_per_pixel / 8;
	xres = fb->vinfo.xres_virtual;
	yres = fb->vinfo.yres;
	pixel = fb->pixel + (xres * yres * pixel_byte * pan);

	if (pixel_byte != 4 && pixel_byte != 3) {
		fprintf(stderr, "not support %dbpp (24,32 bpp)\n",
			pixel_byte * 8);
		return;
	}

	width = bmp_width(bmp);
	height = bmp_height(bmp);

	sx = 0, sy = 0;
	ex = width, ey = height;

	if (width  > xres) {
		sx = (width - xres) >> 1;
		ex = sx + xres;
	} else if (width  < xres) {
		x += (xres - width) >> 1;
		/* clear framebuffer */
		memset(pixel, 0, xres * yres * pixel_byte);
	}

	if (height > yres) {
		sy = (height - yres) >> 1;
		ey = sy + yres;
	} else if (height < yres) {
		y += (yres - height) >> 1;
		/* clear framebuffer */
		memset(pixel, 0, xres * yres * pixel_byte);
	}

	for (i = y, n = sy; n < ey; i++, n++) {
		for (j = x, m = sx; m < ex; j++, m++) {
			rgbpixel_t p = bmp_getpixel(bmp, m, n);
			char *t = pixel + (i * xres + j) * pixel_byte;

			t[2] = p.r;
			t[1] = p.g;
			t[0] = p.b;
		}
	}
}

/**
 * fb_png_draw:
 * support FB : 24bpp/32bpp
 */
void fb_png_draw(fb_t *fb, png_t *png, int pan)
{
	char *pixel;
	int j, i, n, m;
	int pixel_byte;
	int xres, yres, width, height;
	int x = 0, y = 0; /* screen */
	int sx, sy, ex, ey; /* bmp */

	if (!fb)
		return;

	pixel_byte = fb->vinfo.bits_per_pixel / 8;
	xres = fb->vinfo.xres_virtual;
	yres = fb->vinfo.yres;
	pixel = fb->pixel + (xres * yres * pixel_byte * pan);

	if (!png || !png->rows)
		return;

	if (pixel_byte != 4 && pixel_byte != 3) {
		fprintf(stderr, "not support %dbpp (24,32 bpp)\n",
			pixel_byte * 8);
		return;
	}

	width = png->width;
	height = png->height;

	sx = 0, sy = 0;
	ex = width, ey = height;

	if (width  > xres) {
		sx = (width - xres) >> 1;
		ex = sx + xres;
	} else if (width  < xres) {
		x += (xres - width) >> 1;
		/* clear framebuffer */
		memset(pixel, 0, xres * yres * pixel_byte);
	}

	if (height > yres) {
		sy = (height - yres) >> 1;
		ey = sy + yres;
	} else if (height < yres) {
		y += (yres - height) >> 1;
		/* clear framebuffer */
		memset(pixel, 0, xres * yres * pixel_byte);
	}

	for (i = y, n = sy; n < ey; i++, n++) {
		png_byte *row = png->rows[n];

		for (j = x, m = sx; m < ex; j++, m++) {
			png_byte *r = row + (m * 3);
			char *t = pixel + (i * xres + j) * pixel_byte;

			t[2] = r[0];
			t[1] = r[1];
			t[0] = r[2];
		}
	}
}
