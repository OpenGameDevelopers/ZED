#ifndef __ZEDGLEXTENDER_HPP__
#define __ZEDGLEXTENDER_HPP__

#include <DataTypes.hpp>
#include <Renderer.hpp>
#include <list>

namespace ZED
{
	namespace Renderer
	{
		class GLExtender
		{
		public:
			ZED_INLINE GLExtender( ) : m_HDC( NULL ) { }
			explicit GLExtender( HDC p_HDC );
			~GLExtender( );
			ZED_INLINE void RegisterHDC( const HDC &p_HDC ) { m_HDC = p_HDC; }
			ZED_BOOL IsGLExtSupported( const char *p_Extension );

			PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
		private:
			void RegisterBaseWGLExtensions( );

			HDC m_HDC;
			std::list< std::string > m_Extensions;
		};
	}
}

// Define the OpenGL extensions here
/*PFNGLGETSTRINGIPROC glGetStringi;
PFNGLCREATESHADERPROC glCreateShader;*/

#endif
