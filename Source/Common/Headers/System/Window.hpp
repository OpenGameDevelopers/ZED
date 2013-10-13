#ifndef __ZED_SYSTEM_WINDOW_HPP__
#define __ZED_SYSTEM_WINDOW_HPP__

#include <System/DataTypes.hpp>
#if defined ZED_WINDOWSYSTEM_X11
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#if defined ZED_PLATFORM_LINUX
		#include <GL/glx.h>
	#elif defined ZED_PLATFORM_PANDORA_LINUX
		#include <EGL/egl.h>
	#endif // ZED_PLATFORM_LINUX
#elif defined ZED_WINDOWSYSTEM_WIN32
#include <Windows.h>
#endif // ZED_WINODWSYSTEM_X11

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
		typedef enum __SCREEN_ORIENTATION
		{
			SCREEN_ORIENTATION_0,
			SCREEN_ORIENTATION_90,
			SCREEN_ORIENTATION_180,
			SCREEN_ORIENTATION_270
		}SCREEN_ORIENTATION;
		
		typedef struct __SCREEN
		{
			ZED_UINT32 Width;
			ZED_UINT32 Height;
			ZED_UINT32 BitsPerPixel;
			ZED_UINT32 RefreshRate;
		}SCREEN;

		ZED_UINT32 GetNativeScreenSize( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber,
			SCREEN &p_ScreenSize );

		ZED_UINT32 GetDisplayCount( ZED_UINT32 *p_pDisplayCount );
		ZED_UINT32 GetScreenCount( const ZED_UINT32 p_DisplayNumber,
			ZED_UINT32 *p_pScreenCount );
		ZED_UINT32 GetScreenOrientation( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber,
			SCREEN_ORIENTATION *p_pOrientation );

		ZED_UINT32 EnumerateScreens( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber, SCREEN **p_ppScreens,
			ZED_MEMSIZE *p_pCount );

		ZED_UINT32 GetCurrentScreenNumber( );
		SCREEN_ORIENTATION GetCurrentScreenOrientation( );

#if defined ZED_WINDOWSYSTEM_X11
		typedef struct __WINDOWDATA
		{
			XVisualInfo	*pX11VisualInfo;
			Display		*pX11Display;
#if defined ZED_PLATFORM_LINUX
			GLXFBConfig	X11GLXFBConfig;
			::Window	X11Window;
#endif // ZED_PLATFORM_LINUX
		}WINDOWDATA;

#elif defined ZED_WINDOWSYSTEM_WIN32
		typedef struct __WINDOWDATA
		{
			HDC		DeviceContext;
			HWND	WindowHandle;
		}WINDOWDATA;
#else
#error Unknown windowing system
#endif // ZED_WINDOWSYSTEM_X11

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

			virtual WINDOWDATA WindowData( ) const = 0;

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

#endif // __ZED_SYSTEM_WINDOW_HPP__

