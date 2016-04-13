#include <Renderer/LinuxRendererOpenGL.hpp>
#include <System/X11WindowData.hpp>
#include <System/X11Window.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <Renderer/OGL/LinuxGLExtender.hpp>
#include <Renderer/OGL/GLVertexCacheManager.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/MaterialManager.hpp>
#include <Renderer/RendererFactory.hpp>
#include <System/Memory.hpp>
#include <System/NativeFile.hpp>
#include <Renderer/Targa.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		LinuxRendererOpenGL::LinuxRendererOpenGL(
			const ZED_SINT32 p_MajorVersion,
			const ZED_SINT32 p_MinorVersion ) :
			m_pGLExtender( ZED_NULL ),
			m_pWindowData( ZED_NULL ),
			m_GLXContext( ZED_NULL ),
			m_ClearFlags( 0 ),
			m_pVertexCacheManager( ZED_NULL ),
			m_pMaterialManager( ZED_NULL ),
			m_BackBufferID( 0 ),
			m_TakeScreenshot( ZED_FALSE ),
			m_pScreenshotFileName( ZED_NULL ),
			m_MajorVersion( p_MajorVersion ),
			m_MinorVersion( p_MinorVersion )
		{
			// This will be probed when creating the renderer
			m_ShaderSupport = ZED_FALSE;
			m_BackEnd = ZED_RENDERER_BACKEND_OPENGL;
		}

		LinuxRendererOpenGL::~LinuxRendererOpenGL( )
		{
		}

		ZED_UINT32 LinuxRendererOpenGL::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			System::WindowData *pWindowData = new System::X11WindowData( );
			System::Window *pWindowRaw =
				const_cast< System::Window * >( &p_Window );
			System::LinuxWindow *pLinuxWindow =
				reinterpret_cast< System::LinuxWindow * >( pWindowRaw );

			if( p_Window.GetWindowData( &pWindowData ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to obtain the window data from window\n" );

				return ZED_FAIL;
			}

			zedSafeDelete( m_pWindowData );
			m_pWindowData = new System::X11WindowData( );

			memcpy( m_pWindowData, pWindowData,
				sizeof( System::X11WindowData ) );

			if( m_pWindowData->GetDisplay( ) == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Display not initialised\n" );

				return ZED_FAIL;
			}

			ZED_SINT32 GLXMajor = 0, GLXMinor = 0;

			if( !glXQueryVersion( m_pWindowData->GetDisplay( ), &GLXMajor,
				&GLXMinor ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to call glXQueryVersion\n" );

				return ZED_FAIL;
			}

			if( ( ( GLXMajor == 1 ) && ( GLXMinor < 3 ) ) || GLXMajor < 1 )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to get GLX version 1.3 or greater, "
					"instead version %d.%d was discovered\n", GLXMajor,
					GLXMinor );

				return ZED_FAIL;
			}

			m_Canvas = p_Canvas;

			ZED_SINT32 DepthSize = 0, StencilSize = 0, AlphaSize = 0,
				RedSize = 0, GreenSize = 0, BlueSize = 0;

			switch( m_Canvas.DepthStencilFormat( ) )
			{
				case ZED_FORMAT_D24S8:
				{
					DepthSize = 24;
					StencilSize = 8;
					break;
				}
			}

			switch( m_Canvas.ColourFormat( ) )
			{
				case ZED_FORMAT_ARGB8:
				{
					AlphaSize = 8;
					RedSize = 8;
					GreenSize = 8;
					BlueSize = 8;

					break;
				}
				case ZED_FORMAT_RGB565:
				{
					AlphaSize = 0;
					RedSize = 5;
					GreenSize = 6;
					BlueSize = 5;

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
			}

			int VisualAttributes[ ] =
			{
				GLX_X_RENDERABLE,	True,
				GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
				GLX_RENDER_TYPE,	GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
				GLX_RED_SIZE,		RedSize,
				GLX_GREEN_SIZE,		GreenSize,
				GLX_BLUE_SIZE,		BlueSize,
				GLX_ALPHA_SIZE,		AlphaSize,
				GLX_DEPTH_SIZE,		DepthSize,
				GLX_STENCIL_SIZE,	StencilSize,
				GLX_DOUBLEBUFFER,	( m_Canvas.BackBufferCount( ) > 0 ?
					True : False ),
				GLX_SAMPLE_BUFFERS,	AntiAliasing,
				GLX_SAMPLES,		AntiAliasingCount,
				None
			};

			ZED_SINT32 FramebufferCount = 0;

			GLXFBConfig *pGLXFBConfig = glXChooseFBConfig(
				m_pWindowData->GetDisplay( ),
				m_pWindowData->GetScreenNumber( ), VisualAttributes,
				&FramebufferCount );

			if( pGLXFBConfig == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to find a suitable framebuffer "
					"configuration\n" );

				return ZED_FAIL;
			}

			GLXFBConfig FramebufferConfig = pGLXFBConfig[ 0 ];

			XFree( pGLXFBConfig );
			pGLXFBConfig = ZED_NULL;

			XVisualInfo *pXVisualInfo = glXGetVisualFromFBConfig(
				m_pWindowData->GetDisplay( ), FramebufferConfig );

			if( pLinuxWindow->SetXVisualInfo( pXVisualInfo ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to set window's XVisualInfo" );

				return ZED_OK;
			}

			if( p_Window.GetWindowData( &pWindowData ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to acquire the window data\n" );

				return ZED_FAIL;
			}

			memcpy( m_pWindowData, pWindowData,
				sizeof( System::X11WindowData ) );

			ZED_GLVERSION GLVersion;

			GetHighestSupportedOpenGLVersion( GLVersion );

			if( m_MajorVersion > GLVersion.Major )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> OpenGL version requested [%d.%d] is beyond the "
					"capabilities of the current GPU [%d.%d.]\n",
					m_MajorVersion, m_MinorVersion, GLVersion.Major,
					GLVersion.Minor );

				return ZED_FAIL;
			}

			if( m_MinorVersion > GLVersion.Minor )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> OpenGL minor version requested is greater than "
					"the supported minor version\n" );

				return ZED_FAIL;
			}

			int GLMajor = m_MajorVersion, GLMinor = m_MinorVersion < 0 ?
				GLVersion.Minor : m_MinorVersion;

			// Set the maximum OpenGL minor values in case the GPU driver
			// supports a version higher than that requested
			switch( GLMajor )
			{
				case 1:
				{
					if( GLVersion.Major == 1 )
					{
						break;
					}
					GLMinor = 5;
					break;
				}
				case 2:
				{
					if( GLVersion.Major == 2 )
					{
						break;
					}
					GLMinor = 1;
					break;
				}
			}

			// If glXCreateContextAttribs is not available, OpenGL >=3.x
			// context creation should not continue.  For OpenGL < 3, the
			// highest version available will be used.
			PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs =
				ZED_NULL;
			
			if( ( glXCreateContextAttribs =
				( PFNGLXCREATECONTEXTATTRIBSARBPROC )glXGetProcAddress(
					( const GLubyte * )"glXCreateContextAttribsARB" ) ) ==
				ZED_NULL )
			{
				if( m_MajorVersion >= 3 )
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
						"<ERROR> glXCreateContextAttribsARB not supported, "
						"required for OpenGL 3.0 or higher\n" );

					return ZED_FAIL;
				}

				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<WARN> glXCreateContextAttribsARB not supported, "
					"OpenGL verison acquired will be %d.%d\n", GLVersion.Major,
					GLVersion.Minor );

				m_GLXContext = glXCreateNewContext(
					m_pWindowData->GetDisplay( ), FramebufferConfig,
					GLX_RGBA_TYPE, ZED_NULL, True );
			}
			else
			{
				int GLXContextAttributes[ ] =
				{
					GLX_CONTEXT_MAJOR_VERSION_ARB,	GLMajor,
					GLX_CONTEXT_MINOR_VERSION_ARB,	GLMinor,
#if defined ZED_BUILD_DEBUG
					GLX_CONTEXT_FLAGS_ARB,			GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
					None
				};

				m_GLXContext = glXCreateContextAttribs(
					m_pWindowData->GetDisplay( ), FramebufferConfig, ZED_NULL,
					True, GLXContextAttributes );
			}

			if( m_GLXContext == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to create an OpenGL %d.%d context\n",
					GLMajor, GLMinor );

				return ZED_FAIL;
			}

			if( glXMakeCurrent( m_pWindowData->GetDisplay( ),
				m_pWindowData->GetWindow( ), m_GLXContext ) == 0 )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to make the GL context current\n" );

				return ZED_FAIL;
			}

			XSync( m_pWindowData->GetDisplay( ), False );

			if( !glXIsDirect( m_pWindowData->GetDisplay( ), m_GLXContext ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<WARN> GLX context is not direct\n" );

				return ZED_FAIL;
			}

			zedSafeDelete( m_pGLExtender );
			m_pGLExtender = new LinuxGLExtender( *m_pWindowData );

			if( m_pGLExtender->Initialise( GLVersion ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::Create] "
					"<ERROR> Failed to acquire the OpenGL extensions\n" );

				return ZED_FAIL;
			}

			this->ResizeCanvas( m_Canvas.Width( ), m_Canvas.Height( ) );
			m_pVertexCacheManager = new GLVertexCacheManager( );
			m_pMaterialManager = new MaterialManager( );

			m_ShaderSupport = ZED_FALSE;
			// Query for shader support, if the version is 2.x or greater,
			// shader support is built-in
			if( m_MajorVersion >= 2 )
			{
				m_ShaderSupport = ZED_TRUE;
			}
			else
			{
				// ARB or nVIDIA shader type
				if( ( m_pGLExtender->IsGLExtensionSupported(
						"ARB_vertex_shader" ) == ZED_TRUE ) ||
					( m_pGLExtender->IsGLExtensionSupported(
						"NV_vertex_program" ) == ZED_TRUE ) ||
					( m_pGLExtender->IsGLExtensionSupported(
						"ARB_vertex_program" ) == ZED_TRUE ) )
				{
					m_ShaderSupport = ZED_TRUE;
				}
			}
			
			return ZED_OK;
		}

		void LinuxRendererOpenGL::Clear( )
		{
			zglClear( m_ClearFlags );
		}

		void LinuxRendererOpenGL::SwapBuffers( )
		{
			glXSwapBuffers( m_pWindowData->GetDisplay( ),
				m_pWindowData->GetWindow( ) );
		}

		void LinuxRendererOpenGL::SetClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			zglClearColor( p_Red, p_Green, p_Blue, 1.0f );
		}

		void LinuxRendererOpenGL::SetClearFlags( const ZED_BOOL p_Colour,
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

		ZED_UINT32 LinuxRendererOpenGL::ResizeCanvas( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			if( p_Width == 0 || p_Height == 0 )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOpenGL::ResizeCanvas] "
					"<ERROR> Width and/or height is zero\n" );

				return ZED_FAIL;
			}

			if( ( p_Width == m_Canvas.Width( ) ) &&
				( p_Height == m_Canvas.Height( ) ) )
			{
				return ZED_OK;
			}

			m_Canvas.Width( p_Width );
			m_Canvas.Height( p_Height );

			if( p_Width > p_Height )
			{
				m_Canvas.AspectRatio(
					static_cast< ZED_FLOAT32 >( p_Width ) /
					static_cast< ZED_FLOAT32 >( p_Height ) );
			}
			else
			{
				m_Canvas.AspectRatio(
					static_cast< ZED_FLOAT32 >( p_Height ) /
					static_cast< ZED_FLOAT32 >( p_Width ) );
			}

			zglViewport( 0, 0, p_Width, p_Height );


			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOpenGL::CreateMesh(
			const ZED_MEMSIZE p_VertexCount, const ZED_BYTE *p_pVertices,
			const ZED_MEMSIZE p_IndexCount, const ZED_UINT16 *p_pIndices,
			const ZED_UINT64 p_Attributes, const ZED_UINT32 p_MaterialID,
			const ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 LinuxRendererOpenGL::Screenshot(
			const ZED_CHAR8 *p_pFileName,
			const ZED_BOOL p_RelativeToExecutalbe )
		{
			return ZED_FAIL;
		}

		void LinuxRendererOpenGL::SetRenderState(
			const ZED_RENDERSTATE p_RenderState, const ZED_UINT32 p_Value )
		{
		}

		ZED_UINT32 LinuxRendererOpenGL::AddMaterial(
			Material * const &p_pMaterial )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 LinuxRendererOpenGL::GetMaterial(
			const ZED_UINT32 p_MaterialID, Material *p_pMaterial ) const
		{
			return ZED_FAIL;
		}

		ZED_UINT32 LinuxRendererOpenGL::GetMaterial(
			ZED_CHAR8 * const &p_pMaterialName, Material *p_pMaterial ) const
		{
			return ZED_FAIL;
		}
	}
}

