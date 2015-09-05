#include <Renderer/RendererFactory.hpp>
/*#include <Renderer/LinuxRendererOGL2.hpp>
#include <Renderer/LinuxRendererOGL3.hpp>
#include <Renderer/LinuxRendererOGL4.hpp>*/
#include <Renderer/LinuxRendererOpenGL.hpp>
#include <System/Debugger.hpp>
#include <string>
#include <cstring>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

namespace ZED
{
    namespace Renderer
    {
		ZED_UINT32 CreateRenderer( const ZED_CHAR8 *p_pRendererString,
			Renderer **p_ppRenderer )
		{
			if( p_ppRenderer == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::CreateRenderer] <ERROR> "
					"Renderer to output is null\n" );

				return ZED_FAIL;
			}

			Renderer *pRenderer = ZED_NULL;

			// Extract the first part of the string (if there is no version
			// number, query for the highest supported version)

			if( strncmp( "OpenGL", p_pRendererString, strlen( "OpenGL" ) ) ==
				0 )
			{
				if( strlen( p_pRendererString ) > strlen( "OpenGL" ) )
				{
					std::string GLString = p_pRendererString;
					ZED_MEMSIZE GLVersionStringStart =
						GLString.find_first_of( ' ' );

					ZED_MEMSIZE GLMajorDelimiter, GLMinorDelimiter;

					ZED_SINT32 GLMajor = -1, GLMinor = -1;

					++GLVersionStringStart;
					
					GLMajorDelimiter = GLString.find_first_of( '.',
						GLVersionStringStart );

					std::string GLMajorString, GLMinorString;

					if( GLMajorDelimiter == std::string::npos )
					{
						if( GLVersionStringStart == GLString.size( ) )
						{
							zedTrace( "[ZED::Renderer::CreateRenderer] "
								"<ERROR> Incorrectly formatted renderer "
								"string, invalid character discovered in "
								"\"%s\"\n", p_pRendererString );

							return ZED_FAIL;
						}
					}

					GLMajorString = GLString.substr( GLVersionStringStart,
						( GLMajorDelimiter - GLVersionStringStart ) );

					if( GLMajorString.find_first_not_of( "0123456789" ) !=
						std::string::npos )
					{
						zedTrace( "[ZED::Renderer::CreateRenderer] <ERROR> "
							"Major string contains invalid characters: %s\n",
							GLMajorString.c_str( ) );

						return ZED_FAIL;
					}

					if( GLMajorString.size( ) == 0 )
					{
						zedTrace( "[ZED::Renderer::CreateRenderer] <ERROR> "
							"Major version not found in string: %s\n",
							p_pRendererString );

						return ZED_FAIL;
					}

					if( GLMajorDelimiter != std::string::npos )
					{
						++GLMajorDelimiter;
						GLMinorDelimiter = GLString.find_first_of(
							" \t", GLMajorDelimiter );

						if( GLMinorDelimiter == GLMajorDelimiter )
						{
							zedTrace( "[ZED::Renderer::CreateRenderer] "
								"<ERROR> Invalid character used for the minor "
								"version: %s\n", p_pRendererString );

							return ZED_FAIL;
						}

						if( GLMinorDelimiter != std::string::npos )
						{

							GLMinorString = GLString.substr( GLMajorDelimiter,
								( GLMinorDelimiter - ( GLMajorDelimiter ) ) );

							GLMinor = atoi( GLMinorString.c_str( ) );
						}
						else
						{
							// We may be at the end of the string, see if the
							// end of the string contains a number
							if( GLMajorDelimiter < GLString.size( ) )
							{
								GLMinorString = GLString.substr(
									GLMajorDelimiter,
									( GLString.size( ) - GLMajorDelimiter ) );

								if( GLMinorString.size( ) == 0 )
								{
									zedTrace( "[ZED::Renderer::"
										"CreateRenderer] <ERROR> No minor "
										"number provided\n" );

									return ZED_FAIL;
								}
							}
							else
							{
								zedTrace( "[ZED::Renderer::CreateRenderer] "
									"<ERROR> Unrecognised character after "
									"major version\n" );

								return ZED_FAIL;
							}
						}

						if( GLMinorString.size( ) == 0 )
						{
							zedTrace( "[ZED::Renderer::CreateRenderer] <ERROR> "
								"Minor version not found in string: %s\n",
								p_pRendererString );

							return ZED_FAIL;
						}

						if( GLMinorString.find_first_not_of( "0123456789" ) !=
							std::string::npos )
						{
							zedTrace( "[ZED::Renderer::CreateRenderer] "
								"<ERROR> Minor string contains invalid "
								"characters: %s\n", GLMinorString.c_str( ) );

							return ZED_FAIL;
						}


						GLMinor = atoi( GLMinorString.c_str( ) );
					}

					GLMajor = atoi( GLMajorString.c_str( ) );

					zedTrace( "GLVersion: %d.%d | String: %s.%s\n",
						GLMajor, GLMinor,
						GLMajorString.c_str( ), GLMinorString.c_str( ) );

					/*zedTrace( "Major start: %d\n", GLVersionStringStart );
					zedTrace( "Major end: %d\n", GLMajorDelimiter );
					zedTrace( "Minor start: %d\n", GLMajorDelimiter );
					zedTrace( "Minor end: %d\n", GLMinorDelimiter );
					zedTrace( "String size: %d\n", GLString.size( ) );
					zedTrace( "GL string: %s\n", GLString.c_str( ) );*/

					pRenderer = new LinuxRendererOpenGL( GLMajor, GLMinor );
					
					/*
					switch( GLMajor )
					{
						case 2:
						{
							pRenderer = new LinuxRendererOGL2( );//GLMinor );
							break;
						}
						case 3:
						{
							pRenderer = new LinuxRendererOGL3( GLMinor );
							break;
						}
						default:
						{
							zedTrace( "[ZED::Renderer::CreateRenderer] "
								"<ERROR> Unknown version string\n" );

							return ZED_FAIL;
						}
					}*/
				}
				else
				{
					ZED_GLVERSION GLVersion;

					if( GetHighestSupportedOpenGLVersion( GLVersion ) !=
						ZED_OK )
					{
						return ZED_FAIL;
					}

					pRenderer = new LinuxRendererOpenGL( GLVersion.Major,
						GLVersion.Minor );

					/*switch( GLVersion.Major )
					{
						case 2:
						{
							pRenderer = new LinuxRendererOGL2( 2, -1 );

							break;
						}
						case 3:
						{
							pRenderer = new LinuxRendererOpenGL( 3, -1 );

							break;
						}
						default:
						{
							zedTrace( "[ZED::Renderer::CreateRenderer] "
								"<ERROR> Unknown OpenGL version %d.%d "
								"acquired from the operating system\n",
								GLVersion.Major, GLVersion.Minor );

							return ZED_FAIL;
						}
					}*/
				}
			}

