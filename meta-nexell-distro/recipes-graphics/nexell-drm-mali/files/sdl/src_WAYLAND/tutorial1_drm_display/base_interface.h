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
#ifndef __VR_BASE_INTERFACE_H__
#define __VR_BASE_INTERFACE_H__

//------------------------------------------------------------------------------
//
//    Includes
//    
//------------------------------------------------------------------------------
#if 1
#if defined(linux)
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>		// for open/close
#include <fcntl.h>		// for O_RDWR
#include <sys/ioctl.h>	// for ioctl
#include <sys/types.h>	// for mmap options
#include <sys/mman.h>	// for mmap options 
#elif defined(WIN32)
#include <windows.h>
#endif
#endif
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#if defined(WIN32)
#pragma warning(disable: 4201)  
#pragma warning(disable: 4996)
#pragma warning(disable: 4127)  
#endif



//------------------------------------------------------------------------------
//
//    Defines
//    
//------------------------------------------------------------------------------
#if 1
  #ifdef __cplusplus
#define BASE_API		extern "C"
  #else
#define BASE_API		extern
  #endif //__cplusplus
#else
#define BASE_API  
#endif //BASE_API


#if defined( WIN32 )
	#define VR_INLINE			__inline
	#define VR_STATIC_INLINE	static __inline
	#define VR_LOCAL
#else
	#define VR_INLINE			inline
	#define VR_STATIC_INLINE	static inline
	#define VR_LOCAL 			__attribute__ ((visibility ("internal")))
#endif

#if defined(_WIN32)
    #define VR_CALLBACK     __stdcall
#else
    #define VR_CALLBACK
#endif

#if defined(VR_API_USE_DLL)
#if defined(VR_API_DLL_EXPORTS)
#   define VR_APICALL __declspec(dllexport)
#else
#   define VR_APICALL __declspec(dllimport)
#endif
#else
#   define VR_APICALL
#endif

#if defined(VR_DEVDRV_USE_DLL)
#if defined(VR_DEVDRV_DLL_EXPORTS)
#   define VR_DEVCALL __declspec(dllexport)
#else
#   define VR_DEVCALL __declspec(dllimport)
#endif
#else
#   define VR_DEVCALL
#endif

#if defined( WIN32 )
typedef __int64 VR_LONG;
typedef unsigned __int64 VR_ULONG;
#else
typedef long long VR_LONG;
typedef unsigned long long VR_ULONG;
#endif

typedef unsigned int 		VR_BOOL;
typedef int	                VR_F24;
typedef unsigned short	    VR_F16;
typedef signed char	    	VR_BYTE;
typedef unsigned char		VR_UBYTE;
typedef short	        	VR_SHORT;
typedef unsigned short		VR_USHORT;
typedef int	            	VR_INT;
typedef unsigned int		VR_UINT;
typedef void	        	VR_VOID;
typedef float	        	VR_FLOAT;
typedef int	            	VR_FIXED;
#if defined( WIN32 )
typedef __int64 			VR_LONG;
typedef unsigned __int64 	VR_ULONG;
#else
typedef long long 			VR_LONG;
typedef unsigned long long 	VR_ULONG;
#endif

typedef char               VR_S8 ;
typedef unsigned char      VR_U8 ;
typedef short              VR_S16;
typedef unsigned short     VR_U16;
typedef int                VR_S32;
typedef unsigned int       VR_U32;

#if defined( WIN32 )
typedef __int64            VR_S64;
typedef unsigned __int64   VR_U64;
#else
typedef long long          VR_S64;
typedef unsigned long long VR_U64;
#endif

#ifndef INT64_MAX
#define INT64_MAX 0x7fffffffffffffffLL
#endif

#define VR_TRUE			1
#define VR_FALSE		0

#define VR_STRING_MAX	256
#define KB 1024
#define MB (KB*KB)

#define VR_ADDR_BITS	32

//#define VR_BASE_USE_VMEM_LINK_FILESYSTEM
//#define _T(p_const_str)	p_const_str
#define VR_CASSERT_STATIC(expr)    typedef char __VR_C_ASSERT_STATIC__[(expr)?1:-1]
VR_CASSERT_STATIC( sizeof(VR_S8 )  == 1 );
VR_CASSERT_STATIC( sizeof(VR_U8 )  == 1 );
VR_CASSERT_STATIC( sizeof(VR_S16)  == 2 );
VR_CASSERT_STATIC( sizeof(VR_U16)  == 2 );
VR_CASSERT_STATIC( sizeof(VR_S32)  == 4 );
VR_CASSERT_STATIC( sizeof(VR_U32)  == 4 );
VR_CASSERT_STATIC( sizeof(VR_S64)  == 8 );
VR_CASSERT_STATIC( sizeof(VR_U64)  == 8 );
VR_CASSERT_STATIC( sizeof(VR_BOOL) == 4 );

//Align a positive integer to the nearest equal or higher multiple of some base.
#define VR_ALIGN( value, base ) (((value) + ((base) - 1)) & ~((base) - 1))

