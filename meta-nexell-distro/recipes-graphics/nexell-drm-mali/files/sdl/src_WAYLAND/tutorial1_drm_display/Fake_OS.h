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

#define DISP_WIDTH	1280//1024
#define DISP_HEIGHT	800//600

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
	EGLNativeWindowType sWindow;
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
#endif

//------------------------------------------------------------------------------
//	Timer
//------------------------------------------------------------------------------
unsigned int 	OS_GetTickCount( void );
void			OS_Sleep( unsigned int milliseconds );

//------------------------------------------------------------------------------
//	File I/O
//------------------------------------------------------------------------------
#define OS_SEEK_SET		0
#define OS_SEEK_CUR		1
#define OS_SEEK_END		2
typedef void* OS_FILE;
OS_FILE OS_fopen( const char *filename, const char *mode );
int OS_fclose( OS_FILE stream );
int OS_fseek( OS_FILE stream, long offset, int origin );
unsigned int OS_fread( void *buffer, unsigned int size, unsigned int count, OS_FILE stream );

#ifdef __cplusplus
}
#endif
#endif // __FAKE_OS_H__

