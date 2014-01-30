#ifndef __ZED_SYSTEM_LINUXWINDOW_HPP__
#define __ZED_SYSTEM_LINUXWINDOW_HPP__

#include <System/Window.hpp>
#include <X11/Xlib.h>

namespace ZED
{
	namespace System
	{
		class LinuxWindow : public ZED::System::Window
		{
		public:
			LinuxWindow( );
			virtual ~LinuxWindow( );

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height, const ZED_UINT32 p_DisplayNumber,
				const ZED_UINT32 p_ScreenNumber, const ZED_UINT32 p_Style );

			virtual void Destroy( );

			virtual ZED_UINT32 Update( );

			virtual void FlushEvents( );

			virtual WINDOWDATA WindowData( ) const { return m_WindowData; }

			virtual void Title( const char *p_pTitle );

			virtual void HideCursor( );
			virtual void ShowCursor( );
			virtual ZED_BOOL ToggleCursor( );

			virtual void SetWindowed( );
			virtual void SetFullScreen( );
			virtual ZED_BOOL ToggleFullScreen( );

			virtual ZED_BOOL Closed( );

			virtual void WarpPointer( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y );

			virtual ZED_UINT32 GrabKeyboard( );
			virtual ZED_UINT32 GrabMouse( );

			virtual void ReleaseKeyboard( );
			virtual void ReleaseMouse( );

		private:
			::Window		m_Window;
			Display			*m_pDisplay;
			Screen			m_Screen;
			XVisualInfo		*m_pVisualInfo;
			WINDOWDATA		m_WindowData;
			ZED_BOOL		m_CursorHidden;
			ZED_BOOL		m_FullScreen;
			ZED_UINT32		m_X;
			ZED_UINT32		m_Y;
			ZED_UINT32		m_Width;
			ZED_UINT32		m_Height;
			ZED_BOOL		m_Running;

			Cursor NullCursor( );
		};
	}
}

#endif

