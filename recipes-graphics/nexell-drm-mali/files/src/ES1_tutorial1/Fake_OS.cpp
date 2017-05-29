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
#include <Fake_OS.h>
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
	#error "unknown platform"
#endif


//------------------------------------------------------------------------------
//  import
//------------------------------------------------------------------------------
//extern const unsigned char __door128_tga_Data[];
//extern const unsigned char __fire128_tga_Data[];
//extern const unsigned char 	__scene_gsd_Data[];
//extern const unsigned char 	__font_tga_Data[];
//extern const unsigned char 	__spot_raw_Data[];
//extern const unsigned char 	__plane_gsd_Data[];
//extern const unsigned char 	__font_raw_Data[];
//extern const unsigned char 	__floor_tga_Data[];
//extern const unsigned char 	__font_raw_Data[];
//extern const unsigned char 	__fire128_tga_Data[];
//extern const unsigned char 	__knot_gsd_Data[];
//extern const unsigned char  __alien1_gsd_Data       [];
//extern const unsigned char  __alien2_gsd_Data       [];
//extern const unsigned char  __alienShot_wav_Data    [];
//extern const unsigned char  __bala_gsd_Data         [];
//extern const unsigned char  __bunker_gsd_Data       [];
//extern const unsigned char  __bunker_tga_Data       [];
//extern const unsigned char  __bunkerXplosion_wav_Data[];
//extern const unsigned char  __creditos_tga_Data     [];
//extern const unsigned char  __espina_gsd_Data       [];
//extern const unsigned char  __font_raw_Data         [];
//extern const unsigned char  __keymap_tga_Data       [];
//extern const unsigned char  __nave1_tga_Data        [];
//extern const unsigned char  __nave2_tga_Data        [];
//extern const unsigned char  __portada_tga_Data      [];
//extern const unsigned char  __suelo_gsd_Data        [];
//extern const unsigned char  __suelo_tga_Data        [];
//extern const unsigned char  __tank_gsd_Data         [];
//extern const unsigned char  __tank_tga_Data         [];
//extern const unsigned char  __tankShot_wav_Data     [];
//extern const unsigned char  __tankXplosion_wav_Data [];
//extern const unsigned char	__font_modify_Data		[];

const char* __FILE_LIST__[] =
{
//	"/c/resources/door128.tga",		(const char*)__door128_tga_Data,
//	"/c/resources/fire128.tga",		(const char*)__fire128_tga_Data,
//	"/c/resources/floor.tga", 		(const char*)__floor_tga_Data,
//	"/c/resources/scene.gsd",		(const char*)__scene_gsd_Data,
//	"/c/resources/font.tga", 		(const char*)__font_tga_Data,
//	"/c/resources/font.raw", 		(const char*)__font_raw_Data,
//	"/c/resources/knot.gsd", 		(const char*)__knot_gsd_Data,
//	"/c/resources/spot.raw"	, 		(const char*)__spot_raw_Data,
//	"/c/resources/plane.gsd",		(const char*)__plane_gsd_Data,
//	"/c/resources/font.raw"	, 		(const char*)__font_raw_Data,
//    "/c/resources/alien1.gsd"        , (const char*)__alien1_gsd_Data        ,
//    "/c/resources/alien2.gsd"        , (const char*)__alien2_gsd_Data        ,
//    "/c/resources/alienShot.wav"     , (const char*)__alienShot_wav_Data     ,
//    "/c/resources/bala.gsd"          , (const char*)__bala_gsd_Data          ,
//    "/c/resources/bunker.gsd"        , (const char*)__bunker_gsd_Data        ,
//    "/c/resources/bunker.tga"        , (const char*)__bunker_tga_Data        ,
//    "/c/resources/bunkerXplosion.wav", (const char*)__bunkerXplosion_wav_Data,
//    "/c/resources/creditos.tga"      , (const char*)__creditos_tga_Data      ,
//    "/c/resources/espina.gsd"        , (const char*)__espina_gsd_Data        ,
//    //"/c/resources/font.raw"          , (const char*)__font_raw_Data          ,
//    "/c/resources/keymap.tga"        , (const char*)__keymap_tga_Data        ,
//    "/c/resources/nave1.tga"         , (const char*)__nave1_tga_Data         ,
//    "/c/resources/nave2.tga"         , (const char*)__nave2_tga_Data         ,
//    "/c/resources/portada.tga"       , (const char*)__portada_tga_Data       ,
//    "/c/resources/suelo.gsd"         , (const char*)__suelo_gsd_Data         ,
//    "/c/resources/suelo.tga"         , (const char*)__suelo_tga_Data         ,
//    "/c/resources/tank.gsd"          , (const char*)__tank_gsd_Data          ,
//    "/c/resources/tank.tga"          , (const char*)__tank_tga_Data          ,
//    "/c/resources/tankShot.wav"      , (const char*)__tankShot_wav_Data      ,
//    "/c/resources/tankXplosion.wav"  , (const char*)__tankXplosion_wav_Data  ,
//    "/c/resources/font_modify.tga"   , (const char*)__font_modify_Data  ,
	0
};

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

