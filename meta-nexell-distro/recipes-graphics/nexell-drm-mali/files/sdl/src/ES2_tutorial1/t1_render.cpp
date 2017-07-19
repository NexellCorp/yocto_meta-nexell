#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "t1_render.h" //We need the defines and prototypes of there
#include "shaders.h"

#define VERTEX_SHADER_FILE		"./t1_rend.vert"
#define FRAGMENT_SHADER_FILE	"./t1_rend.frag"

namespace __TUTORIAL1__
{

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

int initializeEGL()
{
	EGLint g_aEGLAttributes[] =
	{
		EGL_SAMPLES,			0,
		EGL_RED_SIZE,			WINDOW_RED_SIZE,
		EGL_GREEN_SIZE,			WINDOW_GREEN_SIZE,
		EGL_BLUE_SIZE,			WINDOW_BLUE_SIZE,
		EGL_ALPHA_SIZE,			0,
		EGL_BUFFER_SIZE,		WINDOW_BUFFER_SIZE,
		EGL_DEPTH_SIZE,			16,
		EGL_STENCIL_SIZE,		0,
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
	pStatics->sEGLInfo.sEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
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

				if(iEGLValue == WINDOW_BLUE_SIZE) break;
			}
		}
	}
	if(iEGLConfig >= cEGLConfigs)
	{
		fprintf(stderr, "Error: Failed to find matching EGL config at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}

	/* Create a surface. */
	pStatics->sEGLInfo.sEGLSurface = eglCreateWindowSurface(pStatics->sEGLInfo.sEGLDisplay, pEGLConfig[iEGLConfig], (EGLNativeWindowType)(pStatics->sWindow), NULL);
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
	eglDestroySurface(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLSurface);
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
	processShader(&iVertName, VERTEX_SHADER_FILE, GL_VERTEX_SHADER);
	processShader(&iFragName, FRAGMENT_SHADER_FILE, GL_FRAGMENT_SHADER);

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

	return 1;
}
//----------------------------------------------------------------------------
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw triangle. */
	glUseProgram(iProgName);

	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, aTriangleVertex);

	if(iLocFillColor != -1)
	{
		glEnableVertexAttribArray(iLocFillColor);
		glVertexAttribPointer(iLocFillColor, 4, GL_FLOAT, GL_FALSE, 0, aTriangleColor);
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//----------------------------------------------------------------------------
void Clean()
{
}

}//namespace







