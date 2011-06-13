#include <Renderer.hpp>
#include <WindowsRendererOGL3.hpp>
#include <CanvasDescription.hpp>
#include <Debugger.hpp>
#include <Matrix3x3.hpp>

namespace ZED
{
	namespace Renderer
	{
		WindowsRendererOGL3::WindowsRendererOGL3( )
		{
			memset( &m_PixelFormat, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
			memset( &m_Canvas, 0, sizeof( CanvasDescription ) );
		}

		WindowsRendererOGL3::~WindowsRendererOGL3( )
		{
			// Release the GL RC (if not already done)
			wglMakeCurrent( NULL, NULL );

			if( m_HGLRC )
			{
				wglDeleteContext( m_HGLRC );
				m_HGLRC = NULL;
			}
		}

		ZED_UINT32 WindowsRendererOGL3::Create( GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas )
		{
			// Grab the canvas
			m_Canvas = p_Canvas;

			if( !m_HDC )
			{
				zedTrace( "The HDC has not been set" );
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
					zedTrace( "Failed to set the colour format" );
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
					zedTrace( "Failed to set the depth/stencil format" );
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
				zedTrace( "Failed to choose the pixel format" );
				return ZED_FAIL;
			}

			ZED_BOOL PixelResult = SetPixelFormat( m_HDC, PixelFormat,
				&m_PixelFormat );

			if( PixelResult == ZED_FALSE )
			{
				zedTrace( "Failed to set the pixel format\n" );
				return ZED_FAIL;
			}

			// Create a fallback GL RC (this is necessary, anyway)
			HGLRC TempHGLRC = wglCreateContext( m_HDC );

			if( wglMakeCurrent( m_HDC, TempHGLRC ) == ZED_FALSE )
			{
				zedTrace( "Could not make the HGLRC current\n" );
				return ZED_FAIL;
			}

			//m_Ext = GLExtender( m_HDC );

			// List of OpenGL versions to try and use for the context creation
			const ZED_INT32 OGLVersions[ ] =
			{
				3, 0,
				3, 1,
				3, 2,
				3, 3,
			};

			ZED_INT32 OpenGLVersion [ 2 ];
				glGetIntegerv( GL_MAJOR_VERSION, &OpenGLVersion[ 0 ] );
				glGetIntegerv( GL_MINOR_VERSION, &OpenGLVersion[ 1 ] );
				zedTrace(
							"Version"
							" [ %d.%d ]\n",
							OpenGLVersion[ 0 ], OpenGLVersion[ 1 ] );

						// Report the OpenGL version in use
			const GLubyte *GLVersionString =
				glGetString( GL_VERSION );
			zedTrace( "GLVersion: %s\n", GLVersionString );

			ZED_INT32 Major = OGLVersions[ ( sizeof( OGLVersions ) / 4 ) - 2 ],
				Minor = OGLVersions[ ( sizeof( OGLVersions ) / 4 ) - 1 ];

			ZED_INT32 Attribs[ ] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, Major,
				WGL_CONTEXT_MINOR_VERSION_ARB, Minor,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#if ZED_BUILD_DEBUG
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
				0
			};
			
			// Attempt to create an OGL 3.3 context
			if( m_Ext.IsGLExtSupported( "WGL_ARB_create_context" ) ==
				ZED_TRUE )
			{
				// Try to use the highest available OpenGL version, keep trying
				// until no more can be tried
				for( ZED_INT32 i = 1;
					i < ( sizeof( OGLVersions ) / 4 ) / 2;
					i++ )
				{
					m_HGLRC = m_Ext.wglCreateContextAttribsARB( m_HDC, 
						0, Attribs );
					wglMakeCurrent( NULL, NULL );
					wglDeleteContext( TempHGLRC );
					wglMakeCurrent( m_HDC, m_HGLRC );

					if( !m_HGLRC )
					{
						// Loop, again
						zedTrace(
							"Failed to create GL Render Context for Version"
							" [ %d.%d ]\n",
							Major, Minor );
					}
					else
					{
						ZED_INT32 OpenGLVersion [ 2 ];
						glGetIntegerv( GL_MAJOR_VERSION, &OpenGLVersion[ 0 ] );
						glGetIntegerv( GL_MINOR_VERSION, &OpenGLVersion[ 1 ] );

						// Report the OpenGL version in use
						const GLubyte *GLVersionString =
							glGetString( GL_VERSION );
						zedTrace(
							"Created GL Render Context for Version"
							" [ %d.%d ]\n",
							OpenGLVersion[ 0 ], OpenGLVersion[ 1 ] );

						// Break out
						break;
					}

					Major = OGLVersions[ ( sizeof( OGLVersions ) / 4 ) -
						( 2 + ( i*2) ) ];
					Minor = OGLVersions[ ( sizeof( OGLVersions ) / 4 ) -
						( 1 + ( i*2 ) ) ];

					// Use the next major and minor versions of OpenGL in the
					// list
					Attribs[ 1 ] = Major;
					Attribs[ 3 ] = Minor;
				}
			}
			else
			{
				m_HGLRC = TempHGLRC;
			}
			
			if( !m_HGLRC )
			{
				zedTrace( "Failed to create GL Render Context" );
				return ZED_FAIL;
			}

			// Set the viewport
			ResizeCanvas( m_Canvas.GetWidth( ), m_Canvas.GetHeight( ) );

			return ZED_OK;
		}

