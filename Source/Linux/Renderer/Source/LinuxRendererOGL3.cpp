#include <LinuxRendererOGL3.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		LinuxRendererOGL3::LinuxRendererOGL3( )
		{
			m_pDisplay = ZED_NULL;
			m_pScreen = ZED_NULL;
		}

		LinuxRendererOGL3::~LinuxRendererOGL3( )
		{
			if( m_pDisplay )
			{
				glXMakeCurrent( m_pDisplay, 0, 0 );
			}

			if( m_pDisplay && m_GLContext )
			{
				glXDestroyContext( m_pDisplay, m_GLContext );
			}

			// MOVE THESE OUT OF HERE.
			// LET THE APPLICATION HANDLE IT

			if( m_pDisplay && m_Window )
			{
				XDestroyWindow( m_pDisplay, m_Window );
			}

			if( m_pDisplay && m_ColMap )
			{
				XFreeColormap( m_pDisplay, m_ColMap );
			}

			if( m_pDisplay )
			{
				XCloseDisplay( m_pDisplay );
			}
		}

		ZED_UINT32 LinuxRendererOGL3::Create( GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas )
		{
			if( m_pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Display not initialised\n");
				return ZED_FAIL;
			}

			ZED_INT32 GLXMajor = 0, GLXMinor = 0;
			m_Canvas = p_Canvas;

			// Put the canvas' colour, depth, and stencil formats converted
			// into a format that can be consumed by the VA
			ZED_INT32 Depth = 0, Stencil = 0, A = 0, R = 0, G = 0, B = 0;
			switch( m_Canvas.GetDepthStencil( ) )
			{
			case ZED_FORMAT_D24S8:
				{
					Depth = 24;
					Stencil = 8;
					break;
				}
			default:
				{
					break;
				}
			}

			switch( m_Canvas.GetBPP( ) )
			{
				case ZED_FORMAT_ARGB8:
				{
					A = 8;
					R = 8;
					G = 8;
					B = 8;
					break;
				}
				case ZED_FORMAT_RGB565:
				{
					A = 0;
					R = 5;
					G = 6;
					B = 5;
				}
				default:
				{
					break;
				}
			}
	
			ZED_INT32 AntiAliasing = 0, AntiAliasingCount = 0;
			switch( m_Canvas.GetAntiAliasingType( ) )
			{
			case ZED_SAMPLE_TYPE_MSAA_4:
				{
					AntiAliasing = 1;
					AntiAliasingCount = 4;
					break;
				}
			default:
				{
					break;
				}
			}

			int VA[ ] =
			{
				GLX_X_RENDERABLE,	True,
				GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
				GLX_RENDER_TYPE,	GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
				GLX_RED_SIZE,		R,
				GLX_GREEN_SIZE,		G,
				GLX_BLUE_SIZE,		B,
				GLX_ALPHA_SIZE,		A,
				GLX_DEPTH_SIZE,		Depth,
				GLX_STENCIL_SIZE,	Stencil,
				GLX_DOUBLEBUFFER, ( m_Canvas.GetBackBufferCount( ) > 0 ?
					True : False ),
				GLX_SAMPLE_BUFFERS,	AntiAliasing,
				GLX_SAMPLES,		AntiAliasingCount,/*
				GLX_COVERAGE_SAMPLES_NV,	CSCoverSamples,
				GLX_COLOR_SAMPLES_NV,		CSColourSamples,*/
				None
			};

			// Check if GLX version is 1.3 or greater for FBConfig
			if( !glXQueryVersion( m_pDisplay, &GLXMajor, &GLXMinor ) ||
				( ( GLXMajor == 1 ) && ( GLXMinor < 3 ) ) || ( GLXMajor < 1 ) )
			{
				zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <ERROR> "
					"Invalid GLX version: %d.%d\n"
					"\tRequire at least version 1.3\n", GLXMajor, GLXMinor );
				return ZED_FAIL;
			}

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Getting framebuffer configurations\n" );
			ZED_INT32 FBCount;
			GLXFBConfig *pFBC = glXChooseFBConfig( m_pDisplay,
				DefaultScreen( m_pDisplay ), VA, &FBCount );

			if( !pFBC )
			{
				zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <ERROR> "
					"Failed to retrieve a framebuffer configuration.\n" );
				return ZED_FAIL;
			}

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Found %d matching configurations\n", FBCount );
			
			// Depending on the amount of samples the canvas wants, try and get the best
			// framebuffer for it
			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				"Getting Visual Information with %d samples.\n",
				AntiAliasingCount );
