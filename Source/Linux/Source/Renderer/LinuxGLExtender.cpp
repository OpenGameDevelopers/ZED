#include <Renderer/OGL/LinuxGLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		LinuxGLExtender::LinuxGLExtender( )
		{
		}

		LinuxGLExtender::LinuxGLExtender(
			const ZED::System::X11WindowData &p_WindowData ) :
			m_WindowData( p_WindowData )
		{
		}

		LinuxGLExtender::~LinuxGLExtender( )
		{
		}

		ZED_UINT32 LinuxGLExtender::InitialiseWindowExtensions( )
		{
			// Process the string
			const char *pWinExt = 
				glXQueryExtensionsString( m_WindowData.GetDisplay( ),
					m_WindowData.GetScreenNumber( ) );

			if( pWinExt == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxGLExtender::"
					"InitialiseWindowExtensions] <ERROR> "
					"Failed to obtain the window system extensions string\n" );

				return ZED_FAIL;
			}

			ZED_UINT32 NumExtensions = 0;

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> Getting GLX Extensions.\n" );

			char CurExt[ 64 ] = { '\0' };
			ZED_MEMSIZE Position = 0, CharCount = 0;
			ZED_BOOL Loop = ZED_TRUE;

			do
			{
				CurExt[ Position++ ] = pWinExt[ CharCount ];

				if( pWinExt[ CharCount+1 ] == 0x20 )
				{
					std::string Copy;
					Copy.insert( 0, CurExt, Position );

					m_WindowExtensions.push_back( Copy );

					zedTrace( "%s\n", Copy.c_str( ) );

					Position = 0;
					NumExtensions++;
					CharCount++;
				}

				CharCount++;

				if( pWinExt[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
			}while( Loop );

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> %d GLX Extensions available.\n", NumExtensions );

			pWinExt = 
				glXGetClientString( m_WindowData.GetDisplay( ),
					GLX_EXTENSIONS );

			NumExtensions = 0;

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> Getting Client GLX Extensions.\n" );

			Position = 0;
			CharCount = 0;
			Loop = ZED_TRUE;

			do
			{
				CurExt[ Position++ ] = pWinExt[ CharCount ];

				if( pWinExt[ CharCount+1 ] == 0x20 )
				{
					std::string Copy;
					Copy.insert( 0, CurExt, Position );

					m_WindowExtensions.push_back( Copy );

					zedTrace( "%s\n", Copy.c_str( ) );

					Position = 0;
					NumExtensions++;
					CharCount++;
				}

				CharCount++;

				if( pWinExt[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
			}while( Loop );

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> %d Client GLX Extensions available.\n",
				NumExtensions );

			pWinExt = glXQueryServerString( m_WindowData.GetDisplay( ),
					m_WindowData.GetScreenNumber( ), GLX_EXTENSIONS );

			NumExtensions = 0;

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> Getting Server GLX Extensions.\n" );

			Position = 0;
			CharCount = 0;
			Loop = ZED_TRUE;

			do
			{
				CurExt[ Position++ ] = pWinExt[ CharCount ];

				if( pWinExt[ CharCount+1 ] == 0x20 )
				{
					std::string Copy;
					Copy.insert( 0, CurExt, Position );

					m_WindowExtensions.push_back( Copy );

					zedTrace( "%s\n", Copy.c_str( ) );

					Position = 0;
					NumExtensions++;
					CharCount++;
				}

				CharCount++;

				if( pWinExt[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
			}while( Loop );

			zedTrace( "[ZED::Renderer::GLExtender::InitialiseWindowExt] "
				"<INFO> %d Server GLX Extensions available.\n",
				NumExtensions );

			return ZED_OK;
		}
	}
}

