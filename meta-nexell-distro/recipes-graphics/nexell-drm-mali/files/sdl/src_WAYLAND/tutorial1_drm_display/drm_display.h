//------------------------------------------------------------------------------
//
//	Copyright (C) 2017 NEXELL Co., Ltd All Rights Reserved
//	NEXELL Co. Proprietary & Confidential
//
//	Module     : test00_sample
//	File       : test00_sample.cpp
//	Description: Sample test
//	Author     : hyunjh(hyunjh@nexell.co.kr)
//	Export     :
//	History    :
//     2017-10-02 hyunjh	1st release
//------------------------------------------------------------------------------

#ifndef __NX_AVN_DEVICE_DRM_DISPLAY_H__
#define __NX_AVN_DEVICE_DRM_DISPLAY_H__

//------------------------------------------------------------------------------
//
//    Includes
//    
//------------------------------------------------------------------------------
#include <base_interface.h>


//------------------------------------------------------------------------------
//
//    Defines
//    
//------------------------------------------------------------------------------
#define FB_BUFFER_COUNT_MAX	2

typedef struct tagDrmDispWindow{
	int id;
	int dma_fd;	
}DrmDispWindow;
typedef DrmDispWindow* HWIN_DRM;

typedef struct tagDrmDispCtrl{
	void*			hsema;
	unsigned int	pop_buf_idx;
	unsigned int	push_buf_idx;
	void*			parray_window;
}DrmDispCtrl;



//------------------------------------------------------------------------------
//
//    Functions
//    
//------------------------------------------------------------------------------
int test_drm_disp_main(void);
int* test_drm_init(unsigned int width, unsigned int height);
int test_drm_run(unsigned int fb_idx);
int test_drm_deinit(void);
#ifdef VR_PLATFORM_DST_PIXMAP_CONNECT_TO_DISPLAY
VR_BOOL drm_platform_window_init(DrmDispCtrl* pdrm_disp_ctrl, int* dam_fds);
void drm_platform_window_deinit(DrmDispCtrl* pdrm_disp_ctrl);
HWIN_DRM drm_platform_window_pop(DrmDispCtrl* pdrm_disp_ctrl);
void drm_platform_window_push(DrmDispCtrl* pdrm_disp_ctrl);
#endif

#endif