/*
			ZED_INT32 BestFBC = -1, WorstFBC = -1;
			ZED_INT32 BestSampleCount = -1, WorstSampleCount = -1;

			for( ZED_MEMSIZE i = 0; i < FBCount; i++ )
			{
				XVisualInfo *pVI = glXGetVisualFromFBConfig( m_pDisplay,
					pFBC[ i ] );

				if( pVI )
				{
					ZED_INT32 SampleBuffer, Samples;
					glXGetFBConfigAttrib( m_pDisplay, pFBC[ i ], GLX_SAMPLE_BUFFERS,
						&SampleBuffer );
					glXGetFBConfigAttrib( m_pDisplay, pFBC[ i ], GLX_SAMPLES,
						&Samples );

					zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
						"Matching Framebuffer Configuration: %d | "
						"Visual ID: 0x%2X:\n\t"
						"Sample Buffers = %d\n\tSamples = %d\n",
						i, pVI->visualid, SampleBuffer, Samples);

					if( BestFBC <
				}

			}*/

			GLXFBConfig GLFBConf = pFBC[ 0 ];

			XFree( pFBC );

			// Get a visual
			XVisualInfo *pVI = glXGetVisualFromFBConfig( m_pDisplay, GLFBConf );
			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Visual ID = 0x%X\n", pVI->visualid );

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Setting up colourmap.\n" );

			// MOVE!
			XSetWindowAttributes WinAttrib;

			m_ColMap = XCreateColormap( m_pDisplay,
				RootWindow( m_pDisplay, pVI->screen ), pVI->visual,
				AllocNone );

			WinAttrib.colormap = m_ColMap;			
			WinAttrib.background_pixmap = None;
			WinAttrib.border_pixel = 0;
			WinAttrib.event_mask = StructureNotifyMask|ExposureMask|KeyPressMask|ButtonPressMask;

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Setting up window.\n" );

			m_Window = XCreateWindow( m_pDisplay,
				RootWindow( m_pDisplay, pVI->screen ), 0, 0,
				m_Canvas.GetWidth( ), m_Canvas.GetHeight( ),
				0, pVI->depth, InputOutput, pVI->visual,
				CWBorderPixel|CWColormap|CWEventMask, &WinAttrib );

			if( !m_Window )
			{
				zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <ERROR> "
					"Failed to create window.\n" );
				XFree( pVI );
				return ZED_FAIL;
			}

