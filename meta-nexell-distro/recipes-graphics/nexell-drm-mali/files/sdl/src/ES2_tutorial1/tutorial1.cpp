#include "t1_render.h"

namespace __TUTORIAL1__
{
//Some useful global handles
//HINSTANCE hInst; //Will hold the current instance of the application.
//NativeWindowType hNativeWnd = 0; // A handle to the window we will create.
//HDC hDC;   // A handle to the device context of the window.
}//namespace

using namespace __TUTORIAL1__;

//TCHAR szAppName[] = L"OpenGLES"; /*The application name and the window caption*/

//GLboolean drawInOrtho = GL_TRUE; //This variable will change with every click in the touch screen of the pda

/*This is the main function. Here we will create the rendering window, initialize OpenGL ES, write the message loop, and, at the end, clean all and release all used resources*/
int main()
{
	Statics *pStatics = NULL;

	/* Initialize the single global data pointer. */
	/* The pStatics pointer will point to a structure containing all global data. */
	initializeStatics();
	pStatics = getStatics();

	/* Initialize windowing system. */
	createWindow(DISP_WIDTH, DISP_HEIGHT);

	/* Initialize EGL. */
	initializeEGL();

	if(!InitOGLES()) return GL_FALSE; //OpenGL ES Initialization

	unsigned int frames = 1800;
	while(/*frames--*/1)
	{		
		Render();
		eglSwapBuffers(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLSurface);    

	}
	//Clean up all
	Clean();

	/* Shut down EGL. */
	terminateEGL();

	/* Shut down windowing system. */
	destroyWindow();

	/* Clear up globals. */
	terminateStatics();

	return 0;
}
