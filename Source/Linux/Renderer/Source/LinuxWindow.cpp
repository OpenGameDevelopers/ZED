#include <LinuxWindow.hpp>
#include <X11/extensions/Xrandr.h>

#include <Debugger.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 GetNativeScreenSize( ZED_UINT32 p_ScreenNumber )
		{
			return ZED_OK;
		}

		ZED_UINT32 GetScreenCount( ZED_UINT32 *p_pScreenCount )
		{
			Display *pDisplay = XOpenDisplay( ZED_NULL );
			if( pDisplay == ZED_NULL )
			{
				( *p_pScreenCount ) = 0;
				
				zedTrace( "[ZED::Renderer::GetScreenCount] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			ZED_UINT32 ScreenCount = ScreenCount( pDisplay );
			XCloseDisplay( pDisplay );

			( *p_pScreenCount ) = ScreenCount;

			return ZED_OK;
		}

		ZED_UINT32 EnumerateScreenSizes( ZED_SCREENSIZE **p_ppSizes,
			ZED_MEMSIZE *p_pCount, const ZED_UINT32 p_ScreenNumber )
		{
			Display *pDisplay = XOpenDisplay( ZED_NULL );
			ZED_SINT32 TotalSizes = 0;
			XRRScreenSize *pScreenSize = XRRSizes( pDisplay, p_ScreenNumber,
				&TotalSizes );
			XCloseDisplay( pDisplay );

			zedTrace( "[ZED::Renderer::EnumerateScreenSizes] <INFO> "
				"Found %d resolutions\n", TotalSizes );

			( *p_ppSizes ) = new ZED_SCREENSIZE[ TotalSizes ];

			for( ZED_SINT32 i = 0; i < TotalSizes; ++i )
			{
				zedTrace( "\t%d: %dx%d\n",
					i, pScreenSize[ i ].width, pScreenSize[ i ].height );

				( *p_ppSizes )[ i ].Width = pScreenSize[ i ].width;
				( *p_ppSizes )[ i ].Height = pScreenSize[ i ].height;
			}

			( *p_pCount ) = TotalSizes;
			
			return ZED_OK;
		}

		ZED_UINT32 LinuxWindow::Create( const ZED_UINT32 p_X,
			const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			return ZED_OK;
		}

	}
}

