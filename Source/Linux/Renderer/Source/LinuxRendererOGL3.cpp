#include <LinuxRendererOGL3.hpp>
#include <cstring>
#include <Matrix3x3.hpp>

namespace ZED
{
	namespace Renderer
	{
		LinuxRendererOGL3::LinuxRendererOGL3( )
		{
			// Set pointers to null and everything else to sane values
			m_pDisplay = ZED_NULL;
			m_pScreen = ZED_NULL;
			m_pVertexCacheManager = ZED_NULL;

			m_View3D.Identity( );

			// By default render as perspective
			m_ViewMode = ZED_VIEW_PERSPECTIVE;

			// By default, leave the cursor as visible
			m_CursorHidden = ZED_FALSE;
		}

		LinuxRendererOGL3::~LinuxRendererOGL3( )
		{
			// Get rid of the vertex cache manager
			if( m_pVertexCacheManager != ZED_NULL )
			{
				zedTrace( "[ZED::LinuxRendererOGL3::~LinuxRendererOGL3] <INFO>"
					" Deleting Vertex Cache Manager\n" );
				delete m_pVertexCacheManager;
				m_pVertexCacheManager = ZED_NULL;
			}
			// Unhide the cursor
			if( m_CursorHidden )
			{
				XUndefineCursor( m_pDisplay, m_Window );
			}

			// Unbind GLX
			if( m_pDisplay )
			{
				glXMakeCurrent( m_pDisplay, 0, 0 );
			}

			// Free the GLX context
			if( m_pDisplay && m_GLContext )
			{
				glXDestroyContext( m_pDisplay, m_GLContext );
			}

			// MOVE THESE OUT OF HERE.
			// LET THE APPLICATION HANDLE IT
			if( m_pDisplay && m_Window )
			{
				zedTrace( "Destroying Window\n" );
				XDestroyWindow( m_pDisplay, m_Window );
			}

			if( m_pDisplay && m_ColMap )
			{
				zedTrace( "Destroying colourmap\n" );
				XFreeColormap( m_pDisplay, m_ColMap );
			}

			if( m_pDisplay )
			{
				zedTrace( "Destroying Display\n" );
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

			// Just choose the default one
			GLXFBConfig GLFBConf = pFBC[ 0 ];

			// Done with the FBC
			XFree( pFBC );

			// Get a visual
			XVisualInfo *pVI = glXGetVisualFromFBConfig( m_pDisplay,
				GLFBConf );
			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Visual ID = 0x%08X\n", pVI->visualid );

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
			WinAttrib.event_mask = StructureNotifyMask|ExposureMask|
				KeyPressMask|KeyReleaseMask|ButtonPressMask|ResizeRedirectMask|
				PointerMotionMask;
			WinAttrib.override_redirect = false;

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Setting up window.\n" );

			m_Window = XCreateWindow( m_pDisplay,
				RootWindow( m_pDisplay, pVI->screen ), 0, 0,
				m_Canvas.GetWidth( ), m_Canvas.GetHeight( ),
				0, pVI->depth, InputOutput, pVI->visual,
				CWOverrideRedirect|CWBorderPixel|CWColormap|CWEventMask, &WinAttrib );

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
//			XSelectInput( m_pDisplay, m_Window, ExposureMask | PointerMotionMak
			if( m_CursorHidden != ZED_FALSE )
			{
				Pixmap BlankPointer;
				XColor BlankColour;
				char Data[ 1 ] = { 0 };
				Cursor cursor;

				BlankPointer = XCreateBitmapFromData( m_pDisplay, m_Window,
					Data, 1, 1 );
				if( BlankPointer == None )
				{
				}
				cursor = XCreatePixmapCursor( m_pDisplay, BlankPointer,
					BlankPointer, &BlankColour, &BlankColour, 0, 0 );
				XFreePixmap( m_pDisplay, BlankPointer );
				XDefineCursor( m_pDisplay, m_Window, cursor );
			}
			XMapRaised( m_pDisplay, m_Window );
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
				// Get rid of the temporary OpenGL context
				glXMakeCurrent( m_pDisplay, 0, 0 );
				glXDestroyContext( m_pDisplay, TmpCtx );
				// Something went wrong
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
			//
			this->ResizeCanvas( m_Canvas.GetWidth( ), m_Canvas.GetHeight( ) );
			m_pVertexCacheManager = new GLVertexCacheManager( );
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
			GLbitfield Flags;

			if( p_Colour )
			{
				Flags |= GL_COLOR_BUFFER_BIT;
			}
			if( p_Depth )
			{
				Flags |= GL_DEPTH_BUFFER_BIT;
			}

			glClear( Flags );

			return ZED_OK;
		}

		void LinuxRendererOGL3::EndScene( )
		{
			m_pVertexCacheManager->ForceFlushAll( );
			glXSwapBuffers( m_pDisplay, m_Window );
		}

		ZED_BOOL LinuxRendererOGL3::ToggleFullscreen( )
		{
		}

		ZED_UINT32 LinuxRendererOGL3::ResizeCanvas( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			if( p_Width == 0 || p_Height == 0 )
			{
				return ZED_FAIL;
			}
			
			if( ( p_Width == m_Canvas.GetWidth( ) ) &&
				( p_Height == m_Canvas.GetHeight( ) ) )
			{
				return ZED_OK;
			}

			m_Canvas.SetWidth( p_Width );
			m_Canvas.SetHeight( p_Height );

			// Get the aspect ratio
			if( p_Width > p_Height )
			{
				m_Canvas.SetAspectRatio(
					static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) ) /
					static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) ) );
			}
			else
			{
				m_Canvas.SetAspectRatio(
					static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) ) /
					static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) ) );
			}

			zglViewport( 0, 0, m_Canvas.GetWidth( ), m_Canvas.GetHeight( ) );

			CalcViewProjMatrix( );

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
			// Create the world matrix for tha camera
			//  R  R  R  P
			//  U  U  U  P
			// -D -D -D  P
			//  0  0  0  1
			//
			//  R  U  D  P
			//  R  U  D  P
			//  R  U  D  P
			//  0  0  0  1
			m_View3D( 3, 0 ) = m_View3D( 3, 1 ) = m_View3D( 3, 2 ) = 0.0f;
			m_View3D( 3, 3 ) = 1.0f;

			m_View3D( 0, 0 ) = p_Right[ 0 ];
			m_View3D( 0, 1 ) = p_Right[ 1 ];
			m_View3D( 0, 2 ) = p_Right[ 2 ];

			m_View3D( 1, 0 ) = p_Up[ 0 ];
			m_View3D( 1, 1 ) = p_Up[ 1 ];
			m_View3D( 1, 2 ) = p_Up[ 2 ];

			m_View3D( 2, 0 ) = p_Direction[ 0 ];
			m_View3D( 2, 1 ) = p_Direction[ 1 ];
			m_View3D( 2, 2 ) = p_Direction[ 2 ];

			m_View3D( 0, 3 ) = p_Position[ 0 ];
			m_View3D( 1, 3 ) = p_Position[ 1 ];
			m_View3D( 2, 3 ) = p_Position[ 2 ];
		}

		void LinuxRendererOGL3::SetViewLookAt(
			const Arithmetic::Vector3 &p_Position,
			const Arithmetic::Vector3 &p_Point,
			const Arithmetic::Vector3 &p_WorldUp )
		{
			// Create the view vectors
			Arithmetic::Vector3 Direction;
			Arithmetic::Vector3 Right;
			Arithmetic::Vector3 Up;
/*
			Direction = p_Point - p_Position;
			Direction.Normalise( );

			Right = Direction.Cross( p_WorldUp );
			Right.Normalise( );

			Up = Right.Cross( Direction );
			Up.Normalise( );

			Arithmetic::Matrix3x3 Collection;
			Collection.SetRows( Right, Up, -Direction );

			Arithmetic::Vector3 Position = -( Collection*p_Position );*/

			Direction = ( p_Point - p_Position );
			Direction.Normalise( );

			Up = ( p_WorldUp-( Direction*p_WorldUp.Dot( Direction ) ) );
			Up.Normalise( );

			Right = Up.Cross( Direction );

			Arithmetic::Matrix3x3 Rot;
			Rot.SetRows( Right, Up, -Direction );

			Arithmetic::Vector3 Position = -( Rot*p_Position );

			zedTrace( "Right: %f %f %f\n", Right[ 0 ], Right[ 1 ],
				Right[ 2 ] );
			zedTrace( "Up: %f %f %f\n", Up[ 0 ], Up[ 1 ], Up[ 2 ] );
			zedTrace( "Direction: %f %f %f\n",
				-Direction[ 0 ], -Direction[ 1 ], -Direction[ 2 ] );
			zedTrace( "Position: %f %f %f\n",
				Position[ 0 ], Position[ 1 ], Position[ 2 ] );

			// Use SetView3D to handle the rest
			SetView3D( Right, Up, -Direction, Position );
		}

		void LinuxRendererOGL3::CalcViewProjMatrix( )
		{
			Arithmetic::Matrix4x4 *pMatA, *pMatB;

			if( m_ViewMode == ZED_VIEW_SCREEN )
			{
			}
			else
			{
				pMatB = &m_View3D;

				if( m_ViewMode == ZED_VIEW_PERSPECTIVE )
				{
					pMatA = &m_ProjectionPerspective;
				}
			}
			
			m_ViewProjection = ( ( *pMatA )*( *pMatB ) );
		}

		void LinuxRendererOGL3::CalcWorldViewProjMatrix( )
		{
			Arithmetic::Matrix4x4 *pProjection, *pView, *pWorld;

			pWorld = ( Arithmetic::Matrix4x4 * )&m_World;
		}

		void LinuxRendererOGL3::SetClippingPlanes( const ZED_FLOAT32 p_Near,
			const ZED_FLOAT32 p_Far )
		{
			m_Near = p_Near;
			m_Far = p_Far;

			if( m_Near <= ZED_Epsilon )
			{
				m_Near = ZED_Epsilon;
			}
			if( m_Far <= 1.0f )
			{
				m_Far = 1.0f;
			}

			if( m_Near >= m_Far )
			{
				m_Near = m_Far;
				m_Far = m_Near + 1.0f;
			}

			Prepare2D( );

			// Create the perspective view
			ZED_FLOAT32 FarFactor = ( 1.0f/( m_Near - m_Far ) ) * m_Far;
			ZED_FLOAT32 NearFactor = -FarFactor*m_Near;

			m_ProjectionPerspective( 2, 2 ) = FarFactor;
			m_ProjectionPerspective( 3, 2 ) = NearFactor;
		}

		void LinuxRendererOGL3::Prepare2D( )
		{
		}

		ZED_UINT32 LinuxRendererOGL3::CalcPerspProjMatrix(
			const ZED_FLOAT32 p_FOV, const ZED_FLOAT32 p_AspectRatio,
			Arithmetic::Matrix4x4 *p_pMatrix )
		{
			if( Arithmetic::Absolute( m_Far - m_Near ) < ZED_Epsilon )
			{
				return ZED_FAIL;
			}
			
			ZED_FLOAT32 d = 1.0f/tan( p_FOV / 180.0f * ZED_Pi * 0.5f );
			ZED_FLOAT32 Recip = 1.0f/( m_Near-m_Far );

			( *p_pMatrix )( 0, 0 ) = d / p_AspectRatio;
			( *p_pMatrix )( 1, 1 ) = d;
			( *p_pMatrix )( 2, 2 ) = ( m_Near+m_Far )*Recip;
			( *p_pMatrix )( 2, 3 ) = 2*m_Near*m_Far*Recip;
			( *p_pMatrix )( 3, 2 ) = -1.0f;
			( *p_pMatrix )( 3, 3 ) = 0.0f;

			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::SetMode( const ZED_UINT32 p_Stage,
			const ZED_VIEWMODE p_Mode )
		{
			m_ViewMode = p_Mode;
			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::Render( const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
			const ZED_UINT32 p_MaterialID )
		{
			m_pVertexCacheManager->Render( p_VertexCount, p_pVertices,
				p_pIndexCount, p_pIndices, p_Attributes, p_MaterialID );
//			m_pVertexCacheManager->ForceFlushAll( );

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

		void LinuxRendererOGL3::GetWVP( Arithmetic::Matrix4x4 *p_pMatrix )
		{
			( *p_pMatrix ) = m_View3D;
		}

		void LinuxRendererOGL3::GetVP( Arithmetic::Matrix4x4 *p_pMatrix )
		{
			( *p_pMatrix ) = m_ViewProjection;
		}

		void LinuxRendererOGL3::SetRenderState( const ZED_RENDERSTATE p_State,
			const ZED_MEMSIZE p_Value )
		{
			// Determine the type and set the RS
			// Any fixed function states are not checked
			switch( p_State )
			{
				case ZED_RS_DEPTH:
				{
					switch( p_Value )
					{
						case ZED_ENABLE:
						{
							zglEnable( GL_DEPTH );
						}
						case ZED_DISABLE:
						{
							zglDisable( GL_DEPTH );
						}
					}
					break;
				}
				case ZED_RS_CULLMODE:
				{
					switch( p_Value )
					{
						case ZEDCULL_NONE:
						{
							zglDisable( GL_CULL_FACE );
						}
						case ZEDCULL_CCW:
						{
							zglEnable( GL_CULL_FACE );
							zglFrontFace( GL_CCW );
							zglCullFace( GL_FRONT );
						}
						case ZEDCULL_CW:
						{
							zglEnable( GL_CULL_FACE );
							zglFrontFace( GL_CW );
							zglCullFace( GL_FRONT );
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
}

