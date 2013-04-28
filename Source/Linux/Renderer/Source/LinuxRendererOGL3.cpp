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
			m_pVertexCacheManager = ZED_NULL;

			m_View3D.Identity( );

			// By default render as perspective
			m_ViewMode = ZED_VIEW_PERSPECTIVE;
		}

		LinuxRendererOGL3::~LinuxRendererOGL3( )
		{
			if( m_pVertexCacheManager != ZED_NULL )
			{
				zedTrace( "[ZED::LinuxRendererOGL3::~LinuxRendererOGL3] <INFO>"
					" Deleting Vertex Cache Manager\n" );
				delete m_pVertexCacheManager;
				m_pVertexCacheManager = ZED_NULL;
			}

			// Unbind GLX
			if( m_WindowData.pX11Display )
			{
				glXMakeCurrent( m_WindowData.pX11Display, 0, 0 );
				if( m_GLContext )
				{
					glXDestroyContext( m_WindowData.pX11Display, m_GLContext );
				}
			}

		}

		ZED_UINT32 LinuxRendererOGL3::Create(// GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas,
			const ZED::Renderer::Window &p_Window )
		{
			m_WindowData = p_Window.WindowData( );
			if( m_WindowData.pX11Display == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Display not initialised\n" );
				return ZED_FAIL;
			}

			m_Canvas = p_Canvas;

			// Put the canvas' colour, depth, and stencil formats converted
			// into a format that can be consumed by the VA
			ZED_SINT32 Depth = 0, Stencil = 0, A = 0, R = 0, G = 0, B = 0;
			switch( m_Canvas.DepthStencilFormat( ) )
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

			switch( m_Canvas.ColourFormat( ) )
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
	
			ZED_SINT32 AntiAliasing = 0, AntiAliasingCount = 0;
			switch( m_Canvas.AntiAliasingType( ) )
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
				GLX_DOUBLEBUFFER, ( m_Canvas.BackBufferCount( ) > 0 ?
					True : False ),
				GLX_SAMPLE_BUFFERS,	AntiAliasing,
				GLX_SAMPLES,		AntiAliasingCount,/*
				GLX_COVERAGE_SAMPLES_NV,	CSCoverSamples,
				GLX_COLOR_SAMPLES_NV,		CSColourSamples,*/
				None
			};

			// Create a temporary OpenGL context to get the OpenGL version
			// supported by the graphics card
			GLXContext TmpCtx = glXCreateContext( m_WindowData.pX11Display,
				m_WindowData.pX11VisualInfo, 0, True );
			glXMakeCurrent( m_WindowData.pX11Display, m_WindowData.X11Window,
				TmpCtx );

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
				glXMakeCurrent( m_WindowData.pX11Display, 0, 0 );
				glXDestroyContext( m_WindowData.pX11Display, TmpCtx );
				// Something went wrong
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to get extensions.\n" );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			// Get rid of the temporary OpenGL context
			glXMakeCurrent( m_WindowData.pX11Display, 0, 0 );
			glXDestroyContext( m_WindowData.pX11Display, TmpCtx );

			ZED_SINT32 ScreenNum = DefaultScreen( m_WindowData.pX11Display );
			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				" Getting GLX Extensions.\n" );
			
			// Create a window (TEMP!)
			PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = 
				ZED_NULL;
			glXCreateContextAttribs = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
				glXGetProcAddress(
				( const GLubyte * )"glXCreateContextAttribsARB" );

			m_GLExt = GLExtender( );

			ZED_SINT32 Major = VerInfo.Major;
			ZED_SINT32 Minor = VerInfo.Minor;
			// Rather than hard-code the Major and Minor, it should be
			// determined from the available OpenGL implementation
			ZED_SINT32 ContextAttribs[ ] =
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB,	Major,
				GLX_CONTEXT_MINOR_VERSION_ARB,	Minor,
