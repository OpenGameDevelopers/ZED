#include <GLExtender.hpp>

// Define all the possible OpenGL extensions, initialising them to zero
// OpenGL 2.0 [Shaders]
PFNGLCREATESHADERPROC		__zglCreateShader = ZED_NULL;
PFNGLDELETESHADERPROC		__zglDeleteShader = ZED_NULL;
PFNGLSHADERSOURCEPROC		__zglShaderSource = ZED_NULL;
PFNGLCOMPILESHADERPROC		__zglCompileShader = ZED_NULL;
PFNGLGETSHADERINFOLOGPROC	__zglGetShaderInfoLog = ZED_NULL;
PFNGLCREATEPROGRAMPROC		__zglCreateProgram = ZED_NULL;
PFNGLATTACHSHADERPROC		__zglAttachShader = ZED_NULL;
PFNGLDETACHSHADERPROC		__zglDetachShader = ZED_NULL;

///////////////////////////////////////////////////////////////////////////////
// OpenGL 2.0 Extensions //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace ZED
{
	namespace Renderer
	{
		GLExtender::~GLExtender( )
		{
		}
	}
}

