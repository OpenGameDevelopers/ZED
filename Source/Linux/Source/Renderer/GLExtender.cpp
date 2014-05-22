#include <Renderer/OGL/GLExtender.hpp>

///////////////////////////////////////////////////////////////////////////////
// GLX Extensions /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
PFNGLXCREATECONTEXTATTRIBSARBPROC	__zglCreateContextAttribsARB = ZED_NULL;

///////////////////////////////////////////////////////////////////////////////
// OpenGL 2.0 Extensions //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

PFNGLGETSTRINGIPROC					__zglGetStringi = ZED_NULL;
PFNGLBLENDFUNCSEPARATEPROC			__zglBlendFuncSeparate = ZED_NULL;
PFNGLBLENDEQUATIONSEPARATEPROC		__zglBlendEquationSeparate = ZED_NULL;

// Define all the possible OpenGL extensions, initialising them to zero
// OpenGL 2.0 [Shaders]
PFNGLCREATESHADERPROC				__zglCreateShader = ZED_NULL;
PFNGLDELETESHADERPROC				__zglDeleteShader = ZED_NULL;
PFNGLSHADERSOURCEPROC				__zglShaderSource = ZED_NULL;
PFNGLCOMPILESHADERPROC				__zglCompileShader = ZED_NULL;
PFNGLGETSHADERINFOLOGPROC			__zglGetShaderInfoLog = ZED_NULL;
PFNGLCREATEPROGRAMPROC				__zglCreateProgram = ZED_NULL;
PFNGLATTACHSHADERPROC				__zglAttachShader = ZED_NULL;
PFNGLDETACHSHADERPROC				__zglDetachShader = ZED_NULL;
PFNGLISSHADERPROC					__zglIsShader = ZED_NULL;
PFNGLGETSHADERIVPROC				__zglGetShaderiv = ZED_NULL;
PFNGLLINKPROGRAMPROC				__zglLinkProgram = ZED_NULL;
PFNGLGETPROGRAMINFOLOGPROC			__zglGetProgramInfoLog = ZED_NULL;
PFNGLUSEPROGRAMPROC					__zglUseProgram = ZED_NULL;
PFNGLDELETEPROGRAMPROC				__zglDeleteProgram = ZED_NULL;
PFNGLISPROGRAMPROC					__zglIsProgram = ZED_NULL;
PFNGLVALIDATEPROGRAMPROC			__zglValidateProgram = ZED_NULL;
PFNGLGETPROGRAMIVPROC				__zglGetProgramiv = ZED_NULL;
PFNGLGENVERTEXARRAYSPROC			__zglGenVertexArrays = ZED_NULL;
PFNGLBINDVERTEXARRAYPROC			__zglBindVertexArray = ZED_NULL;
PFNGLDELETEVERTEXARRAYSPROC			__zglDeleteVertexArrays = ZED_NULL;
PFNGLGENBUFFERSPROC					__zglGenBuffers = ZED_NULL;
PFNGLBINDBUFFERPROC					__zglBindBuffer = ZED_NULL;
PFNGLBUFFERDATAPROC					__zglBufferData = ZED_NULL;
PFNGLBUFFERSUBDATAPROC				__zglBufferSubData = ZED_NULL;
PFNGLDELETEBUFFERSPROC				__zglDeleteBuffers = ZED_NULL;
PFNGLBINDATTRIBLOCATIONPROC			__zglBindAttribLocation = ZED_NULL;
PFNGLVERTEXATTRIBPOINTERPROC		__zglVertexAttribPointer = ZED_NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC	__zglEnableVertexAttribArray = ZED_NULL;
PFNGLGETACTIVEUNIFORMSIVPROC		__zglGetActiveUniformsiv = ZED_NULL;
PFNGLGETUNIFORMLOCATIONPROC			__zglGetUniformLocation = ZED_NULL;
PFNGLUNIFORMMATRIX2FVPROC			__zglUniformMatrix2fv = ZED_NULL;
PFNGLUNIFORMMATRIX3FVPROC			__zglUniformMatrix3fv = ZED_NULL;
PFNGLUNIFORMMATRIX4FVPROC			__zglUniformMatrix4fv = ZED_NULL;
PFNGLBINDFRAGDATALOCATIONPROC		__zglBindFragDataLocation = ZED_NULL;
PFNGLUNIFORM1IPROC					__zglUniform1i = ZED_NULL;
PFNGLUNIFORM2IVPROC					__zglUniform2iv = ZED_NULL;
PFNGLUNIFORM3IVPROC					__zglUniform3iv = ZED_NULL;
PFNGLUNIFORM4IVPROC					__zglUniform4iv = ZED_NULL;
PFNGLUNIFORM1FPROC					__zglUniform1f = ZED_NULL;
PFNGLUNIFORM2FVPROC					__zglUniform2fv = ZED_NULL;
PFNGLUNIFORM3FVPROC					__zglUniform3fv = ZED_NULL;
PFNGLUNIFORM4FVPROC					__zglUniform4fv = ZED_NULL;

