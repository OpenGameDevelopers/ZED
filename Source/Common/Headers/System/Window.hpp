#ifndef __ZED_SYSTEM_WINDOW_HPP__
#define __ZED_SYSTEM_WINDOW_HPP__

#include <System/DataTypes.hpp>

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

typedef enum __ZED_WINDOW_FLUSH_TYPE
{
	ZED_WINDOW_FLUSH_ALL			= 1 << 0,
	ZED_WINDOW_FLUSH_NONE			= 1 << 1,
	ZED_WINDOW_FLUSH_MOUSE			= 1 << 2,
	ZED_WINDOW_FLUSH_KEYBOARD		= 1 << 3,
	ZED_WINDOW_FLUSH_WINDOWCROSS	= 1 << 4,
	ZED_WINDOW_FLUSH_FOCUS			= 1 << 5,
	ZED_WINDOW_FLUSH_EXPOSE			= 1 << 6,
	ZED_WINDOW_FLUSH_KEYMAP			= 1 << 7,
	ZED_WINDOW_FLUSH_WINDOWSTATE	= 1 << 8,
	ZED_WINDOW_FLUSH_COLOURMAP		= 1 << 9,
	ZED_WINDOW_FLUSH_CLIENT			= 1 << 10,
	ZED_WINDOW_FLUSH_STRUCTURE		= 1 << 11
}ZED_WINDOW_FLUSH_TYPE;

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

		ZED_SINT32 GetCurrentDisplayNumber( );
		ZED_SINT32 GetCurrentScreenNumber( );
		SCREEN_ORIENTATION GetCurrentScreenOrientation( );

		class WindowData;

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

			virtual void FlushEvents(
				const ZED_WINDOW_FLUSH_TYPE p_FlushType ) = 0;

			virtual ZED_UINT32 GetWindowData(
				WindowData **p_ppWindowData ) const = 0;

			virtual void Title( const char *p_pTitle ) = 0;

			virtual void HideCursor( ) = 0;
			virtual void ShowCursor( ) = 0;
			virtual ZED_BOOL ToggleCursor( ) = 0;

			virtual void SetWindowed( ) = 0;
			virtual void SetFullScreen( ) = 0;
			virtual ZED_BOOL ToggleFullScreen( ) = 0;

			virtual ZED_BOOL Closed( ) = 0;

			virtual void WarpPointer( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y ) = 0;

			virtual ZED_UINT32 GrabKeyboard( ) = 0;
			virtual ZED_UINT32 GrabMouse( const ZED_BOOL p_ConfineMouse,
				const ZED_BOOL p_CentreMouse ) = 0;

			virtual void ReleaseKeyboard( ) = 0;
			virtual void ReleaseMouse( ) = 0;

			virtual ZED_SINT32 GetXPosition( ) const = 0;
			virtual ZED_SINT32 GetYPosition( ) const = 0;

			virtual ZED_UINT32 GetWidth( ) const = 0;
			virtual ZED_UINT32 GetHeight( ) const = 0;

			virtual void SetXPosition( const ZED_SINT32 p_X ) = 0;
			virtual void SetYPosition( const ZED_SINT32 p_Y ) = 0;

			virtual void SetWidth( const ZED_UINT32 p_Width ) = 0;
			virtual void SetHeight( const ZED_UINT32 p_Height ) = 0;

			virtual ZED_BOOL Resized( ) = 0;
			virtual ZED_BOOL Moved( ) = 0;

		protected:
			ZED_UINT32	m_DisplayNumber;
			ZED_UINT32	m_ScreenNumber;
			ZED_UINT32	m_Style;
		};
	}
}

#endif // __ZED_SYSTEM_WINDOW_HPP__