		ZED_UINT32 WindowsRendererOGL3::Create( GraphicsAdapter *p_pAdapter,
			const CanvasDescription &p_Canvas, const HDC &p_HDC )
		{
			if( Create( p_pAdapter, p_Canvas ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			if( SetHDC( p_HDC ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 WindowsRendererOGL3::SetHDC( const HDC &p_HDC )
		{
			if( !p_HDC )
			{
				zedTrace( "Failed to set the HDC" );
				return ZED_FAIL;
			}

			m_HDC = p_HDC;

			return ZED_OK;
		}

		void WindowsRendererOGL3::ForceClear( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
			// Set the glClear flags
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

		ZED_UINT32 WindowsRendererOGL3::BeginScene( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
			// Set the glClear flags
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

		void WindowsRendererOGL3::EndScene( )
		{
			SwapBuffers( m_HDC );
		}

		void WindowsRendererOGL3::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			glClearColor( p_Red, p_Green, p_Blue, 1.0f );
		}

		ZED_BOOL WindowsRendererOGL3::ToggleFullscreen( )
		{
			return ZED_TRUE;
		}

		ZED_BOOL WindowsRendererOGL3::ResizeCanvas( const ZED_UINT32 p_Width,
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

			// Check if the width and height are the same as the ones already
			// set
			if( ( p_Width == m_Canvas.GetWidth( ) ) &&
				( p_Height == m_Canvas.GetHeight( ) ) )
			{
				return ZED_FALSE;
			}

			// Reset the coordinate system
			// GLDEPRICATED!
			//glLoadIdentity( );
			// !GLDEPRICATED

			m_Canvas.SetWidth( p_Width );
			m_Canvas.SetHeight( p_Height );

			// Get the aspect ratio for the window
			m_Canvas.m_AspectRatio = static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) )
				/ static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) );

			glViewport( 0, 0, m_Canvas.GetWidth( ), m_Canvas.GetHeight( ) );

			return ZED_TRUE;
		}

		void WindowsRendererOGL3::Release( )
		{
			// Release the GL RC
			wglMakeCurrent( NULL, NULL );

			if( m_HGLRC )
			{
				wglDeleteContext( m_HGLRC );
				m_HGLRC = NULL;
			}
		}
		
		void WindowsRendererOGL3::SetView3D(
			const Arithmetic::Vector3 &p_Right,
			const Arithmetic::Vector3 &p_Up,
			const Arithmetic::Vector3 &p_Direction,
			const Arithmetic::Vector3 &p_Position )
		{
			// R U D P
			// R U D P
			// R U D P
			// 0 0 0 1
			
			m_View3D( 0, 0 ) = m_View3D( 1, 0 ) = m_View3D( 2, 0 ) = 0.0f;
			m_View3D( 3, 3 ) = 1.0f;

			m_View3D( 0, 0 ) = p_Right[ 0 ];
			m_View3D( 1, 0 ) = p_Right[ 1 ];
			m_View3D( 2, 0 ) = p_Right[ 2 ];

			m_View3D( 0, 1 ) = p_Up[ 0 ];
			m_View3D( 1, 1 ) = p_Up[ 1 ];
			m_View3D( 2, 1 ) = p_Up[ 2 ];

			m_View3D( 0, 2 ) = p_Direction[ 0 ];
			m_View3D( 1, 2 ) = p_Direction[ 1 ];
			m_View3D( 2, 2 ) = p_Direction[ 2 ];
			/*
			Arithmetic::Matrix3x3 Rotation;
			Rotation.SetRows( p_Right, p_Up, -p_Direction );

			// Transform translation
			Arithmetic::Vector3 PosInv = -( Rotation*p_Position );

			// Build 4x4 matrix
			Arithmetic::Matrix4x4 Mat4x4;
			Mat4x4.Rotate( Rotation );
			Mat4x4( 0, 3 ) = PosInv[ 0 ];
			Mat4x4( 1, 3 ) = PosInv[ 1 ];
			Mat4x4( 2, 3 ) = PosInv[ 2 ];

			m_View3D = Mat4x4;*/
			
			m_View3D( 0, 3 ) = p_Position[ 0 ];
			m_View3D( 1, 3 ) = p_Position[ 1 ];
			m_View3D( 2, 3 ) = p_Position[ 2 ];
		}

		void WindowsRendererOGL3::SetViewLookAt(
			const Arithmetic::Vector3 &p_Position,
			const Arithmetic::Vector3 &p_Point,
			const Arithmetic::Vector3 &p_WorldUp )
		{
			// Calculate the view vectors
			Arithmetic::Vector3 ViewDir;
			Arithmetic::Vector3 ViewRight;
			Arithmetic::Vector3 ViewUp;

			ViewDir = p_Point - p_Position;
			ViewDir.Normalise( );

			ViewUp = p_WorldUp - p_WorldUp.Dot( ViewDir )*ViewDir;
			ViewUp.Normalise( );

			ViewRight = ViewDir.Cross( ViewUp );

			// Call SetView3D to handle the rest
			SetView3D( ViewRight, ViewUp, ViewDir, p_Position );
		}

		void WindowsRendererOGL3::CalcViewProjMatrix( )
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

			Arithmetic::Matrix4x4 *p_pMat =
				( Arithmetic::Matrix4x4 * )&m_ViewProjection;

			( *p_pMat ) = ( *pMatA )*( *pMatB );
		}

		void WindowsRendererOGL3::CalcWorldViewProjMatrix( )
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

			/*Arithmetic::Matrix4x4 *pMatrix =
				( Arithmetic::Matrix4x4 * )&m_WorldViewProjection;

			( *pMatrix )*/
			m_WorldViewProjection = ( ( *pWorld )*( *pView ) )*( *pProjection );
		}