//----------------------------------------------------------------------------------
fbdev_window* OS_CreateWindow( void )
{
	fbdev_window *win_native;
    win_native = (fbdev_window *)malloc( sizeof( fbdev_window ) );
    memset(win_native, 0x0, sizeof( fbdev_window ));

	//set window size
    win_native->width = DISP_WIDTH;
    win_native->height = DISP_HEIGHT;

	return win_native;
}

void OS_DestroyWindow( fbdev_window* win_native )
{
	free(win_native);
	win_native = NULL;
}

void OS_GetWindowSize( fbdev_window* win_native, int* pWidth, int* pHeight )
{
	*pWidth  = win_native->width;
	*pHeight = win_native->height;
}

//------------------------------------------------------------------------------
unsigned int OS_GetTickCount( void )
{
#if defined(UNDER_CE)
	return GetTickCount();
#elif defined(linux)
	timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
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

//------------------------------------------------------------------------------
typedef struct
{
	const char*          m_pFileName;
	const unsigned char* m_pFileData;
	int                  m_CurPosition;
} NATIVE_FILE;
//------------------------------------------------------------------------------
OS_FILE OS_fopen( const char *filename, const char *mode )
{

	if( ('r' == mode[0]) || ('r' == mode[1]) )
	{
		int i = 0;

		while( __FILE_LIST__[i] )
		{
			if( !strcmp( filename, __FILE_LIST__[i] ) )
			{
				// create handle
				NATIVE_FILE* pFile = new NATIVE_FILE;

				pFile->m_pFileName = __FILE_LIST__[i];
				pFile->m_pFileData = (const unsigned char*)(__FILE_LIST__[i+1]);
				pFile->m_CurPosition = 0;

				return (OS_FILE)pFile;
			}
			else
				i+=2;
		}
		return 0;
	}
	else
		return 0;

	//return MES_FS_Open( filename, MES_O_RDONLY );

}

//------------------------------------------------------------------------------
int OS_fclose( OS_FILE stream )
{

	delete (NATIVE_FILE*)stream;
	return 0;

	//return MES_FS_Close( stream );
}

//------------------------------------------------------------------------------
int OS_fseek( OS_FILE stream, long offset, int origin )
{
	//return MES_FS_Seek( stream, offset, origin );

	int newPosition;

	// NATIVE_SEEK_CUR, NATIVE_SEEK_SET¸¸ Á¤ÀÇ.
	switch( origin )
	{
	case OS_SEEK_CUR:
		newPosition = ((NATIVE_FILE*)stream)->m_CurPosition + offset;
		break;

	case OS_SEEK_SET:
		newPosition = offset;
		break;
	case OS_SEEK_END:
	default:
		return -1;
	}

	if( newPosition >= 0 )
	{
		((NATIVE_FILE*)stream)->m_CurPosition = newPosition;
		return 0;
	}
	else
		return -1;
}

//------------------------------------------------------------------------------
unsigned int OS_fread( void *buffer, unsigned int size, unsigned int count, OS_FILE stream )
{

	size_t copiedSize = size * count;
	memcpy( buffer, ((((NATIVE_FILE*)stream)->m_pFileData)+(((NATIVE_FILE*)stream)->m_CurPosition)), copiedSize );
	((NATIVE_FILE*)stream)->m_CurPosition += copiedSize;

	return copiedSize;

	//return MES_FS_Read( stream, buffer, size*count );
}