			if( pRenderer )
			{
				( *p_ppRenderer ) = pRenderer;

				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 GetHighestSupportedOpenGLVersion(
			ZED_GLVERSION &p_GLVersion )
		{
			// Error values are -1 for the version number
			p_GLVersion.Major = -1;
			p_GLVersion.Minor = -1;

			// Create a temporary window and GLX context to get the version
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetHighestSupportedOpenGLVersion] "
					"<ERROR> Failed to open a display\n" );

				return ZED_FAIL;
			}

			int VisualAttributes[ ] =
			{
				GLX_X_RENDERABLE,	True,
				GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
				GLX_RENDER_TYPE,	GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
				None
			};

			int FramebufferCount = 0;

			GLXFBConfig *pGLFramebufferConfig = glXChooseFBConfig(
				pDisplay, DefaultScreen( pDisplay ), VisualAttributes,
				&FramebufferCount );

			if( FramebufferCount == 0 )
			{
				zedTrace( "[ZED::Renderer::GetHighestSupportedOpenGLVersion] "
					"<ERROR> Could not acquire any frame buffers\n" );

				return ZED_FAIL;
			}

			GLXFBConfig GLFramebufferConfig = pGLFramebufferConfig[ 0 ];
			XFree( pGLFramebufferConfig );
			pGLFramebufferConfig = ZED_NULL;

			XVisualInfo *pXVisualInfo = glXGetVisualFromFBConfig(
				pDisplay, GLFramebufferConfig );

			XSetWindowAttributes WindowAttributes;
			WindowAttributes.colormap = XCreateColormap( pDisplay,
				RootWindow( pDisplay, pXVisualInfo->screen ),
				pXVisualInfo->visual, AllocNone );

			Window GLWindow = XCreateWindow( pDisplay,
				RootWindow( pDisplay, pXVisualInfo->screen ),
				0, 0, 1, 1, 0, pXVisualInfo->depth, InputOutput,
				pXVisualInfo->visual, CWColormap, &WindowAttributes );

			if( GLWindow == 0 )
			{
				zedTrace( "[ZED::Renderer::GetHighestSupportedOpenGLVersion] "
					"<ERROR> Failed to create a window\n" );

				XCloseDisplay( pDisplay );
				pDisplay = ZED_NULL;

				XFree( pXVisualInfo );
				pXVisualInfo = ZED_NULL;

				return ZED_FAIL;
			}

			GLXContext TempContext = glXCreateContext( pDisplay, pXVisualInfo,
				0, True );

			XFree( pXVisualInfo );
			pXVisualInfo = ZED_NULL;

			glXMakeCurrent( pDisplay, GLWindow, TempContext );

			std::string GLVersionString( reinterpret_cast< const char * >(
				glGetString( GL_VERSION ) ) );

			// Done with the GLX context and window
			glXMakeCurrent( pDisplay, 0, 0 );
			glXDestroyContext( pDisplay, TempContext );

			XDestroyWindow( pDisplay, GLWindow );
			XCloseDisplay( pDisplay );
			pDisplay = ZED_NULL;

			ZED_MEMSIZE GLDelimiter = GLVersionString.find_first_of( '.' );

			if( GLDelimiter == std::string::npos )
			{
				zedTrace( "[ZED::Renderer::GetHighestSupportedOpenGLVersion] "
					"<ERROR> Could not parse OpenGL version string to get the "
					"initial delimiter position: %s\n",
					GLVersionString.c_str( ) );

				return ZED_FAIL;
			}

			std::string GLMajor = GLVersionString.substr( 0, GLDelimiter );

			p_GLVersion.Major = atoi( GLMajor.c_str( ) );
			ZED_MEMSIZE GLMinorDelimiter = GLVersionString.find_first_of(
				". \t", GLDelimiter );

			if( GLMinorDelimiter == std::string::npos )
			{
				zedTrace( "[ZED::Renderer::GetHighestSupportedOpenGLVersion] "
					"<ERROR> Could not parse OpenGL version string to get the "
					"second delimiter position: %s\n",
					GLVersionString.c_str( ) );

				return ZED_FAIL;
			}

			// Advance to allow the first delimiter to be passed over when
			// getting the string
			++GLDelimiter;

			std::string GLMinor = GLVersionString.substr( GLDelimiter,
				GLMinorDelimiter );

			p_GLVersion.Minor = atoi( GLMinor.c_str( ) );

			zedTrace( "[ZED::Renderer::GetHighestSupportedOpenGLVersion] "
				"<INFO> OpenGL version: %d.%d\n", p_GLVersion.Major,
				p_GLVersion.Minor );

			return ZED_OK;
		}
    }
}