		void WindowsRendererOGL3::SetClippingPlanes( const ZED_FLOAT32 p_Near,
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

		void WindowsRendererOGL3::Prepare2D( )
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

		ZED_UINT32 WindowsRendererOGL3::CalcPerspProjMatrix(
			const ZED_FLOAT32 p_FOV,
			const ZED_FLOAT32 p_AspectRatio,
			Arithmetic::Matrix4x4 *p_pMatrix )
		{
			if( Arithmetic::Absolute( m_Far - m_Near ) < ZED_Epsilon )
			{
				return ZED_FAIL;
			}
			/*
			ZED_FLOAT32 SinFOV2, CosFOV2;
			Arithmetic::SinCos( ( p_FOV / 2.0f ), SinFOV2, CosFOV2 );

			if( Arithmetic::Absolute( SinFOV2 ) < ZED_Epsilon )
			{
				return ZED_FAIL;
			}

			ZED_FLOAT32 Width = p_AspectRatio * ( CosFOV2 / SinFOV2 );
			ZED_FLOAT32 Height = 1.0f * ( CosFOV2 / SinFOV2 );
			ZED_FLOAT32 Q = -( m_Far + m_Near ) / ( m_Far - m_Near );
			ZED_FLOAT32 Q2 = ( - 2 * m_Far * m_Near )/( m_Far - m_Near );

			memset( p_pMatrix, 0, sizeof( Arithmetic::Matrix4x4 ) );

			( *p_pMatrix )( 0, 0 ) = Width;
			( *p_pMatrix )( 1, 1 ) = Height;
			( *p_pMatrix )( 2, 2 ) = Q;
			( *p_pMatrix )( 2, 3 ) = -1.0f;
			( *p_pMatrix )( 3, 2 ) = Q2;
			( *p_pMatrix )( 3, 3 ) = 0.0f;*/

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

		ZED_UINT32 WindowsRendererOGL3::SetMode( const ZED_UINT32 p_Stage,
			const ZED_VIEWMODE p_Mode )
		{
			m_Stage = p_Stage;
			m_ViewMode = p_Mode;
			return ZED_OK;
		}
	}
}
