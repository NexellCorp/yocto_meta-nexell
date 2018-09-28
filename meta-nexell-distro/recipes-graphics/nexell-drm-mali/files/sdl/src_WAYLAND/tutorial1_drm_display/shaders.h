/* vim:set sts=4 ts=4 noexpandtab: */
/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef __shaders_h
#define __shaders_h

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

void processShader(GLuint *pShader, const char *pString, GLint iShaderType);
void processBinaryShader(GLuint *pShader, const char *pdata_binary, unsigned int length, GLint iShaderType);

#endif /* __shaders_h */
