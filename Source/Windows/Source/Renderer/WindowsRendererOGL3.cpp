#include <Renderer/Renderer.hpp>
#include <Renderer/WindowsRendererOGL3.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <System/Debugger.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <System/Window.hpp>

namespace ZED
{
	namespace Renderer
	{
		WindowsRendererOGL3::WindowsRendererOGL3( )
		{
			memset( &m_PixelFormat, 0, sizeof( m_PixelFormat ) );
			memset( &m_Canvas, 0, sizeof( m_Canvas ) );

			m_pVertexCacheManager = ZED_NULL;
		}

		WindowsRendererOGL3::~WindowsRendererOGL3( )
		{
			if( m_pVertexCacheManager != ZED_NULL )
			{
				delete m_pVertexCacheManager;
				m_pVertexCacheManager;
			}

			// Release the GL RC (if not already done)
			wglMakeCurrent( NULL, NULL );

			if( m_GLRenderContext )
			{
				wglDeleteContext( m_GLRenderContext );
				m_GLRenderContext = NULL;
			}
		}

		ZED_UINT32 WindowsRendererOGL3::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			ZED::System::WINDOWDATA WinData = p_Window.WindowData( );

			m_DeviceContext = WinData.DeviceContext;

			if( !m_DeviceContext )
			{
				zedTrace( "[ZED::Renderer::WindowsRendererOGL3] <ERROR> "
					"Invalid device context\n" );

				return ZED_FAIL;
			}

			memcpy( &m_Canvas, &p_Canvas, sizeof( m_Canvas ) );

			switch( m_Canvas.ColourFormat( ) )
			{
				case ZED_FORMAT_ARGB8:
				{
					m_PixelFormat.cColorBits = 32;
					m_PixelFormat.iPixelType = PFD_TYPE_RGBA;
					break;
				}
				case ZED_FORMAT_RGB565:
				{
					m_PixelFormat.cColorBits = 15;
					m_PixelFormat.iPixelType = PFD_TYPE_RGBA;
				}
				default:
				{
					zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] "
						"<ERROR> Invalid canvas colour format\n" );
					return ZED_FAIL;
				}
			}

			switch( m_Canvas.DepthStencilFormat( ) )
			{
				case ZED_FORMAT_D24S8:
				{
					m_PixelFormat.cDepthBits = 24;
					m_PixelFormat.cStencilBits = 8;
					break;
				}	
				default:
				{
					zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] "
						"<ERROR> Invalid canvas depth/stencil format\n" );
					return ZED_FAIL;
				}
			}

			m_PixelFormat.nSize = sizeof( m_PixelFormat );
			m_PixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW |
				PFD_TYPE_RGBA;

			if( m_Canvas.BackBufferCount( ) > 0 )
			{
				m_PixelFormat.dwFlags |= PFD_DOUBLEBUFFER;
			}

			int PixelFormat = ChoosePixelFormat( m_DeviceContext,
				&m_PixelFormat );

			if( PixelFormat == 0 )
			{
				zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] "
					"<ERROR> Failed to choose a pixel format\n" );

				return ZED_FAIL;
			}

			BOOL PixelResult = SetPixelFormat( m_DeviceContext, PixelFormat,
				&m_PixelFormat );

			if( PixelResult == FALSE )
			{
				zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] "
					"<ERROR> Could not set pixel format\n" );
				return ZED_FAIL;
			}

			HGLRC TempContext = wglCreateContext( m_DeviceContext );

			if( wglMakeCurrent( m_DeviceContext, TempContext ) == FALSE )
			{
				zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] "
					"<ERROR> Could not make temporary OpenGL context "
					"current\n" );
				return ZED_FAIL;
			}

			m_GLExtender = GLExtender( m_DeviceContext );

			const int OpenGLVersions[ ] =
			{
				3, 0,
				3, 1,
				3, 2,
				3, 3
			};

			int OpenGLVersion[ 2 ];

			glGetIntegerv( GL_MAJOR_VERSION, &OpenGLVersion[ 0 ] );
			glGetIntegerv( GL_MINOR_VERSION, &OpenGLVersion[ 1 ] );

			zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] <INFO> "
				"OpenGL version in use: %d.%d\n", OpenGLVersion[ 0 ],
				OpenGLVersion[ 1 ] );

			m_GLVersion.Major = OpenGLVersion[ 0 ];
			m_GLVersion.Minor = OpenGLVersion[ 1 ];

			m_GLExtender.Initialise( m_GLVersion );

			int Major = OpenGLVersions[
				( sizeof( OpenGLVersions ) / sizeof( OpenGLVersions[ 0 ] ) ) -
				2 ];
			int Minor = OpenGLVersions[
				( sizeof( OpenGLVersions ) / sizeof( OpenGLVersions[ 0 ] ) ) -
				1 ];

			int GLAttribs[ ] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, Major,
				WGL_CONTEXT_MINOR_VERSION_ARB, Minor,
