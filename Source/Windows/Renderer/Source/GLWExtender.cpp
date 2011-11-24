#include <GLWExtender.hpp>
#include <Renderer.hpp>
#include <Debugger.hpp>

// Define all of the possible OpenGL extensions, initialising them to zero
// OpenGL 2.0 [shaders]
PFNGLCREATESHADERPROC			__zglCreateShader = ZED_NULL;
PFNGLDELETESHADERPROC			__zglDeleteShader = ZED_NULL;
PFNGLSHADERSOURCEPROC			__zglShaderSource = ZED_NULL;
PFNGLCOMPILESHADERPROC			__zglCompileShader = ZED_NULL;
PFNGLGETSHADERINFOLOGPROC		__zglGetShaderInfoLog = ZED_NULL;
PFNGLCREATEPROGRAMPROC			__zglCreateProgram = ZED_NULL;
PFNGLATTACHSHADERPROC			__zglAttachShader = ZED_NULL;
PFNGLDETACHSHADERPROC			__zglDetachShader = ZED_NULL;
PFNGLISSHADERPROC				__zglIsShader = ZED_NULL;
PFNGLGETSHADERIVPROC			__zglGetShaderiv = ZED_NULL;
PFNGLLINKPROGRAMPROC			__zglLinkProgram = ZED_NULL;
PFNGLGETPROGRAMINFOLOGPROC		__zglGetProgramInfoLog = ZED_NULL;
PFNGLUSEPROGRAMPROC				__zglUseProgram = ZED_NULL;
PFNGLDELETEPROGRAMPROC			__zglDeleteProgram = ZED_NULL;
PFNGLISPROGRAMPROC				__zglIsProgram = ZED_NULL;
PFNGLVALIDATEPROGRAMPROC		__zglValidateProgram = ZED_NULL;
PFNGLGETPROGRAMIVPROC			__zglGetProgramiv = ZED_NULL;
PFNGLGENVERTEXARRAYSPROC		__zglGenVertexArrays = ZED_NULL;
PFNGLBINDVERTEXARRAYPROC		__zglBindVertexArray = ZED_NULL;
PFNGLDELETEVERTEXARRAYSPROC		__zglDeleteVertexArrays = ZED_NULL;
PFNGLGENBUFFERSPROC				__zglGenBuffers = ZED_NULL;
PFNGLBINDBUFFERPROC				__zglBindBuffer = ZED_NULL;
PFNGLBUFFERDATAPROC				__zglBufferData = ZED_NULL;
PFNGLDELETEBUFFERSPROC			__zglDeleteBuffers = ZED_NULL;
PFNGLBINDATTRIBLOCATIONPROC		__zglBindAttribLocation = ZED_NULL;
PFNGLVERTEXATTRIBPOINTERPROC	__zglVertexAttribPointer = ZED_NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC __zglEnableVertexAttribArray = ZED_NULL;
PFNGLGETUNIFORMLOCATIONPROC		__zglGetUniformLocation = ZED_NULL;
PFNGLUNIFORMMATRIX4FVPROC		__zglUniformMatrix4fv = ZED_NULL;
//PFNGLBINDFRAGDATALOCATIONPROC	__zglBindFragDataLocation = ZED_NULL;
PFNGLUNIFORM1IPROC				__zglUniform1i = ZED_NULL;
PFNGLUNIFORM3FVPROC				__zglUniform3fv = ZED_NULL;

// OpenGL 2.0 [Textures]
PFNGLACTIVETEXTUREPROC			__zglActiveTexture = ZED_NULL;

