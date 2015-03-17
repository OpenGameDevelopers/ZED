#ifndef __ZED_RENDERER_GLEXTENDER_HPP__
#define __ZED_RENDERER_GLEXTENDER_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/Renderer.hpp>
#include <System/WindowData.hpp>
// The STL list will be replaced by ZED::System::List
#include <list>

// Helper macro for determining the offset to use for GL buffers
#define ZGL_BUFFER_OFFSET( p_Offset )( ( ZED_BYTE * )NULL + ( p_Offset ) )

// Define the OpenGL extensions here
#ifdef __cplusplus
extern "C"
{
#endif

extern PFNGLGETSTRINGIPROC				__zglGetStringi;
extern PFNGLBLENDFUNCSEPARATEPROC		__zglBlendFuncSeparate;
extern PFNGLBLENDEQUATIONSEPARATEPROC	__zglBlendEquationSeparate;

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
extern PFNGLBUFFERSUBDATAPROC			__zglBufferSubData;
extern PFNGLDELETEBUFFERSPROC			__zglDeleteBuffers;
extern PFNGLBINDATTRIBLOCATIONPROC		__zglBindAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC		__zglVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC __zglEnableVertexAttribArray;
extern PFNGLGETUNIFORMLOCATIONPROC		__zglGetUniformLocation;
extern PFNGLUNIFORMMATRIX2FVPROC		__zglUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC		__zglUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC		__zglUniformMatrix4fv;
extern PFNGLBINDFRAGDATALOCATIONPROC	__zglBindFragDataLocation;
extern PFNGLUNIFORM1IPROC				__zglUniform1i;
extern PFNGLUNIFORM2IVPROC				__zglUniform2iv;
extern PFNGLUNIFORM3IVPROC				__zglUniform3iv;
extern PFNGLUNIFORM4IVPROC				__zglUniform4iv;
extern PFNGLUNIFORM1FPROC				__zglUniform1f;
extern PFNGLUNIFORM2FVPROC				__zglUniform2fv;
extern PFNGLUNIFORM3FVPROC				__zglUniform3fv;
extern PFNGLUNIFORM4FVPROC				__zglUniform4fv;

extern PFNGLACTIVETEXTUREPROC			__zglActiveTexture;
extern PFNGLBINDTEXTUREEXTPROC			__zglBindTexture;
extern PFNGLDELETETEXTURESEXTPROC		__zglDeleteTextures;
extern PFNGLGENTEXTURESEXTPROC			__zglGenTextures;
extern PFNGLTEXSTORAGE2DPROC			__zglTexStorage2D;
extern PFNGLTEXSUBIMAGE2DEXTPROC		__zglTexSubImage2D;

extern PFNGLMAPBUFFERPROC				__zglMapBuffer;
extern PFNGLMAPBUFFERRANGEPROC			__zglMapBufferRange;
extern PFNGLGETBUFFERPARAMETERIVPROC	__zglGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC		__zglGetBufferPointerv;
extern PFNGLUNMAPBUFFERPROC				__zglUnmapBuffer;

// It's not clean, but it works!
#define ZEDGL_GETFUNC( x )	x
// Declare all OpenGL functions via macros and grab them upon initialisation
#define zglGetStringi			ZEDGL_GETFUNC( __zglGetStringi )

///////////////////////////////////////////////////////////////////////////////
// Core Functions /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define zglGetString				glGetString
#define zglGetError					glGetError
#define zglDrawElements				glDrawElements
#define zglClear					glClear
#define zglClearColor				glClearColor
#define zglGetIntegerv				glGetIntegerv
#define zglViewport					glViewport
#define zglEnable					glEnable
#define zglDisable					glDisable
#define zglFrontFace				glFrontFace
#define zglCullFace					glCullFace
#define zglReadBuffer				glReadBuffer
#define zglReadPixels				glReadPixels
#define zglBlendEquationSeparate	ZEDGL_GETFUNC( __zglBlendEquationSeparate )
#define zglBlendFuncSeparate		ZEDGL_GETFUNC( __zglBlendFuncSeparate )

///////////////////////////////////////////////////////////////////////////////
// Shader Functions ///////////////////////////////////////////////////////////
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
#define zglBufferSubData		ZEDGL_GETFUNC( __zglBufferSubData )
#define zglDeleteBuffers		ZEDGL_GETFUNC( __zglDeleteBuffers )
#define zglBindAttribLocation	ZEDGL_GETFUNC( __zglBindAttribLocation )
#define zglVertexAttribPointer	ZEDGL_GETFUNC( __zglVertexAttribPointer )
#define zglEnableVertexAttribArray ZEDGL_GETFUNC( __zglEnableVertexAttribArray )
#define zglGetUniformLocation	ZEDGL_GETFUNC( __zglGetUniformLocation )
#define zglUniformMatrix2fv		ZEDGL_GETFUNC( __zglUniformMatrix2fv )
#define zglUniformMatrix3fv		ZEDGL_GETFUNC( __zglUniformMatrix3fv )
#define zglUniformMatrix4fv		ZEDGL_GETFUNC( __zglUniformMatrix4fv )
#define zglBindFragDataLocation	ZEDGL_GETFUNC( __zglBindFragDataLocation )
#define zglUniform1i			ZEDGL_GETFUNC( __zglUniform1i )
#define zglUniform2iv			ZEDGL_GETFUNC( __zglUniform2iv )
#define zglUniform3iv			ZEDGL_GETFUNC( __zglUniform3iv )
#define zglUniform4iv			ZEDGL_GETFUNC( __zglUniform4iv )
#define zglUniform1f			ZEDGL_GETFUNC( __zglUniform1f )
#define zglUniform2fv			ZEDGL_GETFUNC( __zglUniform2fv )
#define zglUniform3fv			ZEDGL_GETFUNC( __zglUniform3fv )
#define zglUniform4fv			ZEDGL_GETFUNC( __zglUniform4fv )

///////////////////////////////////////////////////////////////////////////////
// Texture functions //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define zglActiveTexture		ZEDGL_GETFUNC( __zglActiveTexture )
#define zglBindTexture			ZEDGL_GETFUNC( __zglBindTexture )
#define zglDeleteTextures		ZEDGL_GETFUNC( __zglDeleteTextures )
#define zglGenTextures			ZEDGL_GETFUNC( __zglGenTextures )
#define zglTexStorage2D			ZEDGL_GETFUNC( __zglTexStorage2D )
#define zglTexSubImage2D		ZEDGL_GETFUNC( __zglTexSubImage2D )

#define zglMapBuffer			ZEDGL_GETFUNC( __zglMapBuffer )
#define zglMapBufferRange		ZEDGL_GETFUNC( __zglMapBufferRange )
#define zglUnmapBuffer			ZEDGL_GETFUNC( __zglUnmapBuffer )

#if ( ZED_PLATFORM_WINDOWS )
#define zglGetProcAddress( p_Proc )	wglGetProcAddress( ( LPCSTR )p_Proc )
extern PFNWGLGETEXTENSIONSSTRINGARBPROC __zglGetExtensionsString;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC __zglCreateContextAttribs;

#define zglGetExtensionsString	ZEDGL_GETFUNC( __zglGetExtensionsString )
#define zglCreateContextAttribs	ZEDGL_GETFUNC( __zglCreateContextAttribs )
#elif ( ZED_PLATFORM_LINUX )
#define zglGetProcAddress( p_Proc )\
	glXGetProcAddressARB( ( const GLubyte * )p_Proc )
extern PFNGLXCREATECONTEXTATTRIBSARBPROC __zglCreateContextAttribsARB;
#endif

#ifdef __cplusplus
}
#endif

namespace ZED
{
	namespace Renderer
	{
		class GLExtender
		{
		public:
			virtual ~GLExtender( );

			ZED_UINT32 Initialise( const ZED_GLVERSION &p_Version );

			ZED_BOOL IsGLExtensionSupported( const char *p_Extension );
			ZED_BOOL IsWindowExtensionSupported(
				const char *p_WindowExtension );

		protected:
			virtual ZED_UINT32 InitialiseWindowExtensions( ) = 0;
			ZED_UINT32 SetupGLExtensions( );

			ZED_GLVERSION				m_GLVersion;
			std::list< std::string >	m_Extensions;
			std::list< std::string >	m_WindowExtensions;
		};
	}
}

#endif // __ZED_RENDERER_GLEXTENDER_HPP__

