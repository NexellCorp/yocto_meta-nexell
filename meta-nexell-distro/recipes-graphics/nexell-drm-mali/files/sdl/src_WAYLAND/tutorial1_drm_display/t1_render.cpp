#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <gbm.h>
#include <unistd.h>

#include "t1_render.h" //We need the defines and prototypes of there
#include "shaders.h"
#include "drm_display.h"

#define NX_FEATURE_GLLIB_PIXMAP_OPTIMIZE_EN

#if 0
#define VERTEX_SHADER_FILE		"/test/res/t1_rend.vert"
#define FRAGMENT_SHADER_FILE	"/test/res/t1_rend.frag"
#else
const char shader_gles_vertex_shader[] = {
"					\n\
	#version 100	\n\
	attribute vec4 a_v4Position;	\n\
	attribute vec4 a_v4FillColor;	\n\
	varying vec4 v_v4FillColor;	\n\
	void main()	\n\
	{	\n\
		v_v4FillColor = a_v4FillColor;	\n\
		gl_Position = a_v4Position;	\n\
	}	\n\
"
};


const char shader_gles_fragment_shader[] = {
"					\n\
	#version 100	\n\
	precision mediump float;	\n\
	varying vec4 v_v4FillColor;	\n\
	void main()	\n\
	{	\n\
		gl_FragColor = v_v4FillColor;	\n\
	}	\n\
"
};
#define VERTEX_SHADER_SOURCE		shader_gles_vertex_shader
#define FRAGMENT_SHADER_SOURCE		shader_gles_fragment_shader
#endif

EGLDisplay glesDisplay;  // EGL display
EGLSurface glesSurface;	 // EGL rendering surface
EGLContext glesContext;	 // EGL rendering context

//Texture handles
GLuint texture1 = 0; 
GLuint texture2 = 0;

/* Shader variables. */
GLuint iVertName = 0;
GLuint iFragName = 0;
GLuint iProgName = 0;
GLint iLocPosition = -1;
GLint iLocFillColor = -1;

static struct gbm_device *gbm_dev;

/* Simple triangle. */
const float aTriangleVertex[] =
{
	 0.0f,  0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
};

const float aTriangleColor[] =
{
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
};	

#if 0
static PFNEGLCREATEIMAGEKHRPROC _NX_eglCreateImageKHR = NULL;
static PFNEGLDESTROYIMAGEKHRPROC _NX_eglDestroyImageKHR = NULL;
static PFNGLEGLIMAGETARGETTEXTURE2DOESPROC _NX_glEGLImageTargetTexture2DOES = NULL;
static PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC _NX_glEGLImageTargetRenderbufferStorageOES = NULL;
static EGLNativePixmapType g_pixmap_output = NULL;
static int* gp_dma_fd = NULL;

struct fbdev_dma_buf
{
	int fd;
	int size;
	void *ptr;
};

