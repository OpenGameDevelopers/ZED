#include <Renderer/LinuxRendererOGL3.hpp>
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
		LinuxRendererOGL3::LinuxRendererOGL3( ) :
			m_ClearFlags( 0 ),
			m_pGLExtender( ZED_NULL ),
			m_pVertexCacheManager( ZED_NULL ),
			m_pMaterialManager( ZED_NULL ),
			m_BackbufferID( 0 ),
			m_TakeScreenshot( ZED_FALSE ),
			m_pScreenshotFileName( ZED_NULL ),
			m_MinorVersion( -1 )
		{
			// All versions of OpenGL >=3 are shader-based
			Renderer::m_ShaderSupport = ZED_TRUE;
			m_BackEnd = ZED_RENDERER_BACKEND_OPENGL;
		}

		LinuxRendererOGL3::LinuxRendererOGL3(
			const ZED_SINT32 p_MinorVersion ) :
			m_ClearFlags( 0 ),
			m_pGLExtender( ZED_NULL ),
			m_pVertexCacheManager( ZED_NULL ),
			m_pMaterialManager( ZED_NULL ),
			m_BackbufferID( 0 ),
			m_TakeScreenshot( ZED_FALSE ),
			m_pScreenshotFileName( ZED_NULL ),
			m_MinorVersion( p_MinorVersion )
		{
			Renderer::m_ShaderSupport = ZED_TRUE;
			m_BackEnd = ZED_RENDERER_BACKEND_OPENGL;
		}

		LinuxRendererOGL3::~LinuxRendererOGL3( )
		{
			zedSafeDelete( m_pScreenshotFileName );
			zglDeleteBuffers( 1, &m_BackbufferID );

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

		ZED_UINT32 LinuxRendererOGL3::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			System::WindowData *pWindowData =
				new System::X11WindowData( );
			System::Window *pWindowRaw =
				const_cast< System::Window * >( &p_Window );
			System::LinuxWindow *pLinuxWindow =
				reinterpret_cast< System::LinuxWindow * >( pWindowRaw );

			if( p_Window.GetWindowData( &pWindowData ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to obtain the window data from window\n" );
				return ZED_FAIL;
			}

			m_pWindowData = new System::X11WindowData( );

			memcpy( m_pWindowData, pWindowData,
				sizeof( System::X11WindowData ) );

			if( m_pWindowData->GetDisplay( ) == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Display not initialised\n" );
				return ZED_FAIL;
			}

			// Before continuing, ensure that GLX 1.3 or greater is supported
			ZED_SINT32 GLXMajor = 0, GLXMinor = 0;

			if( !glXQueryVersion( m_pWindowData->GetDisplay( ), &GLXMajor,
				&GLXMinor ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to call glXQueryVersion\n" );
				return ZED_FAIL;
			}

			if( ( ( GLXMajor == 1 ) && ( GLXMinor < 3 ) ) || GLXMajor < 1 )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERORR> "
					"Failed to get GLX version 1.3 or greater, instead "
					"version %d.%d was discovered\n", GLXMajor, GLXMinor );
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

			int VisualAttributes[  ] =
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
				GLX_SAMPLES,		AntiAliasingCount,
				None
			};

			ZED_SINT32 FBCount = 0;

			GLXFBConfig *pGLFBC = glXChooseFBConfig(
				m_pWindowData->GetDisplay( ),
				m_pWindowData->GetScreenNumber( ),
				VisualAttributes, &FBCount );

			if( !pGLFBC )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to find a suitable framebuffer configuration\n" );

				return ZED_FAIL;
			}

			GLXFBConfig FBConfig = pGLFBC[ 0 ];

			XFree( pGLFBC );
			pGLFBC = ZED_NULL;

			XVisualInfo *pXVisualInfo = glXGetVisualFromFBConfig(
				m_pWindowData->GetDisplay( ), FBConfig );
			
			if( pLinuxWindow->SetXVisualInfo( pXVisualInfo ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to set XVisualInfo\n" );

				return ZED_FAIL;
			}

			if( p_Window.GetWindowData( &pWindowData ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to obtain the window data from window\n" );

				return ZED_FAIL;
			}

			memcpy( m_pWindowData, pWindowData,
				sizeof( System::X11WindowData ) );


			ZED_GLVERSION GLVersion;

			GetHighestSupportedOpenGLVersion( GLVersion );

			if( GLVersion.Major < 3 )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3] <ERROR> "
					"Maximum OpenGL version is %d\n", GLVersion.Major );

				return ZED_GRAPHICS_ERROR;
			}

			if( ( GLVersion.Major ) > 3 && ( m_MinorVersion != -1 ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3] <INFO> "
					"OpenGL version is %d.%d, attempting to create a 3.3 "
					" context\n", GLVersion.Major, GLVersion.Minor );

				GLVersion.Minor = 3;
			}

			if( ( GLVersion.Major == 3 ) &&
				( m_MinorVersion > GLVersion.Major ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3] <ERROR> "
					"Requested minor version greater than that supported by "
					"graphics device\n" );

				return ZED_GRAPHICS_ERROR;
			}

			GLVersion.Major = 3;

			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				" Getting GLX Extensions.\n" );
			
			PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = 
				ZED_NULL;
			glXCreateContextAttribs = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
				glXGetProcAddress(
				( const GLubyte * )"glXCreateContextAttribsARB" );

			ZED_SINT32 Major = GLVersion.Major;
			ZED_SINT32 Minor;

			if( m_MinorVersion != -1 )
			{
				Minor = m_MinorVersion;
			}
			else
			{
				Minor = GLVersion.Minor;
			}

			// Rather than hard-code the Major and Minor, it should be
			// determined from the available OpenGL implementation
			ZED_SINT32 ContextAttribs[ ] =
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB,	Major,
				GLX_CONTEXT_MINOR_VERSION_ARB,	Minor,