#define MAX_INT32  				(0x7fffffff)
#define MIN_INT32  				(-0x7fffffff-1)

#define VR_ABS(x)				((x) < 0) ? (-x) : (x)
#define	VR_MAX(x, y)			(((x) > (y)) ? (x) : (y))
#define	VR_MIN(x, y)			(((x) < (y)) ? (x) : (y))
#define	VR_CLAMP(x, min, max)	(((x) < (min)) ? (min) : ((x) > (max)) ? (max) : (x))
#define	VR_LOW_32_OF_64(x)		(*((unsigned int*)(&x)))
#define	VR_LOW_32_FROM_PTR(x)	((unsigned int)((VR_ULONG)(x) & 0xFFFFFFFFUL))

/* Branch hint */
#if defined(_MSC_VER)
#define __builtin_expect(expr,b) expr
#endif
#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

/*! Stringify macros */
#define JOIN(X, Y) _DO_JOIN(X, Y)
#define _DO_JOIN(X, Y) _DO_JOIN2(X, Y)
#define _DO_JOIN2(X, Y) X##Y

#if defined( WIN32 )
#define VR_ATTR_ALIGN
#else
#define VR_ATTR_ALIGN 	__attribute__ ((packed))
#endif

/* Number of DWORDS */
#define VR_SIZEOF32(X) (sizeof(X) / sizeof(uint32_t))

//	return code
typedef enum 
{
	VR_ERR_NO_ERROR,
	VR_ERR_FAILED				= -1,
	VR_ERR_OUT_OF_MEM			= -2,
	VR_ERR_OUT_OF_VMEM 			= -3,
	VR_ERR_INVALID_ARG			= -4,
	VR_ERR_TIMEOUT				= -5,
	VR_ERR_OS_SYSCALL_FAILED	= -6,
	VR_ERR_NOT_READY_YET		= -7,
	VR_ERR_ASSERT				= -8, // very critical error
}VR_BASE_DBG_RET_TYPE;


//---------------------------------------------------------
//    GL Device defines  
//---------------------------------------------------------
#ifdef WIN32
//VR(win32 emul) or mali 
#define NX_FEATURE_AVM_GLLIB_VR_USE
#include <platform_export.h>
typedef void* NX_MEMORY_HANDLE;
#endif

//------------------------------------------------------------------------------
//    VMEM Platrom Defines
//------------------------------------------------------------------------------
#ifdef WIN32
#define VR_PLATFORM_SW_ALLOC_USE
#else
/* source */
//#define VR_PLATFORM_ION_ALLOC_USE 
//#define VR_PLATFORM_DRM_ALLOC_USE 
#define VR_PLATFORM_DRM_USER_ALLOC_USE
/* destination */
//#define VR_PLATFORM_DST_FB_USE 
//#define VR_PLATFORM_DST_DRM_USE 
#define VR_PLATFORM_DST_PIXMAP_USE 
//#define VR_PLATFORM_DST_PIXMAP_CONNECT_TO_DISPLAY
/*  
	if use VR_PLATFORM_DST_PIXMAP_USE, 
	enable NX_FEATURE_AVM_DST_PIXMAP_USE at device_common_def.h
*/
#endif

#if defined( NX_FEATURE_AVM_GLLIB_VR_USE )
typedef VRPlatformDevicePixmap VR_PLATFORM_PIXMAP_STRUCT;

#elif defined( VR_PLATFORM_DRM_ALLOC_USE )
#include <fcntl.h>
#include <gbm.h>
#include <drm/drm_fourcc.h>
#include <nx_video_alloc.h>
#include <EGL/drm_window.h>
typedef fbdev_pixmap VR_PLATFORM_PIXMAP_STRUCT;

#elif defined( VR_PLATFORM_DRM_USER_ALLOC_USE )
typedef struct tagNxDrmVmem
{
	int drm_fd;
	int gem_fd;
	int flinks;
	int dma_fd;
	void *pBuffer;
	unsigned int size;
}NxDrmVmem;
#include <fcntl.h>
#include <gbm.h>
#include <drm/drm_fourcc.h>
#include <nx-drm-allocator.h>
#include <EGL/drm_window.h>
typedef fbdev_pixmap VR_PLATFORM_PIXMAP_STRUCT;
typedef void* NX_MEMORY_HANDLE;

#elif defined( VR_PLATFORM_ION_ALLOC_USE )

#include <nx_alloc_mem.h>
#include <EGL/fbdev_window.h>
typedef fbdev_window VR_PLATFORM_WINDOW_STRUCT;
typedef fbdev_pixmap VR_PLATFORM_PIXMAP_STRUCT;

#else
#error "choose valid vmem allocator"
#endif

#if defined( NX_FEATURE_AVM_GLLIB_VR_USE )
#include <platform_export.h>
#elif defined( VR_PLATFORM_DST_FB_USE )
#include <EGL/fbdev_window.h>
#elif defined( VR_PLATFORM_DST_DRM_USE )
#include <EGL/drm_window.h>
#else
/* platform has own window system */
#endif

#endif
