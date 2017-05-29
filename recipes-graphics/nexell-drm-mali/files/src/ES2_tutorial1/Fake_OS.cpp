//------------------------------------------------------------------------------
//
//	Copyright (C) 2003 MagicEyes Digital Co., Ltd All Rights Reserved
//	MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE AND
//	WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//
//	Module     :
//	File       : Fake_OS.cpp
//	Description:
//	Author     : Yuni(yuni@mesdigital.com)
//	Export     :
//	History    :
//	   2007/01/22 Gamza MP2530F port
//	   2007/01/08 Yuni  first implementation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include <string.h>
#if defined(UNDER_CE)
	#error "todo...WinCE"
#elif defined(linux)
	#include <stdlib.h> /* for exit */
	#include <stdio.h> /* for exit */
	#include <unistd.h> /* for open/close .. */
	#include <fcntl.h> /* for O_RDWR */
	#include <sys/ioctl.h> /* for ioctl */
	#include <linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
	#include <sys/time.h> // for timeval, gettimeofday
#else
	#include <windows.h>
	//#error "unknown platform"
#endif
#include "Fake_OS.h"

//------------------------------------------------------------------------------
//  export
//------------------------------------------------------------------------------
void 	OS_InitFakeOS( void )
{
    //ARMStartup_InitBoard();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Window system
//------------------------------------------------------------------------------
#include <EGL/egl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
#include <unistd.h>		/* for open/close */
#include <fcntl.h>		/* for O_RDWR */
#include <sys/ioctl.h>	/* for ioctl */
#include <sys/types.h>	/* for mmap options */
#include <sys/mman.h>	/* for mmap options */

/* The single global variable, pointing to a structure of all other global data. */
static void *f_pStatics = NULL;

void terminateStatics(void)
{
	Statics *pStatic = getStatics();
	free(pStatic);
	setStatics(NULL);
}
void initializeStatics(void)
{
	Statics *pStatics = (Statics *)calloc(1, sizeof(Statics));
	if(pStatics == NULL)
	{
		fprintf(stderr, "Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}
	setStatics(pStatics);
}
Statics *getStatics(void)
{
	return (Statics *)f_pStatics;
}
void setStatics(void *pStatics)
{
	f_pStatics = pStatics;
}



//----------------------------------------------------------------------------------
/* createWindow():	Set up Linux specific bits.
 *
 * uiWidth:	 Width of window to create.
 * uiHeight:	Height of window to create.
 */
int createWindow(int uiWidth, int uiHeight)
{
	Statics *pStatics = getStatics();

	pStatics->sWindow = (fbdev_window *)calloc(1, sizeof(fbdev_window));
	if(pStatics->sWindow == NULL)
	{
		fprintf(stderr, "Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}
	pStatics->sWindow->width = uiWidth;
	pStatics->sWindow->height = uiHeight;

	return 0;
}

/* Destroy the window. */
int destroyWindow(void)
{
	Statics *pStatics = getStatics();

	free(pStatics->sWindow);

	return 0;
}

//------------------------------------------------------------------------------
unsigned int OS_GetTickCount( void )
{
#if defined(linux)
	timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#else
	return GetTickCount();
#endif
}


//------------------------------------------------------------------------------
void OS_Sleep( unsigned int milliseconds )
{
#if defined(UNDER_CE)
	Sleep(milliseconds);
#elif defined(linux)
	usleep(milliseconds*1000);
#endif
}

