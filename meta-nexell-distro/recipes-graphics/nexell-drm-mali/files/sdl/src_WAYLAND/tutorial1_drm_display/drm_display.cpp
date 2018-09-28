#include <nx-drm-allocator.h>
#include "drm_display.h"
//#include "../../common.h"

#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>

#include <drm_fourcc.h>
#include "xf86drm.h"
#include "dp.h"
#include "dp_common.h"
//#include "format.h"

#include <linux/videodev2.h>
//#include "media-bus-format.h"
//#include "nexell_drmif.h"
#include "nx-drm-allocator.h"
#include "drm_display.h"
//#include "option.h"

//#define DP_DGB_EN

#if !defined( DP_DGB_EN )
#ifdef DP_DBG
#undef DP_DBG
#define DP_DBG
#undef DP_LOG
#define DP_LOG
#endif
#endif

static const uint32_t dp_formats[] = {

	/* 1 buffer */
	DRM_FORMAT_YUYV,
	DRM_FORMAT_YVYU,
	DRM_FORMAT_UYVY,
	DRM_FORMAT_VYUY,

	/* 2 buffer */
	DRM_FORMAT_NV12,	/* NV12 : 2x2 subsampled Cr:Cb plane */
	DRM_FORMAT_NV21,	/* NV21 : 2x2 subsampled Cb:Cr plane */
	DRM_FORMAT_NV16,	/* NV16 : 2x1 subsampled Cr:Cb plane */
	DRM_FORMAT_NV61,	/* NV61 : 2x1 subsampled Cb:Cr plane */
	//DRM_FORMAT_NV24,	/* NV24 : non-subsampled Cr:Cb plane */
	//DRM_FORMAT_NV42,	/* NV42 : non-subsampled Cb:Cr plane */

	/* 3 buffer */
#if 0
	DRM_FORMAT_YUV410, 	/* YUV9 : 4x4 subsampled Cb (1) and Cr (2) planes */
	DRM_FORMAT_YVU410,	/* YVU9 : 4x4 subsampled Cr (1) and Cb (2) planes */
	DRM_FORMAT_YUV411,	/* YU11 : 4x1 subsampled Cb (1) and Cr (2) planes */
	DRM_FORMAT_YVU411,	/* YV11 : 4x1 subsampled Cr (1) and Cb (2) planes */
#endif
	DRM_FORMAT_YUV420,	/* YU12 : 2x2 subsampled Cb (1) and Cr (2) planes */
	DRM_FORMAT_YVU420,	/* YV12 : 2x2 subsampled Cr (1) and Cb (2) planes */
	DRM_FORMAT_YUV422,	/* YU16 : 2x1 subsampled Cb (1) and Cr (2) planes */
	DRM_FORMAT_YVU422,	/* YV16 : 2x1 subsampled Cr (1) and Cb (2) planes */
	DRM_FORMAT_YUV444,	/* YU24 : non-subsampled Cb (1) and Cr (2) planes */
	DRM_FORMAT_YVU444,	/* YV24 : non-subsampled Cr (1) and Cb (2) planes */

	/* RGB 1 buffer */
	DRM_FORMAT_RGB565,
	DRM_FORMAT_BGR565,
	DRM_FORMAT_RGB888,
	DRM_FORMAT_BGR888,
	DRM_FORMAT_ARGB8888,
	DRM_FORMAT_ABGR8888,
	DRM_FORMAT_XRGB8888,
	DRM_FORMAT_XBGR8888,
};
static struct dp_framebuffer *g_fbs[FB_BUFFER_COUNT_MAX] = {NULL,};
static uint32_t g_disp_width, g_disp_height;
static int g_gem_fds[FB_BUFFER_COUNT_MAX] = {-1, -1};
static int g_dma_fds[FB_BUFFER_COUNT_MAX] = {-1, -1};



int g_drm_fd = -1;
static struct dp_device *g_drm_device;

int platform_vmem_open_drm(void)
{
	int drm_fd = open_drm_device();	
	if (drm_fd < 0) {
		DP_ERR("failed to open drm device\n");
		return -1;
	}
	DP_DBG("drm_fd(0x%x)\n", drm_fd);
	return drm_fd;
}

