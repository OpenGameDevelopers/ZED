#ifndef __ZED_RENDERER_WINDOW_HPP__
#define __ZED_RENDERER_WINDOW_HPP__

#include <DataTypes.hpp>
#ifdef ZED_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#elif defined ZED_PLATFORM_WINDOWS
#include <Windows.h>
#else
#error Unkown platform
#endif

namespace ZED
{
	namespace Renderer
	{
		typedef enum __ZED_SCREEN_ORIENTATION
		{
			ZED_SCREEN_ORIENTATION_0,
			ZED_SCREEN_ORIENTATION_90,
			ZED_SCREEN_ORIENTATION_180,
			ZED_SCREEN_ORIENTATION_270
		}ZED_SCREEN_ORIENTATION;
		typedef struct __ZED_SCREENSIZE
		{
			ZED_UINT32 Width;
			ZED_UINT32 Height;
		}ZED_SCREENSIZE;

		ZED_UINT32 GetNativeScreenSize( const ZED_UINT32 p_ScreenNumber,
			ZED_SCREENSIZE &p_ScreenSize );
		ZED_UINT32 GetScreenCount( ZED_UINT32 *p_pScreenCount );
		ZED_SCREEN_ORIENTATION GetScreenOrientation(
			const ZED_UINT32 p_ScreenNumber );

		ZED_UINT32 EnumerateScreenSizes( ZED_SCREENSIZE **p_ppSizes,
			ZED_MEMSIZE *p_pCount, const ZED_UINT32 p_ScreenNumber );

		ZED_UINT32 GetCurrentScreenNumber( );
		ZED_SCREEN_ORIENTATION GetCurrentScreenOrientation( );

#ifdef ZED_PLATFORM_LINUX
		typedef struct __ZED_WINDOWDATA
		{
			XVisualInfo	*pX11VisualInfo;
			Display		*pX11Display;
			GLXFBConfig	X11GLXFBConfig;
			::Window	X11Window;
		}ZED_WINDOWDATA;
#elif defined ZED_PLATFORM_WINDOWS
		typedef struct __ZED_WINDOWDATA
		{
			HDC		DeviceContext;
			HWND	WindowHandle;
		}ZED_WINDOWDATA;
#else
#error Unknown platform
#endif

		class Window
		{
		public:
			virtual ~Window( ){ }

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height ) = 0;
			
			virtual void Destroy( ) = 0;

			virtual ZED_UINT32 Update( ) = 0;

			virtual ZED_WINDOWDATA WindowData( ) const = 0;

			virtual void HideCursor( ) = 0;
			virtual void ShowCursor( ) = 0;
			virtual ZED_BOOL ToggleCursor( ) = 0;

			virtual void SetWindowed( ) = 0;
			virtual void SetFullScreen( ) = 0;
			virtual ZED_BOOL ToggleFullScreen( ) = 0;
		};
	}
}

#endif