//			XFree( pVI );

			XStoreName( m_pDisplay, m_Window, "ZED TEST" );
			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Mapping window.\n" );

			XMapWindow( m_pDisplay, m_Window );

			// Create a temporary OpenGL context to get the OpenGL version
			// supported by the graphics card
			GLXContext TmpCtx = glXCreateContext( m_pDisplay, pVI, 0, True );
			glXMakeCurrent( m_pDisplay, m_Window, TmpCtx );

			ZED_GLVERSION VerInfo;
			memset( &VerInfo, 0, sizeof( VerInfo ) );

			const char *pGLVer = ( const char* )glGetString( GL_VERSION );

			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				"Got version: %s.\n", pGLVer );

			// Extract the version information and store it in VerInfo
			// [N.B] This is pretty terrible -_-
			ZED_MEMSIZE StrLen = strlen( pGLVer );
			ZED_MEMSIZE MinorStart = 0;

			char MajorStr[ 3 ];
			char MinorStr[ 3 ];

			memset( MajorStr, '\0', sizeof( char )*3 );
			memset( MinorStr, '\0', sizeof( char )*3 );

			for( ZED_MEMSIZE i = 0; i < StrLen; i++ )
			{
				// Get the first string
				// There will be an error if no period or space is found by
				// three characters in.
				if( pGLVer[ i ] == 0x20 || pGLVer[ i ] == 0x00 ||
					pGLVer[ i ] == 0x2E )
				{
					MinorStart = i+1;
					break;
				}
				// Copy the char into another buffer
				MajorStr[ i ] = pGLVer[ i ];
			}

			for( ZED_MEMSIZE i = MinorStart; i < StrLen; i++ )
			{
				if( pGLVer[ i ] == 0x20 || pGLVer[ i ] == 0x00 ||
					pGLVer[ i ] == 0x2E )
				{
					break;
				}
				MinorStr[ i-MinorStart ] = pGLVer[ i ];
			}

			StrLen = strlen( MajorStr );

			// Convert both strings to their integer equals
			for( ZED_MEMSIZE i = 0; i < StrLen; i++ )
			{
				VerInfo.Major += ( MajorStr[ i ]-0x30 );//*( i*10 ) );
			}

			StrLen = strlen( MinorStr );

			for( ZED_MEMSIZE i = 0; i < StrLen; i++ )
			{
				VerInfo.Minor += ( MinorStr[ i ]-0x30 );//*( i*10 ) );
			}

			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				"GLVer: %d.%d\n", VerInfo.Major, VerInfo.Minor );
			if( m_GLExt.Initialise( VerInfo ) != ZED_OK )
			{
				// Something went wrong!  =O
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to get extensions.\n" );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			// Get rid of the temporary OpenGL context
			glXMakeCurrent( m_pDisplay, 0, 0 );
			glXDestroyContext( m_pDisplay, TmpCtx );

			XFree( pVI );

			ZED_INT32 ScreenNum = DefaultScreen( m_pDisplay );
			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				" Getting GLX Extensions.\n" );
			
			// Create a window (TEMP!)
			PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = ZED_NULL;
			glXCreateContextAttribs = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
				glXGetProcAddress(
				( const GLubyte * )"glXCreateContextAttribsARB" );

			m_GLExt = GLExtender( );

			ZED_INT32 Major = VerInfo.Major;
			ZED_INT32 Minor = VerInfo.Minor;
			// Rather than hard-code the Major and Minor, it should be
			// determined from the available OpenGL implementation
			ZED_INT32 ContextAttribs[ ] =
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB,	Major,
				GLX_CONTEXT_MINOR_VERSION_ARB,	Minor,
				GLX_CONTEXT_FLAGS_ARB,	GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#if ZED_BUILD_DEBUG
				GLX_CONTEXT_FLAGS_ARB,	GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
				None
			};

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Setting up OpenGL context.\n" );

			if( m_GLExt.InitialiseWindowExt( m_pDisplay, ScreenNum ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR>"
					"Failed to get GLX Extensions.\n" );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			// Attempt to create an OGL context for the highest OGL 3 version
			// Going down minor versions until zero is reached
			ZED_BOOL ContextCreated = ZED_FALSE;
			if( m_GLExt.IsWindowExtSupported( "GLX_ARB_create_context" ) ==
				ZED_TRUE )
			{
				while( Minor >= 0 && ContextCreated != ZED_TRUE )
				{
					m_GLContext = glXCreateContextAttribs(
						m_pDisplay, GLFBConf, 0, True, ContextAttribs );

					if( m_GLContext == ZED_NULL )
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
							"<ERROR> Failed to create context.\n" );
						break;
					}

					ZED_INT32 MakeCur = glXMakeCurrent( m_pDisplay, m_Window,
						m_GLContext );

					if( MakeCur == 0 ) // 0 == GLXBadContext
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
							"<ERROR> Failed to make context current.\n" );
						m_GLContext = 0;
						break;
					}

					if( MakeCur == True )
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
							"<INFO> Successfully created an OpenGL %d.%d "
							"Context.\n", ContextAttribs[ 1 ],
							ContextAttribs[ 3 ] );
						break;
					}

					Minor--;
					zedTrace( "Minor: %d\n", Minor );
					ContextAttribs[ 3 ] = Minor;
				}
			}
			else
			{
				// Unfortunately, the GLX version may be 1.3 or lower
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"GLX does not support GLX_EXT_ARB_create_context" );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			XSync( m_pDisplay, False );

			if( !m_GLContext )
			{
				zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <ERROR> "
					"Failed to create OpenGL context.\n" );
				return ZED_FAIL;
			}

			if( !glXIsDirect( m_pDisplay, m_GLContext ) )
			{
				zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
					"Indirect GLX Context.\n" );
			}
			else
			{
				zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
					"Direct GLX Context.\n" );
			}

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Making GLX Context current.\n" );

			glXMakeCurrent( m_pDisplay, m_Window, m_GLContext );
			// !MOVE

			return ZED_OK;
		}

		void LinuxRendererOGL3::CreateGLContext( )
		{
//			m_GLContext = glXCreateContext( m_pDisplay, 
		}

		void LinuxRendererOGL3::ForceClear( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
		}

		void LinuxRendererOGL3::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			glClearColor( p_Red, p_Green, p_Blue, 1.0f );
		}

		ZED_UINT32 LinuxRendererOGL3::BeginScene( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
			ZED_UINT32 Flags;

			if( p_Colour )
			{
				Flags |= GL_COLOR_BUFFER_BIT;
			}

			glClear( Flags );

			return ZED_OK;
		}

		void LinuxRendererOGL3::EndScene( )
		{
			glXSwapBuffers( m_pDisplay, m_Window );
		}

		ZED_BOOL LinuxRendererOGL3::ToggleFullscreen( )
		{
		}

		ZED_UINT32 LinuxRendererOGL3::ResizeCanvas( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			return ZED_OK;
		}

		void LinuxRendererOGL3::Release( )
		{
		}

		void LinuxRendererOGL3::SetView3D(
			const Arithmetic::Vector3 &p_Right,
			const Arithmetic::Vector3 &p_Up,
			const Arithmetic::Vector3 &p_Direction,
			const Arithmetic::Vector3 &p_Position )
		{
		}

		void LinuxRendererOGL3::SetViewLookAt(
			const Arithmetic::Vector3 &p_Position,
			const Arithmetic::Vector3 &p_Point,
			const Arithmetic::Vector3 &p_WorldUp )
		{
		}

		void LinuxRendererOGL3::CalcViewProjMatrix( )
		{
		}

		void LinuxRendererOGL3::CalcWorldViewProjMatrix( )
		{
		}

		void LinuxRendererOGL3::SetClippingPlanes( const ZED_FLOAT32 p_Near,
			const ZED_FLOAT32 p_Far )
		{
		}

		void LinuxRendererOGL3::Prepare2D( )
		{
		}

		ZED_UINT32 LinuxRendererOGL3::CalcPerspProjMatrix(
			const ZED_FLOAT32 p_FOV, const ZED_FLOAT32 p_AspectRatio,
			Arithmetic::Matrix4x4 *p_pMatrix )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::SetMode( const ZED_UINT32 p_Stage,
			const ZED_VIEWMODE p_Mode )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::SetDisplay( Display *p_pDisplay )
		{
			zedTrace( "In SetDisplay\n" );
			zedTrace( "p_pDisplay: %X\n", p_pDisplay );
			// Memory leaking ahoy!
			if( m_pDisplay != ZED_NULL )
			{
				zedTrace( "Deleting m_pDisplay\n" );
				delete m_pDisplay;
				m_pDisplay = ZED_NULL;
			}

			zedTrace( "Assigning p_pDisplay to m_pDisplay\n" );
			m_pDisplay = p_pDisplay;
			zedTrace( "m_pDisplay: %x\np_pDisplay: %x\n", m_pDisplay, p_pDisplay );

			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::Create( GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas,
			Display *p_pDisplay )
		{
			SetDisplay( p_pDisplay );
			Create( p_pAdapter, p_Canvas );
			return ZED_OK;
		}
	}
}

