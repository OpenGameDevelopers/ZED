#ifndef __ZED_GLWEXTENDER_HPP__
#define __ZED_GLWEXTENDER_HPP__

#include <DataTypes.hpp>
#include <Renderer.hpp>
#include <list>

// Define the OpenGL extensions here
#ifdef __cplusplus
extern "C"
{
#endif

extern PFNGLCREATESHADERPROC			__zglCreateShader;
extern PFNGLDELETESHADERPROC			__zglDeleteShader;
extern PFNGLSHADERSOURCEPROC			__zglShaderSource;
extern PFNGLCOMPILESHADERPROC			__zglCompileShader;
extern PFNGLGETSHADERINFOLOGPROC		__zglGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC			__zglCreateProgram;
extern PFNGLATTACHSHADERPROC			__zglAttachShader;
extern PFNGLDETACHSHADERPROC			__zglDetachShader;
extern PFNGLISSHADERPROC				__zglIsShader;
extern PFNGLGETSHADERIVPROC				__zglGetShaderiv;
extern PFNGLLINKPROGRAMPROC				__zglLinkProgram;
extern PFNGLGETPROGRAMINFOLOGPROC		__zglGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC				__zglUseProgram;
extern PFNGLDELETEPROGRAMPROC			__zglDeleteProgram;
extern PFNGLISPROGRAMPROC				__zglIsProgram;
extern PFNGLVALIDATEPROGRAMPROC			__zglValidateProgram;
extern PFNGLGETPROGRAMIVPROC			__zglGetProgramiv;
extern PFNGLGENVERTEXARRAYSPROC			__zglGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC			__zglBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC		__zglDeleteVertexArrays;
extern PFNGLGENBUFFERSPROC				__zglGenBuffers;
extern PFNGLBINDBUFFERPROC				__zglBindBuffer;
extern PFNGLBUFFERDATAPROC				__zglBufferData;
extern PFNGLDELETEBUFFERSPROC			__zglDeleteBuffers;
extern PFNGLBINDATTRIBLOCATIONPROC		__zglBindAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC		__zglVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC __zglEnableVertexAttribArray;
extern PFNGLGETUNIFORMLOCATIONPROC		__zglGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC		__zglUniformMatrix4fv;
//extern PFNGLBINDFRAGDATALOCATIONPROC	__zglBindFragDataLocation;
extern PFNGLUNIFORM1IPROC				__zglUniform1i;
extern PFNGLUNIFORM3FVPROC				__zglUniform3fv;
extern PFNGLACTIVETEXTUREPROC			__zglActiveTexture;
extern PFNGLDELETETEXTURESEXTPROC		__zglDeleteTextures;

// It's not clean, but it works!
#define ZEDGL_GETFUNC( x )	x
// Declare all OpenGL functions via macros and grab them upon initialisation
#define zglGetStringi			ZEDGL_GETFUNC( __zglGetStringi )

///////////////////////////////////////////////////////////////////////////////
// Shader functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define zglCreateShader			ZEDGL_GETFUNC( __zglCreateShader )
#define zglDeleteShader			ZEDGL_GETFUNC( __zglDeleteShader )
#define zglShaderSource			ZEDGL_GETFUNC( __zglShaderSource )
#define zglCompileShader		ZEDGL_GETFUNC( __zglCompileShader )
#define zglGetShaderInfoLog		ZEDGL_GETFUNC( __zglGetShaderInfoLog )
#define zglCreateProgram		ZEDGL_GETFUNC( __zglCreateProgram )
#define zglAttachShader			ZEDGL_GETFUNC( __zglAttachShader )
#define zglDetachShader			ZEDGL_GETFUNC( __zglDetachShader )
#define zglIsShader				ZEDGL_GETFUNC( __zglIsShader )
#define zglGetShaderiv			ZEDGL_GETFUNC( __zglGetShaderiv )
#define zglLinkProgram			ZEDGL_GETFUNC( __zglLinkProgram )
#define zglGetProgramInfoLog	ZEDGL_GETFUNC( __zglGetProgramInfoLog )
#define zglUseProgram			ZEDGL_GETFUNC( __zglUseProgram )
#define	zglDeleteProgram		ZEDGL_GETFUNC( __zglDeleteProgram )
#define zglIsProgram			ZEDGL_GETFUNC( __zglIsProgram )
#define zglValidateProgram		ZEDGL_GETFUNC( __zglValidateProgram )
#define zglGetProgramiv			ZEDGL_GETFUNC( __zglGetProgramiv )
#define zglGenVertexArrays		ZEDGL_GETFUNC( __zglGenVertexArrays )
#define zglBindVertexArray		ZEDGL_GETFUNC( __zglBindVertexArray )
#define zglDeleteVertexArrays	ZEDGL_GETFUNC( __zglDeleteVertexArrays )
#define zglGenBuffers			ZEDGL_GETFUNC( __zglGenBuffers )
#define zglBindBuffer			ZEDGL_GETFUNC( __zglBindBuffer )
#define zglBufferData			ZEDGL_GETFUNC( __zglBufferData )
#define zglDeleteBuffers		ZEDGL_GETFUNC( __zglDeleteBuffers )
#define zglBindAttribLocation	ZEDGL_GETFUNC( __zglBindAttribLocation )
#define zglVertexAttribPointer	ZEDGL_GETFUNC( __zglVertexAttribPointer )
#define zglEnableVertexAttribArray ZEDGL_GETFUNC( __zglEnableVertexAttribArray )
#define zglGetUniformLocation	ZEDGL_GETFUNC( __zglGetUniformLocation )
#define zglUniformMatrix4fv		ZEDGL_GETFUNC( __zglUniformMatrix4fv )
#define zglBindFragDataLocation	ZEDGL_GETFUNC( __zglBindFragDataLocation )
#define zglUniform1i			ZEDGL_GETFUNC( __zglUniform1i )
#define zglUniform3fv			ZEDGL_GETFUNC( __zglUniform3fv )

///////////////////////////////////////////////////////////////////////////////
// Texture functions ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define zglActiveTexture		ZEDGL_GETFUNC( __zglActiveTexture )
	
#if ( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
#define zglGetProcAddress( p_Proc )	wglGetProcAddress( ( LPCSTR )p_Proc )
#endif

#ifdef __cplusplus
}
#endif

namespace ZED
{
	namespace Renderer
	{
		class GLWExtender
		{
		public:
			ZED_INLINE GLWExtender( ) : m_HDC( NULL ) { }
			explicit GLWExtender( HDC p_HDC );
			~GLWExtender( );
			ZED_INLINE void RegisterHDC( const HDC &p_HDC ) { m_HDC = p_HDC; }
			ZED_BOOL IsGLExtSupported( const char *p_Extension );

			ZED_UINT32 Initialise( ZED_GLVERSION p_Version );

			PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
		private:
			void RegisterBaseWGLExtensions( );

			// The OpenGL version used to determine possible extensions
			ZED_GLVERSION m_GLVersion;

			HDC m_HDC;
			std::list< std::string > m_Extensions;
		};
	}
}

#endif
