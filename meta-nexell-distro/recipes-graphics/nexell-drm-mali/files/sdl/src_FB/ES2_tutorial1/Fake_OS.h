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
//	File       : Fake_OS.h
//	Description:
//	Author     : Yuni(yuni@mesdigital.com)
//	Export     :
//	History    :
//	   2007/01/22 Gamza first implementation
//------------------------------------------------------------------------------
#ifndef __FAKE_OS_H__
#define __FAKE_OS_H__

#include <EGL/egl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DISP_WIDTH	1024
#define DISP_HEIGHT	600

//------------------------------------------------------------------------------
//	Type definition
//------------------------------------------------------------------------------
#ifndef		NULL
	#define		NULL	0
#endif

#if defined(linux)
	typedef	unsigned short	WORD;
	typedef	unsigned int	DWORD;

#ifndef true
	#define true	1
#endif

#ifndef	false
	#define false	0
#endif

#ifndef	byte
	typedef char	byte;
#endif
#else
	#include <windows.h>
#endif

#if 1
#ifdef WIN32
#	define WINDOW_RED_SIZE		8
#	define WINDOW_GREEN_SIZE	8
#	define WINDOW_BLUE_SIZE		8
#	define WINDOW_BUFFER_SIZE	32
#else /* WIN32 */
#	define WINDOW_RED_SIZE		8
#	define WINDOW_GREEN_SIZE	8
#	define WINDOW_BLUE_SIZE		8
#	define WINDOW_BUFFER_SIZE	32
#endif /* WIN32 */

typedef struct
{
    EGLDisplay sEGLDisplay;
	EGLContext sEGLContext;
	EGLSurface sEGLSurface;
} EGLInfo;

typedef struct
{
	EGLInfo sEGLInfo;
#ifdef WIN32
	HWND sWindow;
	MSG sMessage;
	HDC sDC;
#else /* WIN32 */
	fbdev_window *sWindow;
#endif /* WIN32 */
} Statics;

static const EGLint aEGLContextAttributes[] =
{
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE
};

Statics *getStatics(void);
void setStatics(void *pStatics);
void terminateStatics(void);
void initializeStatics(void);
int createWindow(int uiWidth, int uiHeight);
int destroyWindow(void);

#endif


//------------------------------------------------------------------------------
//	Initialize platform
//------------------------------------------------------------------------------
void 	OS_InitFakeOS( void );

//------------------------------------------------------------------------------
//	Window
//------------------------------------------------------------------------------
typedef	void*	OS_Display;
typedef void*	OS_Window;
void OS_CreateWindow( void );
void OS_DestroyWindow( NativeWindowType window );
void OS_GetWindowSize( fbdev_window* win_native, int* pWidth, int* pHeight );

//------------------------------------------------------------------------------
//	Timer
//------------------------------------------------------------------------------
unsigned int 	OS_GetTickCount( void );
void			OS_Sleep( unsigned int milliseconds );

#ifdef __cplusplus
}
#endif
#endif // __FAKE_OS_H__

