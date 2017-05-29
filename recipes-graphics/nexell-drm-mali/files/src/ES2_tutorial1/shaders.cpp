/* vim: set sts=4 ts=4 noexpandtab: */
/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#include <GLES2/gl2.h>

#include <stdio.h>
#include <stdlib.h>

#include "shaders.h"

/* processShader(): Create shader, load in source, compile, dump debug as necessary.
 *
 * pShader: Pointer to return created shader ID.
 * sFilename: Passed-in filename from which to load shader source.
 * iShaderType: Passed to GL, e.g. GL_VERTEX_SHADER.
 */
void processShader(GLuint *pShader, char *sFilename, GLint iShaderType)
{
	GLint iStatus;
	const char *aStrings[1] = { NULL };

	/* Create shader and load into GL. */
	*pShader = glCreateShader(iShaderType);
	aStrings[0] = loadShader(sFilename);
	glShaderSource(*pShader, 1, aStrings, NULL);

	/* Clean up shader source. */
	free((void *)(aStrings[0]));
	aStrings[0] = NULL;

	/* Try compiling the shader. */
	glCompileShader(*pShader);
	glGetShaderiv(*pShader, GL_COMPILE_STATUS, &iStatus);

	/* Dump debug info (source and log) if compilation failed. */
	if(iStatus != GL_TRUE) {
		GLint iLen;
		char *sDebugSource = NULL;
		char *sErrorLog = NULL;

		/* Get shader source. */
		glGetShaderiv(*pShader, GL_SHADER_SOURCE_LENGTH, &iLen);
		sDebugSource = (char *)malloc(iLen);
		glGetShaderSource(*pShader, iLen, NULL, sDebugSource);
		printf("Debug source START:\n%s\nDebug source END\n\n", sDebugSource);
		free(sDebugSource);

		/* Now get the info log. */
		glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &iLen);
		sErrorLog = (char *)malloc(iLen);
		glGetShaderInfoLog(*pShader, iLen, NULL, sErrorLog);
		printf("Log START:\n%s\nLog END\n\n", sErrorLog);
		free(sErrorLog);

		printf("Compilation FAILED!\n\n");
		exit(1);
	}
}



/* loadShader():	Load the shader source into memory.
 *
 * sFilename: String holding filename to load.
 */
char *loadShader(char *sFilename)
{
	char *pResult = NULL;
	FILE *pFile = NULL;
	long iLen = 0;

	pFile = fopen(sFilename, "r");
	if(pFile == NULL) {
		printf("Error: Cannot read file '%s'\n", sFilename);
		exit(1);
	}
	fseek(pFile, 0, SEEK_END); /* Seek end of file. */
	iLen = ftell(pFile);
	fseek(pFile, 0, SEEK_SET); /* Seek start of file again. */
	pResult = (char *)calloc(iLen+1, sizeof(char));
	if(pResult == NULL)
	{
		printf("Error: Out of memory at %s:%i\n", __FILE__, __LINE__);
		exit(1);
	}
	fread(pResult, sizeof(char), iLen, pFile);
	pResult[iLen] = '\0';
	fclose(pFile);

	return pResult;
}

