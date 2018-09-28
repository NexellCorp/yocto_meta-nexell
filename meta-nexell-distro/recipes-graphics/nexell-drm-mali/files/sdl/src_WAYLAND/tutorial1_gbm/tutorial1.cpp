//#include "main.h" //We need the defines and prototypes from in.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#include "t1_render.h"

#if 1
void base_dbg_file_save_user_buffer(unsigned char* pbuf, const char* str_file_name, unsigned int width, unsigned int height, unsigned int bytes_per_pixel)
{
	char sName[128];
	static int run_count;
	FILE* fp;
	unsigned int bytes_size = width * bytes_per_pixel * height;
	sprintf((char*)sName, "%s_%d_%dx%d_%d.data", str_file_name, run_count++, width, height, bytes_per_pixel);

#ifdef WIN32
	fp = fopen((char*)sName, "wb");	
#else
	fp = fopen((char*)sName, "w");	
#endif
	if(!fp)
	{
		printf("can't open file(%s)\n", sName);
		return;
	}
	fwrite(pbuf, bytes_size , 1, fp);
	fflush(fp);		
	fclose(fp);
	printf("file saved: %s\n", sName);
}
#endif

/*This is the main function. Here we will create the rendering window, initialize OpenGL ES, write the message loop, and, at the end, clean all and release all used resources*/
int main()
{
	Statics *pStatics = NULL;

	printf("[app] tutorial1 begin\n");

	/* Initialize the single global data pointer. */
	/* The pStatics pointer will point to a structure containing all global data. */
	initializeStatics();
	pStatics = getStatics();

	/* Initialize EGL. */
	initializeEGL();

	if(!InitOGLES()) return GL_FALSE; //OpenGL ES Initialization

	unsigned int frames = 1;
	unsigned int count = 0;
	while(frames--)
	{		
		Render();

		#if 1
		if (0 == count)
		{		
			unsigned char* pdata = (unsigned char*)malloc(DISP_WIDTH*DISP_HEIGHT*4);
			glReadPixels(0, 0, DISP_WIDTH, DISP_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pdata);
			base_dbg_file_save_user_buffer((unsigned char*)pdata, "read", DISP_WIDTH, DISP_HEIGHT, 4);
			free(pdata);
		}
		#endif

		eglSwapBuffers(pStatics->sEGLInfo.sEGLDisplay, pStatics->sEGLInfo.sEGLSurface);    

		++count;
	}
	
	//Clean up all
	Clean();
	
	/* Shut down EGL. */
	terminateEGL();
	
	/* Clear up globals. */
	terminateStatics();	

	return 0;
}