//				GLX_CONTEXT_FLAGS_ARB,	GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#if ZED_BUILD_DEBUG
//				GLX_CONTEXT_FLAGS_ARB,	GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
				None
			};

			zedTrace( "[ZED:Renderer:LinuxRendererOGL3:Create] <INFO> "
				"Setting up OpenGL context.\n" );

			if( m_GLExt.InitialiseWindowExt( m_WindowData.pX11Display,
				ScreenNum ) != ZED_OK )
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
					zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
						"<INFO> Attempting to create context for "
						"OpenGL %d.%d\n", Major, Minor );

					m_GLContext = glXCreateContextAttribs(
						m_WindowData.pX11Display, m_WindowData.X11GLXFBConfig,
						0, True, ContextAttribs );

					if( m_GLContext == ZED_NULL )
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
							"<ERROR> Failed to create context.\n" );
						break;
					}

					ZED_SINT32 MakeCur = glXMakeCurrent(
						m_WindowData.pX11Display, m_WindowData.X11Window,
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
					"GLX does not support GLX_ARB_create_context" );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			XSync( m_WindowData.pX11Display, False );

			if( !m_GLContext )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to create OpenGL context.\n" );

				return ZED_FAIL;
			}

			if( !glXIsDirect( m_WindowData.pX11Display, m_GLContext ) )
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

			glXMakeCurrent( m_WindowData.pX11Display, m_WindowData.X11Window,
				m_GLContext );

			this->ResizeCanvas( m_Canvas.Width( ), m_Canvas.Height( ) );
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
			glXSwapBuffers( m_WindowData.pX11Display, m_WindowData.X11Window );
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
			
			if( ( p_Width == m_Canvas.Width( ) ) &&
				( p_Height == m_Canvas.Height( ) ) )
			{
				return ZED_OK;
			}

			m_Canvas.Width( p_Width );
			m_Canvas.Height( p_Height );

			// Get the aspect ratio
			if( p_Width > p_Height )
			{
				m_Canvas.AspectRatio(
					static_cast< ZED_FLOAT32 >( m_Canvas.Width( ) ) /
					static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) ) );
			}
			else
			{
				m_Canvas.AspectRatio(
					static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) ) /
					static_cast< ZED_FLOAT32 >( m_Canvas.Width( ) ) );
			}

			zglViewport( 0, 0, m_Canvas.Width( ), m_Canvas.Height( ) );

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
				zedTrace( "Far-Near less than epsilon\n" );
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
			const ZED_UINT32 p_MaterialID,
			ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			m_pVertexCacheManager->Render( p_VertexCount, p_pVertices,
				p_pIndexCount, p_pIndices, p_Attributes, p_MaterialID,
				p_PrimitiveType );

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
				case ZED_RENDERSTATE_DEPTH:
				{
					switch( p_Value )
					{
						case ZED_ENABLE:
						{
							zglEnable( GL_DEPTH_TEST );
							glDepthFunc( GL_LEQUAL );
							glDepthMask( GL_TRUE );
							glDepthRange( 0.0f, 1.0f );
							break;
						}
						case ZED_DISABLE:
						{
							zglDisable( GL_DEPTH_TEST );
							break;
						}
					}
					break;
				}
				case ZED_RENDERSTATE_CULLMODE:
				{
					zedTrace( "Setting cullmode: " );
					switch( p_Value )
					{
						case ZED_CULLMODE_NONE:
						{
							zglDisable( GL_CULL_FACE );
							zedTrace( "[DISABLED]\n" );
							break;
						}
						case ZED_CULLMODE_CCW:
						{
							zglEnable( GL_CULL_FACE );
							zglFrontFace( GL_CCW );
							zglCullFace( GL_FRONT );
							zedTrace( "[COUNTER-CLOCKWISE]\n" );
							break;
						}
						case ZED_CULLMODE_CW:
						{
							zglEnable( GL_CULL_FACE );
							zglFrontFace( GL_CW );
							zglCullFace( GL_FRONT );
							zedTrace( "[CLOCKWISE]\n" );
							break;
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