VR_BOOL platform_system_init_display_device(void)
{
	int err;

	g_drm_fd = platform_vmem_open_drm();
	if (g_drm_fd < 0) {
		DP_ERR("failed to open_drm_device\n");
		return VR_FALSE;
	}

	#if 1
	err = drmSetClientCap(g_drm_fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
	if (err < 0) {
		DP_ERR("drmSetClientCap() failed: %d %m\n", err);
		return VR_FALSE;
	}
	#endif

	g_drm_device = dp_device_open(g_drm_fd);
	if (!g_drm_device) {
		DP_ERR("fail : device open() %m\n");
		return VR_FALSE;
	}
	return VR_TRUE;
}

void platform_system_deinit_display_device(void)
{
	dp_device_close((struct dp_device *)g_drm_device);
	g_drm_device = NULL;
	drmClose(g_drm_fd);
	g_drm_fd = 0;
}

int platform_system_get_display_fd(void)
{
	assert(g_drm_fd);
	return g_drm_fd;
}

void* platform_system_get_display_device(void)
{
	assert(g_drm_device);
	return g_drm_device;
}



static uint32_t choose_format(struct dp_plane *plane, uint32_t format)
{
	if (!dp_plane_supports_format(plane, format)) {
		DP_ERR("fail : not support %s\n", dp_forcc_name(format));
		return -EINVAL;
	}

	DP_LOG("format: %s\n", dp_forcc_name(format));
	return format;
}

int dp_plane_update(struct dp_device *device, struct dp_framebuffer *fb,
		    uint32_t screen, uint32_t w, uint32_t h)
{
#if 0 //avn board
	int d_idx = 0/*overlay*/, p_idx = 0/*plane index -rgb*/, err;
#else
	int d_idx = 0/*overlay*/, p_idx = 1/*plane index -rgb*/, err;
#endif
	struct dp_plane *plane;

	plane = dp_device_find_plane_by_index(device,
					      d_idx, p_idx);
	if (!plane) {
		DP_ERR("no overlay plane found\n");
		return -EINVAL;
	}

	err = dp_plane_set(plane, fb, 0,0, w, h,0,0,w,h);
	if (err < 0) {
		DP_ERR("set plane failed \n");
		return -EINVAL;
	}

	return 1;
}

struct dp_framebuffer * dp_buffer_init(struct dp_device *device, int  x, int y,
		int fmt, int seperate)
{
	struct dp_framebuffer *fb = NULL;
#if 0 //avn board
	int d_idx = 0, p_idx = 0;
#else
	int d_idx = 0, p_idx = 2;
#endif
	struct dp_plane *plane;
	uint32_t format;
	int err;

	plane = dp_device_find_plane_by_index(device,
					      d_idx, p_idx);
	if (!plane) {
		DP_ERR("no overlay plane found\n");
		return NULL;
	}

	/*
	 * set plane format
	 */
	format = choose_format(plane, fmt);
	if (!format) {
		DP_ERR("fail : no matching format found\n");
		return NULL;
	}
	DP_DBG("format is %d\n",format);

	fb = dp_framebuffer_create(device, format, x, y, seperate);
	if (!fb) {
		DP_ERR("fail : framebuffer create Fail \n");
		return NULL;
	}

	DP_DBG("add fb2\n");
	err = dp_framebuffer_addfb2(fb);
	if (err < 0) {
		DP_ERR("fail : framebuffer add Fail \n");
		if (fb)
			dp_framebuffer_free(fb);
		return NULL;
	}
	DP_DBG("add fb2 finish\n");

	return fb;
}

VR_BOOL drm_dispaly_map_window(int drm_fd, struct dp_framebuffer **fbs, struct dp_device *device, unsigned int width, unsigned int height)
{
	int seperate = 0;
	int i;
	unsigned int f = DRM_FORMAT_XRGB8888;//DRM_FORMAT_RGB888;

	for (i = 0; i < FB_BUFFER_COUNT_MAX; i++) {
		struct dp_framebuffer *fb;
		fb = dp_buffer_init(device, width, height, f, seperate);
		if (!fb) {
			DP_ERR("fail : framebuffer Init %m\n");
			return VR_FALSE;
		}
		dp_framebuffer_map(fb);		
		g_gem_fds[i] = fb->handles[0];
		g_dma_fds[i] = gem_to_dmafd(drm_fd, g_gem_fds[i]);
		fbs[i] = fb;
		DP_DBG("size = %d\n", fbs[i]->sizes[0]);
	}
	DP_DBG("num_screens:%d\n", device->num_screens);

	return VR_TRUE;
}

void drm_dispaly_unmap_window(struct dp_framebuffer **fbs)
{
	int i;
	/* free buffers */
	for (i = 0; i < FB_BUFFER_COUNT_MAX; i++) {
		int gem_fd = fbs[i]->handles[0];
		if (fbs[i]) {
			dp_framebuffer_unmap(fbs[i]);
			dp_framebuffer_delfb2(fbs[i]);
		}		
		if (gem_fd >= 0)
			close(gem_fd);
		g_gem_fds[i] = g_dma_fds[i] = -1;			
		fbs[i] = NULL;		
	}
}

VR_BOOL drm_dispaly_swap(int drm_fd, struct dp_framebuffer *fb, struct dp_device *device, unsigned int width, unsigned int height)
{
	#if 0 //fill color
	int gem_fd = fb->handles[0];
	int dma_fd = gem_to_dmafd(drm_fd, gem_fd);
	DP_DBG("fd = %d, virtual : %p\n", dma_fd, fb->ptrs[0]);
	if (dma_fd < 0) {
		fprintf(stderr, "failed to gem_to_dmafd\n");
		return VR_FALSE;
	}
	
	#if 1
	for (int y =0; y < fb->height; y++) {
		for (int x = 0; x < fb->width; x++) {
			int c = (y	* (fb->width * 3)) + (x * 3);
			*((unsigned char*)(fb->ptrs[0]) + c + 0) = 0x00; /* B */
			*((unsigned char*)(fb->ptrs[0]) + c + 1) = 0xFF; /* G */
			*((unsigned char*)(fb->ptrs[0]) + c + 2) = 0x00; /* R */
		}
	}
	#else
	memset(fb->ptrs[0], 0x0A/*red:0xf00*/, fb->sizes[0]);
	#endif
	#endif
	
	dp_plane_update(device, fb, 0, width, height);
	usleep(30000);
	return VR_TRUE;
}

int* test_drm_init(unsigned int width, unsigned int height)
{
	platform_system_init_display_device();

	int drm_fd = platform_system_get_display_fd();
	g_disp_width = width;
	g_disp_height = height;

	#ifdef DP_DGB_EN
	dp_debug_on(VR_TRUE);
	#else
	dp_debug_on(VR_FALSE);
	#endif
	
	drm_fd = platform_system_get_display_fd();
	struct dp_device *device = (struct dp_device *)platform_system_get_display_device();
	if (!device) {
		DP_ERR("fail : device open() %m\n");
		return NULL;
	}

	drm_dispaly_map_window(drm_fd, g_fbs, device, width, height);
	DP_DBG("num_screens:%d\n", device->num_screens);	

	return g_dma_fds;
}

int test_drm_run(unsigned int fb_idx)
{
	fb_idx = fb_idx % FB_BUFFER_COUNT_MAX;
	int drm_fd = platform_system_get_display_fd();
	struct dp_device *device = (struct dp_device *)platform_system_get_display_device();
	drm_dispaly_swap(drm_fd, g_fbs[fb_idx], device, g_disp_width, g_disp_height);
	return 0;
}

int test_drm_deinit(void)
{
	drm_dispaly_unmap_window(g_fbs);	
	platform_system_deinit_display_device();
	return 0;
}

#ifdef VR_PLATFORM_DST_PIXMAP_CONNECT_TO_DISPLAY
VR_BOOL drm_platform_window_init(DrmDispCtrl* pdrm_disp_ctrl, int* dam_fds)
{
	pdrm_disp_ctrl->pop_buf_idx = 0;
	pdrm_disp_ctrl->push_buf_idx = 0;
	pdrm_disp_ctrl->hsema = base_runtime_sync_create_sema(FB_BUFFER_COUNT_MAX, FB_BUFFER_COUNT_MAX);			
	if (!pdrm_disp_ctrl->hsema)
	{
		DP_ERR("Can't create sema\n");
		return VR_FALSE;
	}	

	pdrm_disp_ctrl->parray_window = malloc(sizeof(int) * FB_BUFFER_COUNT_MAX);
	if (!pdrm_disp_ctrl->parray_window)
	{
		DP_ERR("Can't alloc\n");
		return VR_FALSE;
	}	
	for (int i = 0 ; i < FB_BUFFER_COUNT_MAX ; i++)
	{
		((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[i] = (DrmDispWindow*)calloc(1, sizeof(DrmDispWindow));
		if (!((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[i])
		{
			DP_ERR("Can't alloc\n");
			return VR_FALSE;
		}	
		(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[i])->id = i;
		(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[i])->dma_fd = dam_fds[i];
	}
	
	//NxDbgMsg("temp sema down...\n"); //temp test
	//base_runtime_sync_down_sema((HSEMA)pdrm_disp_ctrl->hsema);
	//NxDbgMsg("temp sema down done\n"); //temp test
	
	for (int i = 0 ; i < FB_BUFFER_COUNT_MAX ; i++)
	{
		base_runtime_sync_down_sema((HSEMA)pdrm_disp_ctrl->hsema);
	}
	NxDbgMsg("[drm_disp] init done. (sema down done)\n"); 
	
	return VR_TRUE;
}

void drm_platform_window_deinit(DrmDispCtrl* pdrm_disp_ctrl)
{
	#if 0
	//wait until all windows are pushed
	NxDbgMsg("[drm_disp] wait sema down...\n");
	for (int i = 0 ; i < FB_BUFFER_COUNT_MAX ; i++)
	{
		base_runtime_sync_down_sema((HSEMA)pdrm_disp_ctrl->hsema);
	}
	NxDbgMsg("[drm_disp] release all sema\n");
	#endif

	//release all sema
	for (int i = 0 ; i < FB_BUFFER_COUNT_MAX ; i++)
	{
		base_runtime_sync_up_sema((HSEMA)pdrm_disp_ctrl->hsema);
	}
	base_runtime_sync_destroy_sema((HSEMA)pdrm_disp_ctrl->hsema);
	pdrm_disp_ctrl->hsema = NULL;	

	//destroy
	for (int i = 0 ; i < FB_BUFFER_COUNT_MAX ; i++)
	{
		free(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[i]);
	}
	free(pdrm_disp_ctrl->parray_window);
	pdrm_disp_ctrl->parray_window = NULL;
	
	pdrm_disp_ctrl->pop_buf_idx = 0;
	pdrm_disp_ctrl->push_buf_idx = 0;
	NxDbgMsg("[drm_disp] deinit done.\n");
}

//App pop from EGL 
HWIN_DRM drm_platform_window_pop(DrmDispCtrl* pdrm_disp_ctrl)
{
	int curr_buf_idx;
	HWIN_DRM hwin_drm;

	if (pdrm_disp_ctrl->hsema)
	{	
		//NxDbgMsg("[drm_disp] wait sema down...\n");
		base_runtime_sync_down_sema((HSEMA)pdrm_disp_ctrl->hsema);
		//NxDbgMsg("[drm_disp] wait done\n");
	} 

	curr_buf_idx = pdrm_disp_ctrl->pop_buf_idx;
	pdrm_disp_ctrl->pop_buf_idx = (pdrm_disp_ctrl->pop_buf_idx+1) % FB_BUFFER_COUNT_MAX;		

	hwin_drm = (HWIN_DRM)(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[curr_buf_idx]);	
	//(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[curr_buf_idx]) = NULL;

	assert(hwin_drm);
	
	return hwin_drm;
}

/*
	EGL push to APP.
	register EGL_swap_done_callback by EGL api.
	Called Host side API by callback.
*/	
void drm_platform_window_push(DrmDispCtrl* pdrm_disp_ctrl)
{	
	//assert(!(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[pdrm_disp_ctrl->push_buf_idx]));
	//(((DrmDispWindow**)(pdrm_disp_ctrl->parray_window))[pdrm_disp_ctrl->push_buf_idx]) = hwin_drm;
	pdrm_disp_ctrl->push_buf_idx = (pdrm_disp_ctrl->push_buf_idx+1) % FB_BUFFER_COUNT_MAX;
	
	if (pdrm_disp_ctrl->hsema)
	{
		base_runtime_sync_up_sema((HSEMA)pdrm_disp_ctrl->hsema);
	}
}
#endif