static EGLNativePixmapType PixmapCreate(unsigned int stride, unsigned int width, unsigned int height, 
		unsigned int x_offset_pixel, int dma_fd, unsigned int hvid_mem_idx, unsigned int pixel_bits)
{	
	VR_PLATFORM_PIXMAP_STRUCT* pPixmap = (VR_PLATFORM_PIXMAP_STRUCT *)calloc(1, sizeof(VR_PLATFORM_PIXMAP_STRUCT));
	if (pPixmap == NULL)
	{
		printf("Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
		return (EGLNativePixmapType)0;
	}

	if (32 == pixel_bits)
	{
		pPixmap->bytes_per_pixel = 4;
		pPixmap->buffer_size = 32;
		pPixmap->red_size = 8;
		pPixmap->green_size = 8;
		pPixmap->blue_size = 8;
		pPixmap->alpha_size = 8;
		pPixmap->luminance_size = 0;
	}
	else if (16 == pixel_bits)
	{
		pPixmap->bytes_per_pixel = 2;
		pPixmap->buffer_size = 16;
		pPixmap->red_size = 0;
		pPixmap->green_size = 0;
		pPixmap->blue_size = 0;
		pPixmap->alpha_size = 8;
		pPixmap->luminance_size = 8;
	}		
	else if (8 == pixel_bits)
	{
		pPixmap->bytes_per_pixel = 1;
		pPixmap->buffer_size = 8;
		pPixmap->red_size = 0;
		pPixmap->green_size = 0;
		pPixmap->blue_size = 0;
		pPixmap->alpha_size = 0;
		pPixmap->luminance_size = 8;
	}	
	else
	{
		printf("Error: pixel_bits(%d) is not valid. %s:%i\n", pixel_bits, __FILE__, __LINE__);
		free(pPixmap);
		return (EGLNativePixmapType)0;
	}
	width = VR_ALIGN(width, 2);
	pPixmap->width = width;
	pPixmap->height = height;	
	unsigned int x_offset_byte = x_offset_pixel * pPixmap->bytes_per_pixel;
	#ifdef NX_FEATURE_GLLIB_PIXMAP_STRIDE_OFFSET_EN
	pPixmap->stride = stride;
	//나누기 2를 해야지 정상동작 
	pPixmap->x_offset_byte = x_offset_byte / 2;
	#endif

	#ifdef NX_FEATURE_GLLIB_PIXMAP_OPTIMIZE_EN
	pPixmap->is_no_readback_proc = VR_TRUE;
	#endif
	int fd_handle = dma_fd;
	
	pPixmap->flags = (fbdev_pixmap_flags)FBDEV_PIXMAP_COLORSPACE_sRGB;
	pPixmap->flags = (fbdev_pixmap_flags)(pPixmap->flags |FBDEV_PIXMAP_DMA_BUF);

	fbdev_dma_buf *pinfo_dma_buf = (fbdev_dma_buf *)calloc(1, sizeof(fbdev_dma_buf));
	if (pinfo_dma_buf == NULL)
	{
		printf("Error: NULL memory at %s:%i\n", __FILE__, __LINE__);
		free(pPixmap);
		return (EGLNativePixmapType)0;
	}
	pinfo_dma_buf->fd = fd_handle;
	pinfo_dma_buf->ptr = NULL;
	pinfo_dma_buf->size = width * height * (pixel_bits/8);

	pPixmap->data = (unsigned short *)pinfo_dma_buf;
	*((int*)pPixmap->data) = (int)fd_handle;

	return (EGLNativePixmapType)pPixmap;
}

static void PixmapDestroy(EGLNativePixmapType pPixmap)
{
	if (((VR_PLATFORM_PIXMAP_STRUCT*)pPixmap)->data)
		free(((VR_PLATFORM_PIXMAP_STRUCT*)pPixmap)->data);
	if (pPixmap)
		free(pPixmap);
}

EGLSurface PixmapSurfCreate(EGLDisplay display, EGLConfig config, unsigned int width, unsigned int height, int dma_fd)
{
	EGLSurface surface = NULL;
	assert(width && height && ((width&0x1) == 0x0) && ((height&0x1) == 0x0) );
	if(dma_fd < 0)
	{	
		printf("Error: hvid_mem is NULL\n");
		return NULL;
	}
	
	/* Create a EGLNativePixmapType. */
	unsigned int pixmap_stride, pixmap_width, pixmap_height;
	pixmap_stride = width;
	pixmap_width = width;
	pixmap_height = height;

	g_pixmap_output = (EGLNativePixmapType)PixmapCreate(pixmap_stride, pixmap_width, pixmap_height, 
									0, dma_fd, 0, 32);	

	if (g_pixmap_output == (EGLNativePixmapType)NULL || ((VR_PLATFORM_PIXMAP_STRUCT*)g_pixmap_output)->data == NULL)
	{
		printf("Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
		return NULL;
	}

	/* Create a EGLSurface. */
	surface = EGL_CHECK(eglCreatePixmapSurface(display, config, g_pixmap_output, NULL));

	if (!surface)
	{
		printf("Error: Failed to create target at %s:%i\n", __FILE__, __LINE__);
		return NULL;
	}

	return surface;
}

void PixmapSurfDestroy(EGLDisplay display, EGLSurface surface)
{	
	EGL_CHECK(eglDestroySurface(display, surface));
	
	if (g_pixmap_output) { PixmapDestroy(g_pixmap_output); }		
}
#endif

int initializeEGL(struct gbm_device *gbm_dev, EGLNativeWindowType drm_surface)
{
	EGLint g_aEGLAttributes[] =
	{
		EGL_SAMPLES,			EGL_DONT_CARE,
		EGL_RED_SIZE,			WINDOW_RED_SIZE,
		EGL_GREEN_SIZE,			WINDOW_GREEN_SIZE,
		EGL_BLUE_SIZE,			WINDOW_BLUE_SIZE,
		EGL_ALPHA_SIZE,			EGL_DONT_CARE,
		EGL_BUFFER_SIZE,		WINDOW_BUFFER_SIZE,
		EGL_DEPTH_SIZE,			EGL_DONT_CARE,
		EGL_STENCIL_SIZE,		EGL_DONT_CARE,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	Statics *pStatics = getStatics();
	EGLConfig *pEGLConfig = NULL;
	EGLint cEGLConfigs = 0;
	int iEGLConfig = 0;
	EGLBoolean bResult = EGL_FALSE;

#ifdef WIN32
    pStatics->sEGLInfo.sEGLDisplay = eglGetDisplay(pStatics->sDC);
#else /* WIN32 */
	//pStatics->sEGLInfo.sEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	#if 0
	fd = open("/dev/dri/card0", O_RDWR | FD_CLOEXEC);
	gbm_dev = gbm_create_device(drm_fd);
	if (!gbm_dev) {
		abort();
	}
	#endif
	pStatics->sEGLInfo.sEGLDisplay = eglGetDisplay(gbm_dev);

#endif /* WIN32 */

	if(pStatics->sEGLInfo.sEGLDisplay == EGL_NO_DISPLAY)
	{
		fprintf(stderr, "Error: No EGL Display available at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	/* Initialize EGL. */
    bResult = eglInitialize(pStatics->sEGLInfo.sEGLDisplay, NULL, NULL);
	if(bResult != EGL_TRUE)
	{
		EGLint iError = eglGetError();
		fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
		fprintf(stderr, "Error: Failed to initialize EGL at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	/* Enumerate available EGL configurations which match or exceed our required attribute list. */
	bResult = eglChooseConfig(pStatics->sEGLInfo.sEGLDisplay, g_aEGLAttributes, NULL, 0, &cEGLConfigs);
	if(bResult != EGL_TRUE)
	{
		EGLint iError = eglGetError();
		fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
		fprintf(stderr, "Error: Failed to enumerate EGL configs at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	/* Allocate space for all EGL configs available and get them. */
	pEGLConfig = (EGLConfig *)calloc(cEGLConfigs, sizeof(EGLConfig));
	if(pEGLConfig == NULL)
	{
		fprintf(stderr, "Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}
	bResult = eglChooseConfig(pStatics->sEGLInfo.sEGLDisplay, g_aEGLAttributes, pEGLConfig, cEGLConfigs, &cEGLConfigs);
	if(bResult != EGL_TRUE)
	{
		EGLint iError = eglGetError();
		fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
		fprintf(stderr, "Error: Failed to enumerate EGL configs at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	/* Loop through the EGL configs to find a color depth match.
	 * NB This is necessary, since EGL considers a higher color depth than requested to be 'better'
	 * even though this may force the driver to use a slow color conversion blitting routine. */
	for(iEGLConfig = 0; iEGLConfig < cEGLConfigs; iEGLConfig ++)
	{
		EGLint iEGLValue = 0;

		bResult = eglGetConfigAttrib(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], EGL_RED_SIZE, &iEGLValue);
		if(bResult != EGL_TRUE)
		{
			EGLint iError = eglGetError();
			fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
			fprintf(stderr, "Error: Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
			exit(1);
		}

		if(iEGLValue == WINDOW_RED_SIZE)
		{
			bResult = eglGetConfigAttrib(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], EGL_GREEN_SIZE, &iEGLValue);
			if(bResult != EGL_TRUE)
			{
				EGLint iError = eglGetError();
				fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
				fprintf(stderr, "Error: Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
				exit(1);
			}

			if(iEGLValue == WINDOW_GREEN_SIZE)
			{
				bResult = eglGetConfigAttrib(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], EGL_BLUE_SIZE, &iEGLValue);
				if(bResult != EGL_TRUE)
				{
					EGLint iError = eglGetError();
					fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
					fprintf(stderr, "Error: Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
					exit(1);
				}

				if(iEGLValue == WINDOW_BLUE_SIZE) {
					 EGLint gbm_format;

            				if (!eglGetConfigAttrib(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig],
                                    		EGL_NATIVE_VISUAL_ID, &gbm_format)) {
                				abort();
            				}

            				if (gbm_format ==GBM_FORMAT_XRGB8888) {
						break;
					}
				}
			}
		}
	}
	if(iEGLConfig >= cEGLConfigs)
	{
		fprintf(stderr, "Error: Failed to find matching EGL config at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	#if 1
	pStatics->sWindow = drm_surface;//(EGLNativeWindowType)gbm_surface_create(gbm_dev, DISP_WIDTH, DISP_HEIGHT, GBM_FORMAT_XRGB8888, (GBM_BO_USE_RENDERING | GBM_BO_USE_SCANOUT));
	if (!pStatics->sWindow) {
		abort();
	}
	
	/* Create a surface. */
	pStatics->sEGLInfo.sEGLSurface = eglCreateWindowSurface(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], (EGLNativeWindowType)(pStatics->sWindow), NULL);
	#else
	gp_dma_fd = test_drm_init(DISP_WIDTH, DISP_HEIGHT);
	pStatics->sEGLInfo.sEGLSurface = PixmapSurfCreate(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], DISP_WIDTH, DISP_HEIGHT, gp_dma_fd[0]);
	{
		_NX_eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress("eglCreateImageKHR");
		if ( NULL == _NX_eglCreateImageKHR ) exit(1);

		_NX_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress("eglDestroyImageKHR");
		if ( NULL == _NX_eglDestroyImageKHR ) exit(1);

		_NX_glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) eglGetProcAddress("glEGLImageTargetTexture2DOES");
		if ( NULL == _NX_glEGLImageTargetTexture2DOES ) exit(1);

		_NX_glEGLImageTargetRenderbufferStorageOES = (PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC) eglGetProcAddress("glEGLImageTargetRenderbufferStorageOES");
		if ( NULL == _NX_glEGLImageTargetRenderbufferStorageOES ) exit(1);		
	}	
	#endif	
	if(pStatics->sEGLInfo.sEGLSurface == EGL_NO_SURFACE)
	{
		EGLint iError = eglGetError();
		fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
		fprintf(stderr, "Error: Failed to create EGL surface at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	/* Create context. */
	pStatics->sEGLInfo.sEGLContext = eglCreateContext(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], EGL_NO_CONTEXT, aEGLContextAttributes);
	if(pStatics->sEGLInfo.sEGLContext == EGL_NO_CONTEXT)
	{
		EGLint iError = eglGetError();
		fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
		fprintf(stderr, "Error: Failed to create EGL context at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}	

	/* Make context current. */
	bResult = eglMakeCurrent(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLSurface, pStatics->sEGLInfo.sEGLSurface, pStatics->sEGLInfo.sEGLContext);
	if(bResult == EGL_FALSE)
	{
		EGLint iError = eglGetError();
		fprintf(stderr, "eglGetError(): %i (0x%.4x)\n", (int)iError, (int)iError);
		fprintf(stderr, "Error: Failed to make context current at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	free(pEGLConfig);
	pEGLConfig = NULL;

	return 0;
}

int terminateEGL()
{
	Statics *pStatics = getStatics();

	/* Shut down OpenGL-ES. */
	eglMakeCurrent(pStatics->sEGLInfo.sEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, pStatics->sEGLInfo.sEGLContext);
	eglDestroyContext(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLContext);
#if 1
	eglDestroySurface(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLSurface);
	//gbm_surface_destroy((struct gbm_surface *)pStatics->sWindow);
#else
	PixmapSurfDestroy(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLSurface);
	test_drm_deinit();
#endif
    eglTerminate(pStatics->sEGLInfo.sEGLDisplay);

	return 0;
}

GLboolean InitOGLES()
{     
	/* Initialize OpenGL-ES. */
	glEnable(GL_BLEND);
	/* Should do src * (src alpha) + dest * (1-src alpha). */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Load shaders. */
	#if 1
	processShader(&iVertName, VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);
	processShader(&iFragName, FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);
	#else
	{
		char *pResult = NULL;
		FILE *pFile = NULL;
		long iLen = 0;
	
		pFile = fopen("./res/vert.bin.non-prerotate", "r");
		if(pFile == NULL) {
			printf("Error: Cannot read file '%s'\n", "./res/vert.bin.non-prerotate");
			exit(1);
		}
		fseek(pFile, 0, SEEK_END); /* Seek end of file. */
		iLen = ftell(pFile);
		fseek(pFile, 0, SEEK_SET); /* Seek start of file again. */
		pResult = (char *)calloc(iLen, sizeof(char));
		if(pResult == NULL)
		{
			printf("Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
			exit(1);
		}
		fread(pResult, sizeof(char), iLen, pFile);
		fclose(pFile);
	
		processBinaryShader(&iVertName, pResult, iLen, GL_VERTEX_SHADER);
	}
	{
		char *pResult = NULL;
		FILE *pFile = NULL;
		long iLen = 0;
	
		pFile = fopen("./res/frag.bin.non-prerotate", "r");
		if(pFile == NULL) {
			printf("Error: Cannot read file '%s'\n", "./res/frag.bin.non-prerotate");
			exit(1);
		}
		fseek(pFile, 0, SEEK_END); /* Seek end of file. */
		iLen = ftell(pFile);
		fseek(pFile, 0, SEEK_SET); /* Seek start of file again. */
		pResult = (char *)calloc(iLen, sizeof(char));
		if(pResult == NULL)
		{
			printf("Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
			exit(1);
		}
		fread(pResult, sizeof(char), iLen, pFile);
		fclose(pFile);
	
		processBinaryShader(&iFragName, pResult, iLen, GL_FRAGMENT_SHADER);
	}	
	#endif

	/* Set up shaders. */
	iProgName = glCreateProgram();
	glAttachShader(iProgName, iVertName);
	glAttachShader(iProgName, iFragName);
	glLinkProgram(iProgName);
	glUseProgram(iProgName);

	/* Vertex positions. */
	iLocPosition = glGetAttribLocation(iProgName, "a_v4Position");
	if(iLocPosition == -1)
	{
		printf("Error: Attribute not found at %s:%i\n", __FILE__, __LINE__);
		return 0;
	}
	else glEnableVertexAttribArray(iLocPosition);

	/* Fill colors. */
	iLocFillColor = glGetAttribLocation(iProgName, "a_v4FillColor");
	if(iLocFillColor == -1)
	{
		printf("Warning: Attribute not found at %s:%i\n", __FILE__, __LINE__);
		return 0;
	}
	else glEnableVertexAttribArray(iLocFillColor);

	/* Set clear screen color. */
	glClearColor(0.0f, 0.0f, 1.0f, 1.0);

	GLboolean result = LoadTexture("/c/resources/door128.tga", &texture1);
	result     &= LoadTexture("/c/resources/fire128.tga", &texture2);
	//GLboolean result = LoadTexture("door128.tga", &texture1);
	//result     &= LoadTexture("fire128.tga", &texture2); 
	return result;
}
//----------------------------------------------------------------------------
void Render()
{
	static int b_draw;
	char *buf;

	buf = (char *)malloc(DISP_WIDTH * DISP_HEIGHT * 4);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* enable texture */
	glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture1);

	/* Draw triangle. */
	glUseProgram(iProgName);

	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, aTriangleVertex);

	if(iLocFillColor != -1)
	{
		glEnableVertexAttribArray(iLocFillColor);
		glVertexAttribPointer(iLocFillColor, 4, GL_FLOAT, GL_FALSE, 0, aTriangleColor);
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);

	#if 0
	if (b_draw == 0) {
		FILE *fd = fopen("test.raw", "w");

		glReadPixels(0, 0, DISP_WIDTH, DISP_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, buf);

		fwrite(buf, 1, DISP_WIDTH * DISP_HEIGHT * 4, fd);

		fclose(fd);
		
		b_draw = 1;
	}
	#endif
}

//----------------------------------------------------------------------------
void Clean()
{
	if(glesDisplay)
	{
		glDeleteTextures(1, &texture1);
		glDeleteTextures(1, &texture2);  	  	
	}  
}
//----------------------------------------------------------------------------
GLboolean LoadTexture(const char *fileName, GLuint *id)
{
	OS_FILE f = OS_fopen(fileName, "rb");
	GLubyte *pixels = NULL;
	if(!f) return false;
			
	WORD width = 0, height = 0;			
	byte headerLength = 0;					
	byte imageType = 0;				
	byte bits = 0;						
	int format= 0;					
	int lineWidth = 0;							
			
	OS_fread(&headerLength, sizeof(byte), 1, f);		
	//skip next byte
	OS_fseek(f,1,OS_SEEK_CUR); 

	//read in the imageType (RLE, RGB, etc...)
	OS_fread(&imageType, sizeof(byte), 1, f);

	//skip information we don't care about
	OS_fseek(f, 9, OS_SEEK_CUR); 

	//read the width, height and bits per pixel (16, 24 or 32). We only will take care of 24 bits uncompressed TGAs
	OS_fread(&width,  sizeof(WORD), 1, f);
	OS_fread(&height, sizeof(WORD), 1, f);
	OS_fread(&bits,   sizeof(byte), 1, f);

	//move the file pointer to the pixel data
	OS_fseek(f, headerLength + 1, OS_SEEK_CUR); 

	//check if the image is not compressed.
	if(imageType != 10)
	{
		//check if the image is a 24 
		if(bits == 24)
		{		
			format = bits >> 3; //Another way to divide between 8. We want to know the pixel size in bytes.
			lineWidth = format * width;
			pixels = new GLubyte[lineWidth * height];

			//we are going to load the pixel data line by line
			for(int y = 0; y < height; y++)
			{
				//Read current line of pixels
				GLubyte *line = &(pixels[lineWidth * y]);				
				OS_fread(line, lineWidth, 1, f);
			
				/*Because the TGA is BGR instead of RGB, we must swap the R and G components (OGL ES does not have the 
	      GL_BGR_EXT extension*/
				for(int i=0;i<lineWidth ; i+=format)
				{
					int temp  = line[i];
					line[i]   = line[i+2];
					line[i+2] = temp;
				}
			}
		}		
		else
	{      
	  OS_fclose(f);
	  *id = 0;
	  return false;	
	}
	}	
	OS_fclose(f);
				
	/*Creation of the OpenGL Texture:
	OpenGL ES texturing only allows 2D textures (1D textures could be easily achieved, using '1' as 
	height parameter in the glTexImage2D call). 

	Allowed filtering modes are:    
	GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR
	and GL_LINEAR_MIPMAP_LINEAR.
	Because OpenGL ES is written against OpenGL 1.5, it supports automatic mipmap generation, through:
	glTexParameterx(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); 

	Allowed clamping modes are:
	GL_REPEAT and GL_CLAMP_TO_EDGE (GL_CLAMP, GL_CLAMP_TO_BORDER and GL_MIRRORED_REPEAT are not supported)

	Allowed formats are:

	Internal & External Format              Type              Bytes per Pixel
	        GL_RGBA                   GL_UNSIGNED_BYTE               4
	        GL_RGB                    GL_UNSIGNED_BYTE               3
	        GL_RGBA                   GL_UNSIGNED_SHORT_4_4_4_4      2
	        GL_RGBA                   GL_UNSIGNED_SHORT_5_5_5_1      2
	        GL_RGB                    GL_UNSIGNED_SHORT_5_6_5        2
	        GL_LUMINANCE_ALPHA        GL_UNSIGNED_BYTE               2
	        GL_LUMINANCE              GL_UNSIGNED_BYTE               1
	        GL_ALPHA                  GL_UNSIGNED_BYTE               1

	Texture borders are not suported at all
	*/
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);  
	delete [] pixels;

	return true;
}







