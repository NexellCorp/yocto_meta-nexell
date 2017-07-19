#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#include <math.h>
#include "Fake_OS.h"
#include <stdio.h>
#include <stdlib.h>
#include "tutorial1.h" //We need the defines and prototypes of there

namespace __TUTORIAL1__
{

	//Some useful global handles
	NativeWindowType hNativeWnd = 0; // A handle to the window we will create


	// OpenGL variables
	EGLDisplay glesDisplay;  // EGL display
	EGLSurface glesSurface;	 // EGL rendering surface
	EGLContext glesContext;	 // EGL rendering context

	GLboolean InitOGLES()
	{
	    EGLConfig config;
		EGLConfig *configs = NULL;
		EGLNativeWindowType win;
		EGLint major, minor, num_config, max_num_config;
		int i;	
		#if 0
	    EGLint attrib_list[] = { EGL_RED_SIZE, 5,  EGL_GREEN_SIZE, 6,  EGL_BLUE_SIZE, 5, 
				     EGL_ALPHA_SIZE, 0,  EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES_BIT, 
				     EGL_SURFACE_TYPE,  EGL_WINDOW_BIT,  EGL_NONE };
		#else
	    EGLint attrib_list[] = { EGL_RED_SIZE, 8,  EGL_GREEN_SIZE, 8,  EGL_BLUE_SIZE, 8, 
				     EGL_ALPHA_SIZE, 8,  EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES_BIT, 
				     EGL_SURFACE_TYPE,  EGL_WINDOW_BIT,  EGL_NONE };
		#endif
		
		glesDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	    if ( EGL_NO_DISPLAY == glesDisplay )
	    {
			printf( "eglGetDisplay() failed (error 0x%x)\n", eglGetError() );
			return 1;
	    }		

	    if ( EGL_FALSE == eglInitialize( glesDisplay, &major, &minor ) )
	    {
			printf( "eglInitialize() failed (error 0x%x)\n", eglGetError() );
			return 1;
	    }

		/* step2 - find the number of configs */
	 	if ( EGL_FALSE == eglGetConfigs(glesDisplay, NULL, 0, &max_num_config) )
		{
			printf( "eglGetConfigs() failed to retrive the number of configs (error 0x%x)\n", eglGetError() );
			return 1;
		}

		if(max_num_config <= 0)
		{
			printf( "No EGLconfigs found\n" );
			return 1;
		}

		configs = (EGLConfig *)malloc( sizeof( EGLConfig) * max_num_config );
		if ( NULL == configs )
		{
			printf( "Out of memory\n" );
			return 1;
		}
		
	    /* eglBindAPI( EGL_OPENGL_ES_API ); */
	 	/* step3 - find a suitable config */
	    if ( EGL_FALSE == eglChooseConfig( glesDisplay, attrib_list, configs, max_num_config, &num_config ) )
	    {
			printf( "eglChooseConfig() failed (error 0x%x)\n", eglGetError() );
			return 1;
	    }
	    if ( 0 == num_config )
	    {
			printf( "eglChooseConfig() was unable to find a suitable config\n" );
			return 1;
	    }

		for ( i=0; i<num_config; i++ )
		{
			EGLint value;
			
			/*Use this to explicitly check that the EGL config has the expected color depths */
			#if 0
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_RED_SIZE, &value );
			if ( 5 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_GREEN_SIZE, &value );
			if ( 6 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_BLUE_SIZE, &value );
			if ( 5 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_ALPHA_SIZE, &value );
			if ( 0 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_SAMPLES, &value );
			if ( 4 != value ) continue;
			#else
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_RED_SIZE, &value );
			if ( 8 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_GREEN_SIZE, &value );
			if ( 8 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_BLUE_SIZE, &value );
			if ( 8 != value ) continue;
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_ALPHA_SIZE, &value );
			if ( 8 != value ) continue;	
			eglGetConfigAttrib( glesDisplay, configs[i], EGL_SAMPLES, &value );
			if ( 4 != value ) continue;
			#endif
			config = configs[i];
			break;
		}
		
	 	/* step4 - create a window surface (512x512 pixels) */
	    win = (EGLNativeWindowType)hNativeWnd;
	    glesSurface = eglCreateWindowSurface( glesDisplay, config, win, NULL );
	    if ( EGL_NO_SURFACE == glesSurface )
	    {
			printf( "eglCreateWindowSurface failed (error 0x%x\n", eglGetError() );
			return 1;
	    }

	 	/* step5 - create a context */
	    glesContext = eglCreateContext( glesDisplay, config, EGL_NO_CONTEXT, NULL );
	    if ( EGL_NO_CONTEXT == glesContext )
	    {
			printf( "eglCreateContext failed (error 0x%x)\n", eglGetError() );
			return 1;
	    }
	    
	 	/* step6 - make the context and surface current */
	    if ( EGL_FALSE == eglMakeCurrent( glesDisplay, glesSurface, glesSurface, glesContext ) )
	    {
			printf( "eglMakeCurrent failed (error 0x%x)\n", eglGetError() );
			return 1;
	    }

		/*Remember: because we are programming for a mobile device, we cant
		use any of the OpenGL ES functions that finish in 'f', we must use
		the fixed point version (they finish in 'x'*/
		glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
		glShadeModel(GL_SMOOTH);

		/*Setup of the projection matrix. We will use an ortho cube centered
		at (0,0,0) with 100 units of edge*/
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrthox(FixedFromInt(-50), FixedFromInt(50),
		       FixedFromInt(-50), FixedFromInt(50),
		       FixedFromInt(-50), FixedFromInt(50));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable( GL_DEPTH_TEST);
		glDisable( GL_CULL_FACE );

		glViewport(0, 0, DISP_WIDTH, DISP_HEIGHT);
		return GL_TRUE;
	}
	
	//----------------------------------------------------------------------------
	void Render()
	{

	  static int rotation = 0;

	  #if 1
	  GLshort vertexArray[9] = {-25,-25,0,   25,-25,0,     0,25,0 };
	  GLubyte colorArray[12] = {255,0,0,255,   0,255,0,255,    0,0,255,255};
	  #else //force to black
	  GLshort vertexArray[12] = {-50,50,0,   -50,-50,0,     50,50,0, 	 50,-50,0	 };
	  GLubyte colorArray[16] = {255,0,0,0,   0,255,0,0,    0,0,255,0,    0,0,255,0};
	  #endif

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  
	  glLoadIdentity();

	  glTranslatex(0, 0, FixedFromInt(-10));
	  glRotatex(FixedFromInt(rotation++), 0, ONE,0);

#ifdef USE_TESTDRIVE
	  rotation += 30;
#endif

	  //Enable the vertices array
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(3, GL_SHORT, 0, vertexArray);
	  //3 = XYZ coordinates, GL_SHORT = data type, 0 = 0 stride bytes

	  //Enable the vertex color array
	  glEnableClientState(GL_COLOR_ARRAY);
	  glColorPointer(4,GL_UNSIGNED_BYTE, 0, colorArray);
	  //4 = RGBA format, GL_UNSIGNED_BYTE = data type,0=0 stide    bytes

	  glDrawArrays(GL_TRIANGLES, 0, 3);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glDisableClientState(GL_COLOR_ARRAY);
	}
	//----------------------------------------------------------------------------
	void Clean()
	{
	  if(glesDisplay)
	  {		  
	    eglMakeCurrent(glesDisplay, 0, 0, 0);		
	    if(glesContext) eglDestroyContext(glesDisplay, glesContext);		
	    if(glesSurface) eglDestroySurface(glesDisplay, glesSurface);
	    eglTerminate(glesDisplay);		
	  }
	}

}

using namespace __TUTORIAL1__;

/*This is the main function. Here we will create the rendering window, initialize OpenGL ES, write the message loop, and, at the end, clean all and release all used resources*/
//int Tutorial1Main()
int main(void)
{
	// Initialize native OS
	OS_InitFakeOS();
	GLboolean done = GL_FALSE;

	// Create native window.
	hNativeWnd = (NativeWindowType)OS_CreateWindow();
	if(!hNativeWnd) return GL_FALSE;


	if(!InitOGLES()) return GL_FALSE; //OpenGL ES Initialization
	
	unsigned int frames = 1;
	static int firstDoneFlag = 0;

	//Message Loop
	while(/*frames--*/1)
	{
		Render();		
		eglSwapBuffers(glesDisplay, glesSurface);	
	}

	//Clean up all
	Clean();
		
	OS_DestroyWindow(hNativeWnd);
	return GL_TRUE;
}