// OpenGL 2.0 [Textures]
PFNGLACTIVETEXTUREPROC				__zglActiveTexture = ZED_NULL;
PFNGLBINDTEXTUREEXTPROC				__zglBindTexture = ZED_NULL;
PFNGLDELETETEXTURESEXTPROC			__zglDeleteTextures = ZED_NULL;
PFNGLGENTEXTURESEXTPROC				__zglGenTextures = ZED_NULL;
PFNGLTEXSTORAGE2DPROC				__zglTexStorage2D = ZED_NULL;
PFNGLTEXSUBIMAGE2DEXTPROC			__zglTexSubImage2D = ZED_NULL;

PFNGLMAPBUFFERPROC					__zglMapBuffer = ZED_NULL;
PFNGLMAPBUFFERRANGEPROC				__zglMapBufferRange = ZED_NULL;
PFNGLUNMAPBUFFERPROC				__zglUnmapBuffer = ZED_NULL;

namespace ZED
{
	namespace Renderer
	{
		// Initialise all OpenGL 2.0 extensions
		static ZED_UINT32 zglInitGLVer20( )
		{
			ZED_BOOL Ret = ZED_FALSE;
			ZED_BOOL TemporaryStatus = ZED_FALSE;

			Ret = ( ( __zglBlendFuncSeparate =
				( PFNGLBLENDFUNCSEPARATEPROC )zglGetProcAddress(
					"glBlendFuncSeparate" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBlendEquationSeparate =
				( PFNGLBLENDEQUATIONSEPARATEPROC )zglGetProcAddress(
					"glBlendEquationSeparate" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglCreateShader =
				( PFNGLCREATESHADERPROC )zglGetProcAddress(
					"glCreateShader" ) ) == ZED_NULL ) || Ret;
			
			Ret = ( ( __zglDeleteShader =
				( PFNGLDELETESHADERPROC )zglGetProcAddress(
					"glDeleteShader" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglShaderSource =
				( PFNGLSHADERSOURCEPROC )zglGetProcAddress( 
					"glShaderSource" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglCompileShader =
				( PFNGLCOMPILESHADERPROC )zglGetProcAddress(
					"glCompileShader" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGetShaderInfoLog =
				( PFNGLGETSHADERINFOLOGPROC )zglGetProcAddress(
					"glGetShaderInfoLog" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglCreateProgram =
				( PFNGLCREATEPROGRAMPROC )zglGetProcAddress(
					"glCreateProgram" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglAttachShader =
				( PFNGLATTACHSHADERPROC )zglGetProcAddress(
					"glAttachShader" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglDetachShader =
				( PFNGLDETACHSHADERPROC )zglGetProcAddress(
					"glDetachShader" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglIsShader =
				( PFNGLISSHADERPROC )zglGetProcAddress(
					"glIsShader" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGetShaderiv =
				( PFNGLGETSHADERIVPROC )zglGetProcAddress(
					"glGetShaderiv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglLinkProgram =
				( PFNGLLINKPROGRAMPROC )zglGetProcAddress(
					"glLinkProgram" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGetProgramInfoLog =
				( PFNGLGETPROGRAMINFOLOGPROC )zglGetProcAddress(
					"glGetProgramInfoLog" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUseProgram =
				( PFNGLUSEPROGRAMPROC )zglGetProcAddress(
					"glUseProgram" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglDeleteProgram =
				( PFNGLDELETEPROGRAMPROC )zglGetProcAddress(
					"glDeleteProgram" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglIsProgram =
				( PFNGLISPROGRAMPROC )zglGetProcAddress(
					"glIsProgram" ) ) == ZED_NULL ) || Ret;
			
			Ret = ( ( __zglValidateProgram =
				( PFNGLVALIDATEPROGRAMPROC )zglGetProcAddress(
					"glValidateProgram" ) ) == ZED_NULL ) || Ret;
			
			Ret = ( ( __zglGetProgramiv =
				( PFNGLGETPROGRAMIVPROC )zglGetProcAddress(
					"glGetProgramiv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGenVertexArrays =
				( PFNGLGENVERTEXARRAYSPROC )zglGetProcAddress(
					"glGenVertexArrays" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBindVertexArray =
				( PFNGLBINDVERTEXARRAYPROC )zglGetProcAddress(
					"glBindVertexArray" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglDeleteVertexArrays =
				( PFNGLDELETEVERTEXARRAYSPROC )zglGetProcAddress(
					"glDeleteVertexArrays" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGenBuffers =
				( PFNGLGENBUFFERSPROC )zglGetProcAddress(
					"glGenBuffers" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBindBuffer =
				( PFNGLBINDBUFFERPROC )zglGetProcAddress(
					"glBindBuffer" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBufferData =
				( PFNGLBUFFERDATAPROC )zglGetProcAddress(
					"glBufferData" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBufferSubData =
				( PFNGLBUFFERSUBDATAPROC )zglGetProcAddress(
					"glBufferSubData" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglDeleteBuffers =
				( PFNGLDELETEBUFFERSPROC )zglGetProcAddress(
					"glDeleteBuffers" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBindAttribLocation =
				( PFNGLBINDATTRIBLOCATIONPROC )zglGetProcAddress(
					"glBindAttribLocation" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglVertexAttribPointer =
				( PFNGLVERTEXATTRIBPOINTERPROC )zglGetProcAddress(
					"glVertexAttribPointer" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglEnableVertexAttribArray =
				( PFNGLENABLEVERTEXATTRIBARRAYPROC )zglGetProcAddress(
					"glEnableVertexAttribArray" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGetActiveUniformsiv =
				( PFNGLGETACTIVEUNIFORMSIVPROC )zglGetProcAddress(
					"glGetActiveUniformsiv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglGetUniformLocation =
				( PFNGLGETUNIFORMLOCATIONPROC )zglGetProcAddress(
					"glGetUniformLocation" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniformMatrix2fv =
				( PFNGLUNIFORMMATRIX2FVPROC )zglGetProcAddress(
					"glUniformMatrix2fv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniformMatrix3fv =
				( PFNGLUNIFORMMATRIX3FVPROC )zglGetProcAddress(
					"glUniformMatrix3fv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniformMatrix4fv =
				( PFNGLUNIFORMMATRIX4FVPROC )zglGetProcAddress(
					"glUniformMatrix4fv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglBindFragDataLocation =
				( PFNGLBINDFRAGDATALOCATIONPROC )zglGetProcAddress(
					"glBindFragDataLocation" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform1i =
				( PFNGLUNIFORM1IPROC )zglGetProcAddress(
					"glUniform1i" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform2iv =
				( PFNGLUNIFORM2IVPROC )zglGetProcAddress(
					"glUniform2iv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform3iv =
				( PFNGLUNIFORM3IVPROC )zglGetProcAddress(
					"glUniform3iv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform4iv =
				( PFNGLUNIFORM4IVPROC )zglGetProcAddress(
					"glUniform4iv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform1f =
				( PFNGLUNIFORM1FPROC )zglGetProcAddress(
					"glUniform1f" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform2fv =
				( PFNGLUNIFORM2FVPROC )zglGetProcAddress(
					"glUniform2fv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform3fv =
				( PFNGLUNIFORM3FVPROC )zglGetProcAddress(
					"glUniform3fv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUniform4fv =
				( PFNGLUNIFORM4FVPROC )zglGetProcAddress(
					"glUniform4fv" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglActiveTexture =
				( PFNGLACTIVETEXTUREPROC )zglGetProcAddress(
					"glActiveTexture" ) ) == ZED_NULL ) || Ret;

			TemporaryStatus = ( ( __zglBindTexture =
				( PFNGLBINDTEXTUREEXTPROC )zglGetProcAddress(
					"glBindTexture" ) ) == ZED_NULL );

			if( TemporaryStatus )
			{
				Ret = ( ( __zglBindTexture =
					( PFNGLBINDTEXTUREEXTPROC )zglGetProcAddress(
						"glBindTextureEXT" ) ) == ZED_NULL ) || Ret;
			}

			TemporaryStatus = ( ( __zglDeleteTextures =
				( PFNGLDELETETEXTURESEXTPROC )zglGetProcAddress(
					"glDeleteTextures" ) ) == ZED_NULL );

			if( TemporaryStatus )
			{
				Ret = ( ( __zglDeleteTextures =
					( PFNGLDELETETEXTURESEXTPROC )zglGetProcAddress(
						"glDeleteTexturesEXT" ) ) == ZED_NULL ) || Ret;
			}

			TemporaryStatus = ( ( __zglGenTextures =
				( PFNGLGENTEXTURESEXTPROC )zglGetProcAddress(
					"glGenTextures" ) ) == ZED_NULL );

			if( TemporaryStatus )
			{
				Ret = ( ( __zglGenTextures =
					( PFNGLGENTEXTURESEXTPROC )zglGetProcAddress(
						"glGenTexturesEXT" ) ) == ZED_NULL ) || Ret;
			}


			Ret = ( ( __zglTexStorage2D =
				( PFNGLTEXSTORAGE2DPROC )zglGetProcAddress(
					"glTexStorage2D" ) ) == ZED_NULL ) || Ret;

			TemporaryStatus = ( ( __zglTexSubImage2D =
				( PFNGLTEXSUBIMAGE2DEXTPROC )zglGetProcAddress(
					"glTexSubImage2D" ) ) == ZED_NULL );

			if( TemporaryStatus )
			{
				Ret = ( ( __zglTexSubImage2D =
					( PFNGLTEXSUBIMAGE2DEXTPROC )zglGetProcAddress(
						"glTexSubImage2DEXT" ) ) == ZED_NULL ) || Ret;
			}

			Ret = ( ( __zglMapBuffer =
				( PFNGLMAPBUFFERPROC )zglGetProcAddress( "glMapBuffer" ) ) ==
					ZED_NULL ) || Ret;
			
			Ret = ( ( __zglMapBufferRange =
				( PFNGLMAPBUFFERRANGEPROC )zglGetProcAddress(
					"glMapBufferRange" ) ) == ZED_NULL ) || Ret;

			Ret = ( ( __zglUnmapBuffer =
				( PFNGLUNMAPBUFFERPROC )zglGetProcAddress(
					"glUnmapBuffer" ) ) == ZED_NULL ) || Ret;

			return ( Ret ? ZED_FAIL : ZED_OK );
		}

		GLExtender::GLExtender( const ZED::System::WINDOWDATA &p_WindowData ) :
			m_WindowData( p_WindowData )
		{
		}

		GLExtender::~GLExtender( )
		{
		}

		ZED_BOOL GLExtender::IsGLExtensionSupported(
			const char *p_pGLExtensionQuery )
		{
			if( m_Extensions.size( ) == 0 )
			{
				zedTrace( "[ZED::Renderer::GLExtender::IsSupported] <ERROR> "
					"Failed to get extensions.\n" );
				zedAssert( ZED_FALSE );

				return ZED_FALSE;
			}

			ZED_BOOL Ret = ZED_FALSE;

			std::list< std::string >::iterator ExtItr = m_Extensions.begin( );
			
			while( ( Ret == ZED_FALSE ) && ( ExtItr != m_Extensions.end( ) ) )
			{
				if( ( *ExtItr ).compare( p_pGLExtensionQuery ) == 0 )
				{
					Ret = ZED_TRUE;
				}

				ExtItr++;
			}

			return Ret;
		}
		
		ZED_BOOL GLExtender::IsWindowExtensionSupported(
			const char *p_pWindowExtensionQuery )
		{
			if( m_WindowExtensions.size( ) == 0 )
			{
				zedTrace( "[ZED::Renderer::GLExtender::IsWindowExtSupported "
					"<ERROR> Failed to get extensions\n" );
				zedAssert( ZED_FALSE );
				return ZED_FALSE;
			}
			
			ZED_BOOL Ret = ZED_FALSE;
			std::list< std::string >::iterator Itr =
				m_WindowExtensions.begin( );

			while( ( Ret == ZED_FALSE ) &&
				( Itr != m_WindowExtensions.end( ) ) )
			{
				if( ( *Itr ).compare( p_pWindowExtensionQuery ) == 0 )
				{
					Ret = ZED_TRUE;
				}

				Itr++;				
			}

			return Ret;
		}

		ZED_UINT32 GLExtender::Initialise( const ZED_GLVERSION &p_Version )
		{
			zedTrace( "VER in %p Ver attrib. %p\n", &p_Version, &m_GLVersion );
			m_GLVersion.Major = p_Version.Major;
			m_GLVersion.Minor = p_Version.Minor;

			// If the OpenGL version is greater than 3.0, use glGetStringi
			if( m_GLVersion.Major >= 3 )
			{
				// Initialise zglGetStringi
				__zglGetStringi = ( PFNGLGETSTRINGIPROC )zglGetProcAddress(
					"glGetStringi" );
				if( __zglGetStringi == ZED_NULL )
				{
					// Something went wrong!
					zedAssert( ZED_FALSE );
					zedTrace( "[ZED::Renderer::GLExtender::Initialise] "
						"<ERROR> Failed to get zglGetString\n" );
					return ZED_FAIL;
				}
				
				ZED_SINT32 NumExtensions = 0;
				zglGetIntegerv( GL_NUM_EXTENSIONS, &NumExtensions );
				
				zedTrace( "[ZED::Renderer::GLExtender::Initialise] <INFO> "
					"%d OpenGL Extensions supported:\n", NumExtensions );
				// Store all extensions in the extensions list
				for( ZED_SINT32 i = 0; i < NumExtensions; i++ )
				{
					std::string GLExt(
						( char* )zglGetStringi( GL_EXTENSIONS, i ) );
					m_Extensions.push_back( GLExt );
					zedTrace( "%s\n", GLExt.c_str( ) );
				}
			}
			// Have to use OGL <=2.x method
			else
			{
				ZED_SINT32 NumExtensions = 0;
				zglGetIntegerv( GL_NUM_EXTENSIONS, &NumExtensions );

				zedTrace( "[ZED::Renderer::GLExtender::Initialise] <INFO> "
					"%d OpenGL Extensions supported:\n", NumExtensions );

				char CurExt[ 64 ] = { '\0' };
				ZED_MEMSIZE Position = 0, CharCount = 0;
				ZED_BOOL Loop = ZED_TRUE;
				const char *pGLExtensions =
					( const char * )glGetString( GL_EXTENSIONS );

				do
				{
					CurExt[ Position++ ] = pGLExtensions[ CharCount ];

					if( pGLExtensions[ CharCount+1 ] == 0x20 )
					{
						std::string CopyString;
						CopyString.insert( 0, CurExt, Position );

						m_Extensions.push_back( CopyString );

						zedTrace( "%s\n", CopyString.c_str( ) );

						Position = 0;

						CharCount++;
					}

					CharCount++;

					if( pGLExtensions[ CharCount ] == 0x00 )
					{
						Loop = ZED_FALSE;
					}

				}while( Loop );
			}

			if( m_GLVersion.Major >= 2 )
			{
				if( zglInitGLVer20( ) != ZED_OK )
				{
					return ZED_GRAPHICS_ERROR;
				}
			}

			this->InitialiseWindowExtensions( );


			return ZED_OK;
		}

		ZED_UINT32 GLExtender::InitialiseWindowExtensions( )
		{
			// Process the string
			const char *pWinExt = 
				glXQueryExtensionsString( m_WindowData.pX11Display,
					DefaultScreen( m_WindowData.pX11Display ) );

			ZED_UINT32 NumExtensions = 0;

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> Getting GLX Extensions.\n" );

			char CurExt[ 64 ] = { '\0' };
			ZED_MEMSIZE Position = 0, CharCount = 0;
			ZED_BOOL Loop = ZED_TRUE;

			do
			{
				CurExt[ Position++ ] = pWinExt[ CharCount ];

				if( pWinExt[ CharCount+1 ] == 0x20 )
				{
					std::string Copy;
					Copy.insert( 0, CurExt, Position );

					m_WindowExtensions.push_back( Copy );

					zedTrace( "%s\n", Copy.c_str( ) );

					Position = 0;
					NumExtensions++;
					CharCount++;
				}

				CharCount++;

				if( pWinExt[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
			}while( Loop );

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> %d GLX Extensions available.\n", NumExtensions );

			pWinExt = 
				glXGetClientString( m_WindowData.pX11Display, GLX_EXTENSIONS );

			NumExtensions = 0;

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> Getting Client GLX Extensions.\n" );

			Position = 0;
			CharCount = 0;
			Loop = ZED_TRUE;

			do
			{
				CurExt[ Position++ ] = pWinExt[ CharCount ];

				if( pWinExt[ CharCount+1 ] == 0x20 )
				{
					std::string Copy;
					Copy.insert( 0, CurExt, Position );

					m_WindowExtensions.push_back( Copy );

					zedTrace( "%s\n", Copy.c_str( ) );

					Position = 0;
					NumExtensions++;
					CharCount++;
				}

				CharCount++;

				if( pWinExt[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
			}while( Loop );

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> %d Client GLX Extensions available.\n",
				NumExtensions );

			pWinExt = glXQueryServerString( m_WindowData.pX11Display,
					DefaultScreen( m_WindowData.pX11Display ),
					GLX_EXTENSIONS );

			NumExtensions = 0;

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> Getting Server GLX Extensions.\n" );

			Position = 0;
			CharCount = 0;
			Loop = ZED_TRUE;

			do
			{
				CurExt[ Position++ ] = pWinExt[ CharCount ];

				if( pWinExt[ CharCount+1 ] == 0x20 )
				{
					std::string Copy;
					Copy.insert( 0, CurExt, Position );

					m_WindowExtensions.push_back( Copy );

					zedTrace( "%s\n", Copy.c_str( ) );

					Position = 0;
					NumExtensions++;
					CharCount++;
				}

				CharCount++;

				if( pWinExt[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
			}while( Loop );

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> %d Server GLX Extensions available.\n",
				NumExtensions );

			return ZED_OK;
		}

		ZED_UINT32 GLExtender::RegisterBaseGLExtensions( )
		{
			// Just register the CreateContextAttribsARB
			return ZED_OK;
		}
	}
}

