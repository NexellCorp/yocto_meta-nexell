#ifndef _TUTORIAL1_H
#define _TUTORIAL1_H


//OpenGL ES Includes
#include <GLES/gl.h>

/*EGL is the "machine" that glues OpenGL ES with the underlying
windowing system. We need it to create a suitable context and a drawable window*/
#include <EGL/egl.h>

/*Because we are building software device dependent (the PDA), we have care about 
its limitations. PDA's doesn't have a FPU unit, so all floating point operations 
are emulated in the CPU. To have real data type, PDA's uses reals with a fixed point
format. For a fixed point number we only need an integer, with the same size (in bytes)
that a float, that is, a normal int number. The first 16 bits of the int will be the 
"integer" part, and the last 16 bits will be the "real" part. This will cause a lack 
of precision, but it is better than emulate all FPU in the CPU. To convert an integer 
number to a fixed point number we need to displace its bits to the left, as the FixedFromInt 
function does. In this chapter we only will need the conversion int->fixed point.
Other conversions will be showed when needed, in later chapters. A complete description of 
the fixed point maths is beyond the purpose of this set of tutorials, but the topic will
be widely covered through the chapters. 
OpenGL ES offers us a set of functions that works with fixed point (Glfixed). These 
functions are available through the OpenGL ES OES_fixed_point extension. 
A little word about the OpenGL ES extensions: They are divided into two categories: 
those that are fully integrated into the profile definition (core additions); and those
that remain extensions (profile extensions). Core additions do not use extension suffixes
and does not requires initialization, whereas profile extensions retain their extension suffixes.
OES_fixed_point is a core addition. The other extensions are listed and explained in the 
OpenGL ES 1.1 specification.*/

	
namespace __TUTORIAL1__
{
	#define PRECISION 16	
	#define ONE	(1 << PRECISION)
	#define ZERO 0

	inline GLfixed FixedFromInt(int value) {return value << PRECISION;};
	GLboolean InitOGLES();// Our GL initialization function
	void Render();  // Our Render function
	void Clean();   //Our clean function. It will clean all used resources	
}	
#endif
