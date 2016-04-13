#include <Renderer/LinuxRendererOGL2.hpp>
#include <System/Memory.hpp>
#include <System/WindowX11.hpp>
#include <Renderer/OGL/LinuxGLExtender.hpp>
#include <Renderer/OGL/GLVertexCacheManager.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/MaterialManager.hpp>
#include <cstring>
#include <string>
#include <GL/glx.h>

namespace ZED
{
	namespace Renderer
	{
		LinuxRendererOGL2::LinuxRendererOGL2( ) :
			m_ClearFlags( 0 ),
			m_pGLExtender( ZED_NULL ),
			m_pVertexCacheManager( ZED_NULL ),
			m_pMaterialManager( ZED_NULL ),
			m_BackbufferID( 0 ),
			m_TakeScreenshot( ZED_FALSE ),
			m_pScreenshotFileName( ZED_NULL )
		{
			// All versions of OpenGL >=2 support shaders
			Renderer::m_ShaderSupport = ZED_TRUE;
			Renderer::m_BackEnd = ZED_RENDERER_BACKEND_OPENGL;
		}

		LinuxRendererOGL2::~LinuxRendererOGL2( )
		{
			zedSafeDelete( m_pScreenshotFileName );

			if( m_BackbufferID != 0 )
			{
				zglDeleteBuffers( 1, &m_BackbufferID );
			}

			zedSafeDelete( m_pGLExtender );
			zedSafeDelete( m_pVertexCacheManager );
			zedSafeDelete( m_pMaterialManager );

			// Unbind GLX
			if( m_pWindowData->GetDisplay( ) )
			{
				glXMakeCurrent( m_pWindowData->GetDisplay( ), 0, 0 );

				if( m_GLContext )
				{
					glXDestroyContext( m_pWindowData->GetDisplay( ),
						m_GLContext );
				}
			}

			zedSafeDelete( m_pWindowData );
		}

		ZED_UINT32 LinuxRendererOGL2::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			System::WindowData *pWindowData = new System::LinuxWindowData( );
			System::Window *pWindowRaw =
				const_cast< System::Window * >( &p_Window );
			System::LinuxWindow *pLinuxWindow =
				reinterpret_cast< System::LinuxWindow * >( pWindowRaw );

			if( p_Window.GetWindowData( &pWindowData ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to obtain the window data from the window "
					"parameter\n" );

				return ZED_FAIL;
			}

			m_pWindowData = new System::LinuxWindowData( );
			memcpy( m_pWindowData, pWindowData, sizeof( *m_pWindowData ) );

			if( m_pWindowData->GetDisplay( ) == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Display not initialised\n" );

				return ZED_FAIL;
			}

			// GLX 1.3 or greater is needed
			int GLXMajor, GLXMinor;

			if( !glXQueryVersion( m_pWindowData->GetDisplay( ), &GLXMajor,
				&GLXMinor ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to call glXQueryVersion\n" );

				return ZED_FAIL;
			}

			if( ( ( GLXMajor == 1 ) && GLXMinor < 3 ) || GLXMajor < 1 )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to get GLX version 1.3 or greater, instead "
					"version %d.%d was discovered\n", GLXMajor, GLXMinor );

				return ZED_FAIL;
			}

			m_Canvas = p_Canvas;

			int GLDepth = 0, GLStencil = 0, GLAlpha = 0, GLRed = 0,
				GLGreen = 0, GLBlue = 0;