#if ZED_BUILD_DEBUG
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
				0
			};

			if( m_GLExtender.IsWindowExtSupported( "WGL_ARB_create_context" )
				== ZED_TRUE )
			{
				for( int i = 1;	i < ( sizeof( OpenGLVersions ) /
					sizeof( OpenGLVersions[ 0 ] ) )	/ 2; ++i )
				{
					m_GLRenderContext = zglCreateContextAttribs(
						m_DeviceContext, 0, GLAttribs );
					wglMakeCurrent( NULL, NULL );
					wglDeleteContext( TempContext );
					wglMakeCurrent( m_DeviceContext, m_GLRenderContext );

					if( !m_GLRenderContext )
					{
						zedTrace(
							"[ZED::Renderer::WindowsRendererOGL3::Create] "
							"<WARN> Failed to create OpenGL render context "
							"for version %d.%d\n", Major, Minor );
					}
					else
					{
						int CurrentMajor, CurrentMinor;
						glGetIntegerv( GL_MAJOR_VERSION, &CurrentMajor );
						glGetIntegerv( GL_MINOR_VERSION, &CurrentMinor );

						zedTrace(
							"[ZED::Renderer::WindowsRendererOGL3::Create] "
							"<INFO> Created OpenGL context for version "
							"%d.%d\n", CurrentMajor, CurrentMinor );
						break;
					}

					Major = OpenGLVersions[ ( sizeof( OpenGLVersions ) /
						sizeof( OpenGLVersions[ 0 ] ) ) - ( 2 + ( i * 2 ) ) ];
					Minor = OpenGLVersions[ ( sizeof( OpenGLVersions ) /
						sizeof( OpenGLVersions[ 0 ] ) ) - ( 1 + ( i * 2 ) ) ];

					GLAttribs[ 1 ] = Major;
					GLAttribs[ 3 ] = Minor;
				}
			}
			else
			{
				zedTrace( "[ZED::Renderer::WindowsRendererOGL3::Create] "
					"<ERROR> No support for creating an OpenGL context with "
					"attributes, cannot guarantee an OpenGL 3.x renderer\n" );

				return ZED_FAIL;
			}

			this->ResizeCanvas( m_Canvas.Width( ), m_Canvas.Height( ) );

			m_pVertexCacheManager = new GLVertexCacheManager( );
			
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
			SwapBuffers( m_DeviceContext );
		}

		void WindowsRendererOGL3::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			zglClearColor( p_Red, p_Green, p_Blue, 1.0f );
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
			if( ( p_Width == m_Canvas.Width( ) ) &&
				( p_Height == m_Canvas.Height( ) ) )
			{
				return ZED_FALSE;
			}

			m_Canvas.Width( p_Width );
			m_Canvas.Height( p_Height );

			// Get the aspect ratio for the window
			m_Canvas.AspectRatio(
				static_cast< ZED_FLOAT32 >( m_Canvas.Width( ) ) /
				static_cast< ZED_FLOAT32 >( m_Canvas.Height( ) ) );

			glViewport( 0, 0, m_Canvas.Width( ), m_Canvas.Height( ) );

			return ZED_TRUE;
		}

		void WindowsRendererOGL3::Release( )
		{
			wglMakeCurrent( NULL, NULL );

			if( m_GLRenderContext )
			{
				wglDeleteContext( m_GLRenderContext );
				m_GLRenderContext = NULL;
			}
		}
	
		ZED_UINT32 WindowsRendererOGL3::Render(
			const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
			const ZED_UINT32 p_MaterialID )
		{
/*			m_pVertexCacheManager->Render( p_VertexCount, p_pVertices,
				p_IndexCount, p_pIndices, p_Attributes, p_MaterialID,
				p_PrimitiveType );
			m_pVertexCacheManager->ForceFlushAll( );*/

			return ZED_OK;
		}

		void WindowsRendererOGL3::RenderState( const ZED_RENDERSTATE p_State,
			const ZED_MEMSIZE p_Value )
		{
		}
	}
}
