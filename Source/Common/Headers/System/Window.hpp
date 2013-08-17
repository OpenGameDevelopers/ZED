#ifndef __ZED_RENDERER_WINDOW_HPP__
#define __ZED_RENDERER_WINDOW_HPP__

#include <System/DataTypes.hpp>
#ifdef ZED_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#elif defined ZED_PLATFORM_WINDOWS
#include <Windows.h>
#else
#error Unknown platform
#endif

const ZED_UINT32 ZED_WINDOW_STYLE_ALL			= 0x00000000;
const ZED_UINT32 ZED_WINDOW_STYLE_MINIMISE		= 0x00000001;
const ZED_UINT32 ZED_WINDOW_STYLE_MAXIMISE		= 0x00000002;
const ZED_UINT32 ZED_WINDOW_STYLE_CLOSE			= 0x00000004;
const ZED_UINT32 ZED_WINDOW_STYLE_RESIZE		= 0x00000008;
const ZED_UINT32 ZED_WINDOW_STYLE_TITLEBAR		= 0x00000010;
const ZED_UINT32 ZED_WINDOW_STYLE_MENU			= 0x00000020;
const ZED_UINT32 ZED_WINDOW_STYLE_BORDER		= 0x00000040;
const ZED_UINT32 ZED_WINDOW_STYLE_MOVE			= 0x00000080;
const ZED_UINT32 ZED_WINDOW_STYLE_FULLSCREEN	= 0x00000100;
const ZED_UINT32 ZED_WINDOW_STYLE_NONE			= 0x80000000;

namespace ZED
{
	namespace System
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

		ZED_UINT32 GetNativeScreenSize( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber,
			ZED_SCREENSIZE &p_ScreenSize );
		ZED_UINT32 GetScreenCount( const ZED_UINT32 p_DisplayNumber,
			ZED_UINT32 *p_pScreenCount );
		ZED_SCREEN_ORIENTATION GetScreenOrientation(
			const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber );

		ZED_UINT32 EnumerateScreenSizes( ZED_SCREENSIZE **p_ppSizes,
			ZED_MEMSIZE *p_pCount, const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber );

		ZED_UINT32 GetCurrentScreenNumber( );
		ZED_SCREEN_ORIENTATION GetCurrentScreenOrientation( );

		ZED_UINT32 GetDisplayCount( );

#if defined ZED_PLATFORM_LINUX || ZED_PLATFORM_PANDORA_LINUX
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
				const ZED_UINT32 p_Height,
				const ZED_UINT32 p_DisplayNumber = 0,
				const ZED_UINT32 p_ScreenNumber = 0,
				const ZED_UINT32 p_Style  = ZED_WINDOW_STYLE_ALL ) = 0;
			
			virtual void Destroy( ) = 0;

			virtual ZED_UINT32 Update( ) = 0;

			virtual ZED_WINDOWDATA WindowData( ) const = 0;

			virtual void Title( const char *p_pTitle ) = 0;

			virtual void HideCursor( ) = 0;
			virtual void ShowCursor( ) = 0;
			virtual ZED_BOOL ToggleCursor( ) = 0;

			virtual void SetWindowed( ) = 0;
			virtual void SetFullScreen( ) = 0;
			virtual ZED_BOOL ToggleFullScreen( ) = 0;

			virtual ZED_BOOL Closed( ) = 0;
		};
	}
}

#endif

