#include <WindowsRendererOGL2.hpp>
#include <Matrix3x3.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Renderer
	{
		WindowsRendererOGL2::WindowsRendererOGL2( )
		{
			memset( &m_PixelFormat, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
			memset( &m_Canvas, 0, sizeof( CanvasDescription ) );
		}

		WindowsRendererOGL2::~WindowsRendererOGL2( )
		{
			Release( );
		}

		ZED_UINT32 WindowsRendererOGL2::Create( GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas )
		{
			// Grab the canvas for later
			m_Canvas = p_Canvas;

			if( !m_HDC )
			{
				zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
					" <ERROR> The HDC has not been set.\n" );
				return ZED_FAIL;
			}

			switch( m_Canvas.GetBPP( ) )
			{
			case ZED_FORMAT_A8R8G8B8:
				{
					m_PixelFormat.cColorBits = 32;
					m_PixelFormat.iPixelType = PFD_TYPE_RGBA;
					break;
				}
			default:
				{
					zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
						" <ERROR> Unsupported colour depth.\n" );
					return ZED_FAIL;
				}
			}

			switch( m_Canvas.GetDepthStencil( ) )
			{
			case ZED_FORMAT_D24S8:
				{
					m_PixelFormat.cDepthBits = 24;
					m_PixelFormat.cStencilBits = 8;
					break;
				}
			default:
				{
					zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
						" <ERROR> Unsupported depth/stencil format.\n" );
					return ZED_FAIL;
				}
			}

			m_PixelFormat.nSize = sizeof( PIXELFORMATDESCRIPTOR );
			m_PixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW |
				PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
			m_PixelFormat.iLayerType = PFD_MAIN_PLANE;

			// Select the most appropriate pixel format
			ZED_UINT32 PixelFormat = ChoosePixelFormat( m_HDC,
				&m_PixelFormat );

			if( PixelFormat == 0 )
			{
				zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
					" <ERROR> Failed to choose the preferred pixel format.\n" );
			}

			ZED_BOOL PixelResult = SetPixelFormat( m_HDC, PixelFormat,
				&m_PixelFormat );

			if( PixelResult == ZED_FALSE )
			{
				zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
					" <ERROR> Failed to set the preferred pixel format.\n" );
				return ZED_FAIL;
			}

			m_HGLRC = wglCreateContext( m_HDC );

			if( wglMakeCurrent( m_HDC, m_HGLRC ) == ZED_FALSE )
			{
				zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
					" <ERROR> Failed ro create the OpenGL Render Context.\n" );
				return ZED_FAIL;
			}

			// Make absolutely certain the GL RC is fine
			if( !m_HGLRC )
			{
				zedTrace( "[ZED|Renderer|WindowsRendererOGL2|Create]"
					" <ERROR> Failed to create GL Render Context.\n" );
				return ZED_FAIL;
			}

			m_Ext = GLExtender( m_HDC );

			// Get the OpenGL version, both for testing and for GL extensions
			const GLubyte *GLVersionString =
							glGetString( GL_VERSION );
			ZED_INT32 OpenGLVersion[ 2 ];

			// Extract the version number from the string
			OpenGLVersion[ 0 ] = ( GLVersionString[ 0 ] - 48 );
			OpenGLVersion[ 1 ] = ( GLVersionString[ 2 ] - 48 );

			zedTrace(	"<INFO> OpenGL Version in use: "
						" [ %d.%d ]\n",
						OpenGLVersion[ 0 ], OpenGLVersion[ 1 ] );

			// Set the class-side OpenGL Version information
			m_GLVersion.Major = OpenGLVersion[ 0 ];
			m_GLVersion.Minor = OpenGLVersion[ 1 ];
			
			if( m_Ext.Initialise( m_GLVersion ) != ZED_OK )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "<ERROR> Failed to initialise OpenGL extensions.\n" );
				MessageBoxA( NULL, "Failed to initialise OpenGL extensions",
					"ZED|Error", MB_OK );
				return ZED_FAIL;
			}
			
			zedTrace(
				"Created GL Render Context for Version"
				" [ %s ]\n", GLVersionString );

			char Message[ 255 ] = { '\0' };
			sprintf( Message,  "Created GL Render Context for Version"
				" [ %d.%d ]\n",
				OpenGLVersion[ 0 ], OpenGLVersion[ 1 ] );
			/*
			MessageBoxA( NULL, Message,
				"[ZED|Renderer|WindowsRendererOGL3] INFO",
				MB_ICONINFORMATION | MB_OK );*/

			// Set up the viewport
			ResizeCanvas( m_Canvas.GetWidth( ), m_Canvas.GetHeight( ) );

			return ZED_OK;
		}

		ZED_UINT32 WindowsRendererOGL2::Create( GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas, const HDC &p_HDC )
		{
			if( SetHDC( p_HDC ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			if( Create( p_pAdapter, p_Canvas ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 WindowsRendererOGL2::SetHDC( const HDC &p_HDC )
		{
			if( !p_HDC )
			{
				zedTrace( "[ZED|Renderer|WindowsRendererOGL2|SetHDC]"
					" <ERROR> Failed to set HDC.\n" );
				return ZED_FAIL;
			}

			m_HDC = p_HDC;

			return ZED_OK;
		}

		void WindowsRendererOGL2::ForceClear( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
			// Set the clear flags
			ZED_DWORD Flags = 0;

			if( p_Colour == ZED_TRUE )
			{
				Flags |= GL_COLOR_BUFFER_BIT;
			}

			if( p_Depth == ZED_TRUE )
			{
				Flags |= GL_DEPTH_BUFFER_BIT;
			}
			
			if( p_Stencil == ZED_TRUE )
			{
				Flags |= GL_STENCIL_BUFFER_BIT;
			}

			glClear( Flags );
		}

		ZED_UINT32 WindowsRendererOGL2::BeginScene( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
			// Set the clear flags
			ZED_DWORD Flags = 0;

			if( p_Colour == ZED_TRUE )
			{
				Flags |= GL_COLOR_BUFFER_BIT;
			}

			if( p_Depth == ZED_TRUE )
			{
				Flags |= GL_DEPTH_BUFFER_BIT;
			}
			
			if( p_Stencil == ZED_TRUE )
			{
				Flags |= GL_STENCIL_BUFFER_BIT;
			}

			glClear( Flags );

			return ZED_OK;
		}

		void WindowsRendererOGL2::EndScene( )
		{
			SwapBuffers( m_HDC );
		}

		void WindowsRendererOGL2::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			glClearColor( p_Red, p_Green, p_Blue, 1.0f );
		}

		ZED_BOOL WindowsRendererOGL2::ToggleFullscreen( )
		{
			return ZED_TRUE;
		}

		ZED_BOOL WindowsRendererOGL2::ResizeCanvas( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			// Make sure that there isn't a divide by zero
			if( p_Width == 0 )
			{
				return ZED_FALSE;
			}

			if( p_Height == 0 )
			{
				return ZED_FALSE;
			}

			// Check that the width and height are the same as the ones already
			// set
			if( ( p_Width == m_Canvas.GetWidth( ) ) &&
				( p_Height == m_Canvas.GetHeight( ) ) )
			{
				return ZED_FALSE;
			}

			m_Canvas.SetWidth( p_Width );
			m_Canvas.SetHeight( p_Height );

			// Get the aspect ratio for the window
			m_Canvas.SetAspectRatio(
				static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) ) /
				static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) ) );

			glViewport( 0, 0, m_Canvas.GetWidth( ), m_Canvas.GetHeight( ) );

			return ZED_TRUE;
		}

		void WindowsRendererOGL2::Release( )
		{
			// Release the GL RC
			wglMakeCurrent( NULL, NULL );

			if( m_HGLRC )
			{
				wglDeleteContext( m_HGLRC );
				m_HGLRC = NULL;
			}
		}

		void WindowsRendererOGL2::SetView3D(
			const Arithmetic::Vector3 &p_Right,
			const Arithmetic::Vector3 &p_Up,
			const Arithmetic::Vector3 &p_Direction,
			const Arithmetic::Vector3 &p_Position )
		{
			//  R  R  R P
			//  U  U  U P
			// -D -D -D P
			//  0  0  0 1
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

		void WindowsRendererOGL2::SetViewLookAt(
			const Arithmetic::Vector3 &p_Position,
			const Arithmetic::Vector3 &p_Point,
			const Arithmetic::Vector3 &p_WorldUp )
		{
			// Calculate the view vectors
			Arithmetic::Vector3 ViewDir;
			Arithmetic::Vector3 ViewRight;
			Arithmetic::Vector3 ViewUp;

			ViewDir.Copy( p_Point - p_Position );
			ViewDir.Normalise( );

			ViewRight.Copy( ViewDir.Cross( p_WorldUp ) );
			ViewRight.Normalise( );

			ViewUp.Copy( ViewRight.Cross( ViewDir ) );
			ViewUp.Normalise( );

			Arithmetic::Matrix3x3 Mat3;
			Mat3.SetRows( ViewRight, ViewUp, -ViewDir );

			Arithmetic::Vector3 Position;
			Position.Copy( -( Mat3*p_Position ) );

			// Call SetView3D to handle the rest
			SetView3D( ViewRight, ViewUp, -ViewDir, Position );
		}

		void WindowsRendererOGL2::CalcViewProjMatrix( )
		{
			Arithmetic::Matrix4x4 *pMatA, *pMatB;

			if( m_ViewMode == ZED_VIEW_SCREEN )
			{
				pMatA = ( Arithmetic::Matrix4x4 * )&m_ProjectionScreen;
				pMatB = ( Arithmetic::Matrix4x4 * )&m_View2D;
			}
			else
			{
				pMatB = ( Arithmetic::Matrix4x4 * )&m_View3D;

				if( m_ViewMode == ZED_VIEW_PERSPECTIVE )
				{
					pMatA = ( Arithmetic::Matrix4x4 * )&(
						m_ProjectionPerspective[ m_Stage ] );
				}
				else
				{
					pMatA = ( Arithmetic::Matrix4x4 * )&(
						m_ProjectionOrthogonal[ m_Stage ] );
				}
			}

			Arithmetic::Matrix4x4 *pMat =
				( Arithmetic::Matrix4x4 * )&m_ViewProjection;

			( *pMat ).Copy( ( *pMatA )*( *pMatB ) );
		}

		void WindowsRendererOGL2::CalcWorldViewProjMatrix( )
		{
			Arithmetic::Matrix4x4 *pProjection, *pView, *pWorld;

			pWorld = ( Arithmetic::Matrix4x4 * )&m_World;

			if( m_ViewMode == ZED_VIEW_SCREEN )
			{
				pProjection = ( Arithmetic::Matrix4x4 * )&m_ProjectionScreen;
				pView = ( Arithmetic::Matrix4x4 * )&m_View2D;
			}
			else	
			{
				pView = ( Arithmetic::Matrix4x4 * )&m_View3D;

				if( m_ViewMode == ZED_VIEW_PERSPECTIVE )
				{
					pProjection = ( Arithmetic::Matrix4x4 * )&(
						m_ProjectionPerspective[ m_Stage ] );
				}
				else
				{
					pProjection = ( Arithmetic::Matrix4x4 * )&(
						m_ProjectionOrthogonal[ m_Stage ] );
				}
			}

			m_WorldViewProjection.Copy( ( ( *pWorld )*( *pView ) )*
				( *pProjection ) );
		}

		void WindowsRendererOGL2::SetClippingPlanes( const ZED_FLOAT32 p_Near,
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

			// Adjust orthogonal projection
			ZED_FLOAT32 FarFactor = 1.0f / ( m_Near - m_Far );
			ZED_FLOAT32 NearFactor = -FarFactor*m_Near;

			m_ProjectionOrthogonal[ 0 ]( 2, 2 ) =
				m_ProjectionOrthogonal[ 1 ]( 2, 2 ) =
				m_ProjectionOrthogonal[ 2 ]( 2, 2 ) =
				m_ProjectionOrthogonal[ 3 ]( 2, 2 ) = FarFactor;

			m_ProjectionOrthogonal[ 0 ]( 3, 2 ) =
				m_ProjectionOrthogonal[ 1 ]( 3, 2 ) =
				m_ProjectionOrthogonal[ 2 ]( 3, 2 ) =
				m_ProjectionOrthogonal[ 3 ]( 3, 2 ) = NearFactor;

			// Re-assign the near and far factors for perspective
			FarFactor *= m_Far;
			NearFactor = -FarFactor*m_Near;

			m_ProjectionPerspective[ 0 ]( 2, 2 ) =
				m_ProjectionPerspective[ 1 ]( 2, 2 ) =
				m_ProjectionPerspective[ 2 ]( 2, 2 ) =
				m_ProjectionPerspective[ 3 ]( 2, 2 ) = FarFactor;

			m_ProjectionPerspective[ 0 ]( 3, 2 ) =
				m_ProjectionPerspective[ 1 ]( 3, 2 ) =
				m_ProjectionPerspective[ 2 ]( 3, 2 ) =
				m_ProjectionPerspective[ 3 ]( 3, 2 ) = NearFactor;
		}

		void WindowsRendererOGL2::Prepare2D( )
		{
			// Make the 2D projection matrix an identity matrix
			m_ProjectionScreen.Identity( );
			m_ViewScreen.Identity( );

			m_ProjectionScreen( 0, 0 ) =
				2.0f / static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) );
			m_ProjectionScreen( 1, 1 ) =
				2.0f / static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) );
			m_ProjectionScreen( 2, 2 ) = 1.0f / ( m_Far - m_Near );
			m_ProjectionScreen( 3, 2 ) = -m_Near*( 1.0f / ( m_Far - m_Near ) );

			// 2D view matrix
			ZED_FLOAT32 TX, TY, TZ;
			TX = -( static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) ) ) +
				( static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) )*0.5f );
			TY = static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) ) -
				( static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) )*0.5f );
			TZ = m_Near + 0.1f;

			m_View2D( 1, 1 ) = -1.0f;
			m_View2D( 3, 0 ) = TX;
			m_View2D( 3, 1 ) = TY;
			m_View2D( 3, 2 ) = TZ;
		}

		ZED_UINT32 WindowsRendererOGL2::CalcPerspProjMatrix(
			const ZED_FLOAT32 p_FOV,
			const ZED_FLOAT32 p_AspectRatio,
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

		ZED_UINT32 WindowsRendererOGL2::SetMode( const ZED_UINT32 p_Stage,
			const ZED_VIEWMODE p_Mode )
		{
			m_Stage = p_Stage;
			m_ViewMode = p_Mode;
			return ZED_OK;
		}
	}
}
