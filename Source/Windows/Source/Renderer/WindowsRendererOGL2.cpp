#include <Renderer/WindowsRendererOGL2.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <System/Debugger.hpp>

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

		ZED_UINT32 WindowsRendererOGL2::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			// Grab the canvas for later
			m_Canvas = p_Canvas;

			if( !m_HDC )
			{
				zedTrace( "[ZED::Renderer::WindowsRendererOGL2::Create]"
					" <ERROR> The HDC has not been set.\n" );
				return ZED_FAIL;
			}

			switch( m_Canvas.ColourFormat( ) )
			{
			case ZED_FORMAT_ARGB8:
				{
					m_PixelFormat.cColorBits = 32;
					m_PixelFormat.iPixelType = PFD_TYPE_RGBA;
					break;
				}
			default:
				{
					zedTrace( "[ZED::Renderer::WindowsRendererOGL2::Create]"
						" <ERROR> Unsupported colour depth.\n" );
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
			ZED_SINT32 OpenGLVersion[ 2 ];

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

			// Set up the viewport
			ResizeCanvas( m_Canvas.Width( ), m_Canvas.Height( ) );

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
			zglClearColor( p_Red, p_Green, p_Blue, 1.0f );
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

		ZED_UINT32 WindowsRendererOGL2::Render(
			const ZED_MEMSIZE p_VertexCount, const ZED_BYTE *p_pVertices,
			const ZED_MEMSIZE p_IndexCount, const ZED_UINT16 *p_pIndices,
			const ZED_UINT64 p_Attributes, const ZED_UINT32 p_MaterialID )
		{
			return ZED_OK;
		}

		void WindowsRendererOGL2::RenderState(
			const ZED_RENDERSTATE p_State, const ZED_MEMSIZE p_Value )
		{
		}
	}
}
