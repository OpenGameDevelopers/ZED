#ifndef __ZED_SYSTEM_WINDOWSWINDOW_HPP__
#define __ZED_SYSTEM_WINDOWSWINDOW_HPP__

#include <windows.h>
#include <System/Window.hpp>

namespace ZED
{
	namespace System
	{
		class WindowsWindow
		{
		public:
			WindowsWindow( );
			virtual ~WindowsWindow( );

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height,
				const ZED_UINT32 p_DisplayNumber,
				const ZED_UINT32 p_ScreenNumber,
				const ZED_UINT32 p_Style );

			virtual void Destroy( );

			virtual ZED_UINT32 Update( );

			virtual ZED_WINDOWDATA WindowData( ) const { return m_WindowData; }

			virtual void Title( const char *p_pTitle );

			virtual void HideCursor( );
			virtual void ShowCursor( );
			virtual ZED_BOOL ToggleCursor( );

			virtual void SetWindowed( );
			virtual void SetFullScreen( );
			virtual ZED_BOOL ToggleFullScreen( );

			virtual ZED_BOOL Closed( );

		private:
			ZED_WINDOWDATA m_WindowData;
		};
	}
}

#endif // __ZED_SYSTEM_WINDOWSWINDOW_HPP__