#if ZED_BUILD_DEBUG
				GLX_CONTEXT_FLAGS_ARB,	GLX_CONTEXT_DEBUG_BIT_ARB,
#endif
				None
			};

			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				"Setting up OpenGL context.\n" );

			// Attempt to create an OGL context for the highest OGL 3 version
			// Going down minor versions until zero is reached
			ZED_BOOL ContextCreated = ZED_FALSE;
			if( glXCreateContextAttribs != ZED_NULL )
			{
				while( Minor > -1 && ContextCreated != ZED_TRUE )
				{
					zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
						"<INFO> Attempting to create context for "
						"OpenGL %d.%d\n", Major, Minor );

					m_GLContext = glXCreateContextAttribs(
						m_pWindowData->GetDisplay( ), FBConfig, 0, True,
						ContextAttribs );

					if( m_GLContext == ZED_NULL )
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] "
							"<ERROR> Failed to create context.\n" );
						break;
					}

					ZED_SINT32 MakeCur = glXMakeCurrent(
						m_pWindowData->GetDisplay( ),
						m_pWindowData->GetWindow( ), m_GLContext );

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

			XSync( m_pWindowData->GetDisplay( ), False );

			if( !m_GLContext )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to create OpenGL context.\n" );

				return ZED_FAIL;
			}

			if( !glXIsDirect( m_pWindowData->GetDisplay( ), m_GLContext ) )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
					"Indirect GLX Context.\n" );
			}
			else
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
					"Direct GLX Context.\n" );
			}

			zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <INFO> "
				"Making GLX Context current.\n" );

			glXMakeCurrent( m_pWindowData->GetDisplay( ),
				m_pWindowData->GetWindow( ), m_GLContext );

			zedSafeDelete( m_pGLExtender );
			m_pGLExtender = new LinuxGLExtender( *m_pWindowData );

			if( m_pGLExtender->Initialise( GLVersion ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Create] <ERROR> "
					"Failed to get extensions.\n" );
				zedAssert( ZED_FALSE );

				return ZED_FAIL;
			}

			this->ResizeCanvas( m_Canvas.Width( ), m_Canvas.Height( ) );
			m_pVertexCacheManager = new GLVertexCacheManager( );
			m_pMaterialManager = new ZED::Renderer::MaterialManager( );

			zglGenBuffers( 1, &m_BackbufferID );
			zglBindBuffer( GL_PIXEL_PACK_BUFFER_ARB, m_BackbufferID );
			ZED_MEMSIZE BPPSize = 0;
			if( m_Canvas.ColourFormat( ) == ZED_FORMAT_ARGB8 )
			{
				BPPSize = 4;
			}
			else if( m_Canvas.ColourFormat( ) == ZED_FORMAT_RGB565 )
			{
				BPPSize = 2;
			}

			zglBufferData( GL_PIXEL_PACK_BUFFER_ARB,
				p_Window.GetWidth( ) * p_Window.GetHeight( ) * BPPSize, 0,
				GL_STREAM_READ_ARB );

			zglBindBuffer( GL_PIXEL_PACK_BUFFER_ARB, 0 );

			return ZED_OK;
		}

		void LinuxRendererOGL3::Clear( )
		{
			glClear( m_ClearFlags );
		}

		void LinuxRendererOGL3::SwapBuffers( )
		{
			m_pVertexCacheManager->ForceFlushAll( );
			glXSwapBuffers( m_pWindowData->GetDisplay( ),
				m_pWindowData->GetWindow( ) );

			if( m_TakeScreenshot )
			{
				m_TakeScreenshot = ZED_FALSE;
				ZED::System::NativeFile File;
				if( File.Open( m_pScreenshotFileName,
					ZED::System::FILE_ACCESS_WRITE |
					ZED::System::FILE_ACCESS_BINARY ) != ZED_OK )
				{
					return ;
				}
				
				zglReadBuffer( GL_BACK );

				zglBindBuffer( GL_PIXEL_PACK_BUFFER_ARB, m_BackbufferID );
				zglReadPixels( 0, 0, m_Canvas.Width( ), m_Canvas.Height( ),
					GL_BGR, GL_UNSIGNED_BYTE, 0 );

				GLubyte *pBackbuffer = ( GLubyte * )zglMapBuffer(
					GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB );

				if( pBackbuffer )
				{
					ZED::Renderer::TARGA_HEADER TargaHeader;
					memset( &TargaHeader, 0, sizeof( TargaHeader ) );

					TargaHeader.IDLength = 0;
					TargaHeader.ColourmapType = 0;
					TargaHeader.ImageType = 2;
					TargaHeader.X = 0;
					TargaHeader.Y = 0;
					TargaHeader.Width = m_Canvas.Width( );
					TargaHeader.Height = m_Canvas.Height( );
					TargaHeader.BitsPerPixel = 24;
					TargaHeader.ImageDescription = 0;

					if( File.WriteByte(
						 reinterpret_cast< ZED_BYTE * >( &TargaHeader ),
						sizeof( TargaHeader ), ZED_NULL ) != ZED_OK )
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::"
							"EndScene] <ERROR> Failed to write Targa header "
							"to file: %s\n", m_pScreenshotFileName );

						File.Close( );
						zglBindBuffer( GL_PIXEL_PACK_BUFFER_ARB, 0 );

						return;
					}

					if( File.WriteByte(
						reinterpret_cast< ZED_BYTE * >( pBackbuffer ),
						m_Canvas.Width( ) * m_Canvas.Height( ) * 4,
						ZED_NULL ) != ZED_OK )
					{
						zedTrace( "[ZED::Renderer::LinuxRendererOGL3::"
							"EndScene] <ERROR> Something went wrong writing "
							"the Targa image data to file: %s\n",
							m_pScreenshotFileName );

						File.Close( );
						zglBindBuffer( GL_PIXEL_PACK_BUFFER_ARB, 0 );

						return;
					}
				}

				File.Close( );
				zglBindBuffer( GL_PIXEL_PACK_BUFFER_ARB, 0 );
			}
		}

		void LinuxRendererOGL3::SetClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
			glClearColor( p_Red, p_Green, p_Blue, 1.0f );
		}

		void LinuxRendererOGL3::SetClearFlags( const ZED_BOOL p_Colour,
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

			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::CreateMesh(
			const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
			const ZED_UINT32 p_MaterialID,
			const ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			m_pVertexCacheManager->Render( p_VertexCount, p_pVertices,
				p_pIndexCount, p_pIndices, p_Attributes, p_MaterialID,
				p_PrimitiveType );

			// TODO
			// The following line is incredibly stupid, the vertex cache being
			// used should flush itself if the object to be rendered is unable
			// to fit in any available caches
			// Furthermore, this member function should probably take a matrix
			// to set the world view projection for the rendered object, as
			// that is the reason for ForceFlushAll being called
			m_pVertexCacheManager->ForceFlushAll( );

			return ZED_OK;
		}

		ZED_UINT32 LinuxRendererOGL3::Screenshot( const ZED_CHAR8 *p_pFileName,
			const ZED_BOOL p_RelativeToExecutable )
		{
			if( !p_pFileName )
			{
				zedTrace( "[ZED::Renderer::LinuxRendererOGL3::Screenshot] "
					"<ERROR> No file specified\n" );

				return ZED_FAIL;
			}

			ZED_CHAR8 *pExecutablePath = ZED_NULL;

			if( p_RelativeToExecutable )
			{
				pExecutablePath = new ZED_CHAR8[ ZED_MAX_PATH ];
				memset( pExecutablePath, '\0', ZED_MAX_PATH );

				ZED::System::GetExecutablePath( &pExecutablePath,
					ZED_MAX_PATH );
			}

			ZED_MEMSIZE FileNameLength = strlen( p_pFileName );

			if( pExecutablePath )
			{
				FileNameLength += strlen( pExecutablePath );

				m_pScreenshotFileName = new ZED_CHAR8[ FileNameLength + 1 ];

				strncpy( m_pScreenshotFileName, pExecutablePath,
					strlen( pExecutablePath ) );

				zedSafeDelete( pExecutablePath );
			}
			else
			{
				m_pScreenshotFileName = new ZED_CHAR8[ FileNameLength + 1 ];
			}

			memset( m_pScreenshotFileName, '\0', FileNameLength + 1 );

			strncat( m_pScreenshotFileName, p_pFileName, FileNameLength );

			m_TakeScreenshot = ZED_TRUE;

			return ZED_OK;
		}

		void LinuxRendererOGL3::SetRenderState( const ZED_RENDERSTATE p_State,
			const ZED_UINT32 p_Value )
		{
			// Determine the type and set the render state
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
					zedTrace( "[ZED::Renderer::LinuxRendererOGL3::RenderState]"
						" <INFO> Setting cullmode: " );
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

		ZED_UINT32 LinuxRendererOGL3::AddMaterial(
			ZED::Renderer::Material * const &p_pMaterial )
		{
			return m_pMaterialManager->AddMaterial( p_pMaterial );
		}

		ZED_UINT32 LinuxRendererOGL3::GetMaterial(
			const ZED_UINT32 p_MaterialID,
			ZED::Renderer::Material *p_pMaterial ) const
		{
			return m_pMaterialManager->GetMaterial( p_MaterialID,
				p_pMaterial );
		}

		ZED_UINT32 LinuxRendererOGL3::GetMaterial(
			ZED_CHAR8 * const &p_pMaterialName,
			ZED::Renderer::Material *p_pMaterial ) const
		{
			return m_pMaterialManager->GetMaterial( p_pMaterialName,
				p_pMaterial );
		}
	}
}

