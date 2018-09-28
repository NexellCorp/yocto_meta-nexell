#ifndef RENDER_H
#define RENDER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <base_interface.h>
#include "Fake_OS.h"

#if 1
#define EGL_CHECK(x) \
	x; \
	{ \
		EGLint eglError = eglGetError(); \
		if (eglError != EGL_SUCCESS) { \
			printf("eglGetError() = %i (0x%.8x) at %s:%i\n", eglError, eglError, __FILE__, __LINE__); \
			exit(1); \
		} \
	}

#define GL_CHECK(x) \
	x; \
	{ \
		GLenum glError = glGetError(); \
		if (glError != GL_NO_ERROR) { \
			printf( "glGetError() = %i (0x%.8x) at %s:%i\n", glError, glError, __FILE__, __LINE__); \
			exit(1); \
		} \
	}
#else
#define EGL_CHECK(x) 	x
#define GL_CHECK(x)		x
#endif

#ifdef __cplusplus
extern "C" {
#endif

	
int initializeEGL();
int terminateEGL();

GLboolean InitOGLES();// Our GL initialization function
void Render();  // Our Render function
void SetPerspective();
GLboolean LoadTexture(const char *fileName, GLuint *id); //function to load 24-bit uncompressed TGA textures

//Our own gluPerspective-like function but modified to work with GLfixed
void Perspective (GLfloat fovy, GLfloat aspect, GLfloat zNear,  GLfloat zFar); 
void Clean();   //Our clean function. It will clean all used resources

#ifdef __cplusplus
}
#endif

#endif