			switch( m_Canvas.DepthStencilFormat( ) )
			{
				case ZED_FORMAT_D24S8:
				{
					GLDepth = 24;
					GLStencil = 8;
					break;
				}
				default:
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] "
						"<ERROR> Unknown depth/stencil format\n" );

					return ZED_FAIL;
				}
			}

			switch( m_Canvas.ColourFormat( ) )
			{
				case ZED_FORMAT_ARGB8:
				{
					GLAlpha = GLRed = GLGreen = GLBlue = 8;
					break;
				}
				case ZED_FORMAT_RGB565:
				{
					GLAlpha = 0;
					GLRed = GLBlue = 5;
					GLGreen = 6;
					break;
				}
				default:
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] "
						"<ERROR> Unknown colour format\n" );
					return ZED_FAIL;
				}
			}

			int AntiAliasing = 0, AntiAliasingCount = 0;

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

			int VisualAttributes[ ] =
			{
				GLX_X_RENDERABLE,	True,
				GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
				GLX_RENDER_TYPE,	GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
				GLX_RED_SIZE,		GLRed,
				GLX_GREEN_SIZE,		GLGreen,
				GLX_BLUE_SIZE,		GLBlue,
				GLX_ALPHA_SIZE,		GLAlpha,
				GLX_DEPTH_SIZE,		GLDepth,
				GLX_STENCIL_SIZE,	GLStencil,
				GLX_DOUBLEBUFFER,	( m_Canvas.BackBufferCount( ) > 0 ?
					True : False ),
				GLX_SAMPLE_BUFFERS,	AntiAliasing,
				GLX_SAMPLES,		AntiAliasingCount,
				None
			};

			int FramebufferCount = 0;

			GLXFBConfig *pGLFramebufferConfig = glXChooseFBConfig(
				m_pWindowData->GetDisplay( ),
				m_pWindowData->GetScreenNumber( ), VisualAttributes,
				&FramebufferCount );

			if( !FramebufferCount )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to find a suitable framebuffer configuration\n" );

				return ZED_FAIL;
			}

			GLXFBConfig GLFramebufferConfig = pGLFramebufferConfig[ 0 ];

			XFree( pGLFramebufferConfig );
			pGLFramebufferConfig = ZED_NULL;

			XVisualInfo *pXVisualInfo = glXGetVisualFromFBConfig(
				m_pWindowData->GetDisplay( ), GLFramebufferConfig );

			if( pLinuxWindow->SetXVisualInfo( pXVisualInfo ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to set XVisualInfo\n" );

				return ZED_FAIL;
			}

			// Get the window data again, after setting the XVisualInfo
			if( p_Window.GetWindowData( &pWindowData ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to obtain the window data from the window "
					"parameter\n" );

				return ZED_FAIL;
			}

			memcpy( m_pWindowData, pWindowData, sizeof( *m_pWindowData ) );

			// Determine that an OpenGL 2.x context was created
			m_GLContext = glXCreateContext( m_pWindowData->GetDisplay( ),
				pXVisualInfo, 0, True );
			glXMakeCurrent( m_pWindowData->GetDisplay( ),
				m_pWindowData->GetWindow( ), m_GLContext );

			std::string GLVersionString( reinterpret_cast< const char * >(
				glGetString( GL_VERSION ) ) );

			ZED_MEMSIZE GLDelimiter = GLVersionString.find_first_of( '.' );

			if( GLDelimiter == std::string::npos )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Could not find OpenGL version string delimiter\n" );

				return ZED_FAIL;
			}

			std::string GLMajor = GLVersionString.substr( 0, GLDelimiter );
			int GLMinor = 1;

			if( atoi( GLMajor.c_str( ) ) != 2 )
			{
				PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs =
					reinterpret_cast< PFNGLXCREATECONTEXTATTRIBSARBPROC >(
						glXGetProcAddress(
							reinterpret_cast< const GLubyte * >(
								"glXCreateContextAttribsARB" ) ) );

				if( glXCreateContextAttribs == ZED_NULL )
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] "
						"<ERROR> Failed to create an OpenGL 2 context\n" );

					return ZED_FAIL;
				}

				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <WARN> "
					"OpenGL version not supported, attempting to use "
					"glXCreateContextAttribsARB to get a 2.x context\n" );

				int ContextAttributes[ ] =
				{
					GLX_CONTEXT_MAJOR_VERSION_ARB,	2,
					GLX_CONTEXT_MINOR_VERSION_ARB,	GLMinor,
					None
				};

				ZED_BOOL ContextCreated = ZED_FALSE;

				while( ( GLMinor >= 0 ) && ( ContextCreated != ZED_TRUE ) )
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] "
						"<INFO> Attempting to create an OpenGL 2.%d context\n",
						GLMinor );

					m_GLContext = glXCreateContextAttribs(
						m_pWindowData->GetDisplay( ), GLFramebufferConfig, 0,
						True, ContextAttributes );

					if( m_GLContext == ZED_NULL )
					{
						--GLMinor;
						ContextAttributes[ 3 ] = GLMinor;

						zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] "
							"<WARN> Failed to create context\n" );

						continue;
					}

					ContextCreated = ZED_TRUE;
				}

				if( ContextCreated == ZED_FALSE )
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] "
						"<ERROR> Failed to create an OpenGL 2 context\n" );

					return ZED_FAIL;
				}
			}

			int MakeContextCurrent = glXMakeCurrent(
				m_pWindowData->GetDisplay( ), m_pWindowData->GetWindow( ),
				m_GLContext );

			if( MakeContextCurrent == 0 ) // GLXBadContext
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <ERROR> "
					"Failed to make the OpenGL context current\n" );

				return ZED_FAIL;
			}

			if( MakeContextCurrent == True )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <INFO> "
					"Created an OpenGL 2.%d context\n", GLMinor );
			}

			XSync( m_pWindowData->GetDisplay( ), False );

			if( !glXIsDirect( m_pWindowData->GetDisplay( ), m_GLContext ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::Create] <WARN> "
					"Indirect GLX context\n" );
			}

			m_pGLExtender = new LinuxGLExtender( *m_pWindowData );

			if( m_pGLExtender == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2] <ERROR> "
					"Failed to create an instance of the OpenGL extension "
					"manager\n" );

				return ZED_FAIL;
			}

			ZED_GLVERSION GLVersion;
			GLVersion.Major = 2;
			GLVersion.Minor = GLMinor;

			if( m_pGLExtender->Initialise( GLVersion ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			this->ResizeCanvas( m_Canvas.Width( ), m_Canvas.Height( ) );

			return ZED_OK;
		}

		void LinuxRendererOGL2::Clear( )
		{
			glClear( m_ClearFlags );
		}

		void LinuxRendererOGL2::SwapBuffers( )
		{
			glXSwapBuffers( m_pWindowData->GetDisplay( ),
				m_pWindowData->GetWindow( ) );
		}

		void LinuxRendererOGL2::SetClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			glClearColor( p_Red, p_Green, p_Blue, 1.0f );
		}

		void LinuxRendererOGL2::SetClearFlags( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
			m_ClearFlags = 0;

			if( p_Colour == ZED_TRUE )
			{
				m_ClearFlags |= GL_COLOR_BUFFER_BIT;
			}

			if( p_Depth == ZED_TRUE )
			{
				m_ClearFlags |= GL_DEPTH_BUFFER_BIT;
			}

			if( p_Stencil == ZED_TRUE )
			{
				m_ClearFlags |= GL_STENCIL_BUFFER_BIT;
			}
		}

		ZED_UINT32 LinuxRendererOGL2::ResizeCanvas( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			if( ( p_Width == m_Canvas.Width( ) ) &&
				( p_Height == m_Canvas.Height( ) ) )
			{
				return ZED_OK;
			}

			if( ( p_Width == 0UL || ( p_Height == 0UL ) ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL2::ResizeCanvas] "
					"<ERROR> Width or height of canvas is zero\n" );

				return ZED_FAIL;
			}

			m_Canvas.Width( p_Width );
			m_Canvas.Height( p_Height );

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

			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL2::CreateMesh(
			const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
			const ZED_UINT32 p_MaterialID,
			const ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL2::Screenshot( const ZED_CHAR8 *p_pFileName,
			const ZED_BOOL p_RelativeToExecutable )
		{
			return ZED_OK;
		}

		void LinuxRendererOGL2::SetRenderState( const ZED_RENDERSTATE p_State,
			const ZED_UINT32 p_Value )
		{
		}

		ZED_UINT32 LinuxRendererOGL2::AddMaterial(
			ZED::Renderer::Material * const &p_pMaterial )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL2::GetMaterial(
			const ZED_UINT32 p_MaterialID,
			ZED::Renderer::Material *p_pMaterial ) const
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL2::GetMaterial(
			ZED_CHAR8 * const &p_pMaterialName,
			ZED::Renderer::Material *p_pMaterial ) const
		{
			return ZED_OK;
		}
	}
}

