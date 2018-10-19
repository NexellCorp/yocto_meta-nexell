#ifndef _FB_H_
#define _FB_H_

#include "bmp.h"
#include "png.h"

#include <linux/fb.h>

/**
 * fb_t:
 */
typedef struct {
	int fd;
	void *pixel;
	const char *device_name;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	int refresh;
	int pans;
} fb_t;

fb_t *fb_initialize(const char *devname);
void fb_release(fb_t *fb);

void fb_flip(fb_t *fb, int nr);
void fb_bmp_draw(fb_t *fb, bmp_t *bmp, int pan);
void fb_png_draw(fb_t *fb, png_t *png, int pan);

#endif