namespace ZED
{
	namespace Renderer
	{
		// Create functions for initialising different OpenGL x.x extensions
		static ZED_INT32 zglInitGLVer20( )
		{
			ZED_BOOL RVal = ZED_FALSE;

			RVal = ( ( __zglCreateShader =
				( PFNGLCREATESHADERPROC )zglGetProcAddress(
					"glCreateShader" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglDeleteShader =
				( PFNGLDELETESHADERPROC )zglGetProcAddress(
					"glDeleteShader" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglShaderSource =
				( PFNGLSHADERSOURCEPROC )zglGetProcAddress(
					"glShaderSource" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglCompileShader =
				( PFNGLCOMPILESHADERPROC )zglGetProcAddress(
					"glCompileShader" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGetShaderInfoLog =
				( PFNGLGETSHADERINFOLOGPROC )zglGetProcAddress(
					"glGetShaderInfoLog" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglCreateProgram =
				( PFNGLCREATEPROGRAMPROC )zglGetProcAddress(
					"glCreateProgram" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglAttachShader =
				( PFNGLATTACHSHADERPROC )zglGetProcAddress(
					"glAttachShader" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglDetachShader =
				( PFNGLDETACHSHADERPROC )zglGetProcAddress(
					"glDetachShader" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglIsShader =
				( PFNGLISSHADERPROC )zglGetProcAddress(
					"glIsShader" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGetShaderiv =
				( PFNGLGETSHADERIVPROC )zglGetProcAddress(
					"glGetShaderiv" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglLinkProgram =
				( PFNGLLINKPROGRAMPROC )zglGetProcAddress(
					"glLinkProgram" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGetProgramInfoLog =
				( PFNGLGETPROGRAMINFOLOGPROC )zglGetProcAddress(
					"glGetProgramInfoLog" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglUseProgram =
				( PFNGLUSEPROGRAMPROC )zglGetProcAddress(
					"glUseProgram" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglDeleteProgram =
				( PFNGLDELETEPROGRAMPROC )zglGetProcAddress(
					"glDeleteProgram" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglIsProgram =
				( PFNGLISPROGRAMPROC )zglGetProcAddress(
					"glIsProgram" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglValidateProgram =
				( PFNGLVALIDATEPROGRAMPROC )zglGetProcAddress(
					"glValidateProgram" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGetProgramiv =
				( PFNGLGETPROGRAMIVPROC )zglGetProcAddress(
					"glGetProgramiv" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGenVertexArrays =
				( PFNGLGENVERTEXARRAYSPROC )zglGetProcAddress(
					"glGenVertexArrays" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglBindVertexArray =
				( PFNGLBINDVERTEXARRAYPROC )zglGetProcAddress(
					"glBindVertexArray" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglDeleteVertexArrays =
				( PFNGLDELETEVERTEXARRAYSPROC )zglGetProcAddress(
					"glDeleteVertexArrays" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGenBuffers =
				( PFNGLGENBUFFERSPROC )zglGetProcAddress(
					"glGenBuffers" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglBindBuffer =
				( PFNGLBINDBUFFERPROC )zglGetProcAddress(
					"glBindBuffer" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglBufferData =
				( PFNGLBUFFERDATAPROC )zglGetProcAddress(
					"glBufferData" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglDeleteBuffers =
				( PFNGLDELETEBUFFERSPROC )zglGetProcAddress(
					"glDeleteBuffers" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglBindAttribLocation =
				( PFNGLBINDATTRIBLOCATIONPROC )zglGetProcAddress(
					"glBindAttribLocation" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglVertexAttribPointer =
				( PFNGLVERTEXATTRIBPOINTERPROC )zglGetProcAddress(
					"glVertexAttribPointer" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglEnableVertexAttribArray =
				( PFNGLENABLEVERTEXATTRIBARRAYPROC )zglGetProcAddress(
					"glEnableVertexAttribArray" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglGetUniformLocation =
				( PFNGLGETUNIFORMLOCATIONPROC )zglGetProcAddress(
					"glGetUniformLocation" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglUniformMatrix4fv =
				( PFNGLUNIFORMMATRIX4FVPROC )zglGetProcAddress(
					"glUniformMatrix4fv" ) ) == ZED_NULL ) || RVal;

			/*RVal = ( ( __zglBindFragDataLocation =
				( PFNGLBINDFRAGDATALOCATIONPROC )zglGetProcAddress(
					"glBindFragDataLocation" ) ) == ZED_NULL ) || RVal;*/

			RVal = ( ( __zglUniform1i =
				( PFNGLUNIFORM1IPROC )zglGetProcAddress(
					"glUniform1i" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglUniform3fv =
				( PFNGLUNIFORM3FVPROC )zglGetProcAddress(
					"glUniform3fv" ) ) == ZED_NULL ) || RVal;

			RVal = ( ( __zglActiveTexture =
				( PFNGLACTIVETEXTUREPROC )zglGetProcAddress(
					"glActiveTexture" ) ) == ZED_NULL ) || RVal;

			return ( RVal ? ZED_FAIL : ZED_OK );
		}

		GLWExtender::GLWExtender( HDC p_HDC ) :
			m_HDC( p_HDC )
		{
			wglGetExtensionsStringARB = ZED_NULL;
			wglCreateContextAttribsARB = ZED_NULL;
		}

		GLWExtender::~GLWExtender( )
		{
		}

		ZED_UINT32 GLWExtender::Initialise( const ZED_GLVERSION &p_Version )
		{
			// Check that m_HDC is valid
			if( !m_HDC )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "GLExtender: HDC is not set\n" );
				return ZED_GRAPHICS_ERROR;
			}

			m_GLVersion = p_Version;

			RegisterBaseGLExtensions( );

			// Check that the base WGL extensions were registered
			/*if( wglCreateContextAttribsARB == ZED_NULL ||
				wglGetExtensionsStringARB == ZED_NULL )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "GLExtender | [ERROR] | Failed to register base "
					"WGL extensions\n" );
				return ZED_GRAPHICS_ERROR;
			}*/

			// OPT!
			// Here comes the slow part!

			// Get the list of supported extensions
			const char *WGLExtensions = wglGetExtensionsStringARB( m_HDC );

			ZED_BOOL Loop = ZED_TRUE;

			// Let's hope that no extension exceeds 64 characters!
			char CurrentExtension[ 64 ] = { 0 };
			ZED_UINT64 CharCount = 0;
			ZED_UINT32 Position = 0;

#ifdef ZED_BUILD_DEBUG
					zedTrace( "OpenGL Extensions supported\n" );
#endif

			// Read all the Extensions into m_Extensions
			do
			{
				CurrentExtension[ Position++ ] = WGLExtensions[ CharCount ];

				// Check for the space character
				if( WGLExtensions[ CharCount + 1 ] == 0x20 )
				{
					// The CurrentExtension has bad data at the end, cull it
					std::string CopyString;
					CopyString.insert( 0, CurrentExtension, Position );

					m_Extensions.push_back( CopyString );

					// Print out the extension
#ifdef ZED_BUILD_DEBUG
					zedTrace( "%s\n", CopyString.c_str( ) );
#endif

					// Reset the position to copy into
					Position = 0;

					// Increment the lost char
					CharCount++;
				}

				CharCount++;

				// End of the line
				if( WGLExtensions[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
				
			}while( Loop );
			// !OPT

			if( p_Version.Major >= 2 && p_Version.Minor >= 0 )
			{
				if( zglInitGLVer20( ) != ZED_OK )
				{
					return ZED_GRAPHICS_ERROR;
				}
			}

			return ZED_OK;
		}

		ZED_BOOL GLWExtender::IsSupported( const char *p_Extension )
		{
			// Check that the HDC is valid
			if( m_HDC == ZED_NULL )
			{
				return ZED_FALSE;
			}
			
			ZED_BOOL Return = ZED_FALSE;

			// OPT!
			// Check that the extension can be found
			std::list< std::string >::iterator ExtItr = m_Extensions.begin( );

			// Maybe this should be hashed to speed up the finding?
			while( ( Return == ZED_FALSE ) &&
				( ExtItr != m_Extensions.end( ) ) )
			{
				if( ( *ExtItr ).compare( p_Extension ) == 0 )
				{
					Return = ZED_TRUE;
				}

				ExtItr++;
			}
			// !OPT

			return Return;
		}

		void GLWExtender::RegisterBaseGLExtensions( )
		{
			// OpenGL 1.2 implemented this, so at least 1.2 is supported if
			// this isn't NULL =D
			if( ( ( m_GLVersion.Major >= 1 ) && ( m_GLVersion.Minor >= 2 ) ) ||
				( m_GLVersion.Major >= 2 ) )
			{
				wglGetExtensionsStringARB =
					( PFNWGLGETEXTENSIONSSTRINGARBPROC )
					zglGetProcAddress( "wglGetExtensionsStringARB" );
			}
/*
			// As OpenGL 3.x only supports this (though it can be used with 3.x
			// GPUs for lower profiles), if it is NULL, then it cannot be an
			// OGL 3.x part
			if( ( m_GLVersion.Major >= 3 ) && ( m_GLVersion.Minor >= 0 ) )
			{
				wglCreateContextAttribsARB =
					( PFNWGLCREATECONTEXTATTRIBSARBPROC )
					zglGetProcAddress( "wglCreateContextAttribsARB" );
			}*/
		}
	}
}

