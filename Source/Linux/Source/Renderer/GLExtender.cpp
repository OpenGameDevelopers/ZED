#include <Renderer/OGL/GLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{

		ZED_UINT32 GLExtender::InitialiseWindowExtensions( )
		{
			// Process the string
			const char *pWinExt = 
				glXQueryExtensionsString( m_WindowData.pX11Display,
					DefaultScreen( m_WindowData.pX11Display ) );

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
				glXGetClientString( m_WindowData.pX11Display, GLX_EXTENSIONS );

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

			pWinExt = glXQueryServerString( m_WindowData.pX11Display,
					DefaultScreen( m_WindowData.pX11Display ),
					GLX_EXTENSIONS );

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

		ZED_UINT32 GLExtender::RegisterBaseGLExtensions( )
		{
			// Just register the CreateContextAttribsARB
			return ZED_OK;
		}
	}
}

