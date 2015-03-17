#ifndef __ZED_SYSTEM_LINUXWINDOW_HPP__
#define __ZED_SYSTEM_LINUXWINDOW_HPP__

#include <System/Window.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace ZED
{
	namespace System
	{
		class LinuxWindowData;
		class LinuxWindow : public ZED::System::Window
		{
		public:
			LinuxWindow( );
			LinuxWindow( const Window &p_RawWindow );
			virtual ~LinuxWindow( );

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height, const ZED_UINT32 p_DisplayNumber,
				const ZED_UINT32 p_ScreenNumber, const ZED_UINT32 p_Style );

			virtual void Destroy( );

			virtual ZED_UINT32 Update( );

			virtual void FlushEvents(
				const ZED_WINDOW_FLUSH_TYPE p_FlushType );

			virtual ZED_UINT32 GetWindowData(
				WindowData **p_ppWindowData ) const;

			virtual void SetTitle( const ZED_CHAR8 *p_pTitle );

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
			virtual ZED_UINT32 GrabMouse( const ZED_BOOL p_ConfineMouse,
				const ZED_BOOL p_CentreMouse );

			virtual void ReleaseKeyboard( );
			virtual void ReleaseMouse( );

			virtual ZED_SINT32 GetXPosition( ) const;
			virtual ZED_SINT32 GetYPosition( ) const;

			virtual ZED_UINT32 GetWidth( ) const;
			virtual ZED_UINT32 GetHeight( ) const;

			virtual void SetXPosition( const ZED_SINT32 p_X );
			virtual void SetYPosition( const ZED_SINT32 p_Y );

			virtual void SetWidth( const ZED_UINT32 p_Width );
			virtual void SetHeight( const ZED_UINT32 p_Height );

			virtual ZED_BOOL Resized( );
			virtual ZED_BOOL Moved( );

			ZED_UINT32 SetXVisualInfo( XVisualInfo *p_pXVisualInfo,
				ZED_BOOL p_Recreate = ZED_TRUE );

		private:
			LinuxWindowData	*m_pWindowData;
			XVisualInfo		*m_pXVisualInfo;
			Screen			*m_pScreen;
			::Window		m_Window;
			Display			*m_pDisplay;
			ZED_BOOL		m_CursorHidden;
			ZED_BOOL		m_FullScreen;
			ZED_SINT32		m_X;
			ZED_SINT32		m_Y;
			ZED_UINT32		m_Width;
			ZED_UINT32		m_Height;
			ZED_BOOL		m_Running;
			ZED_BOOL		m_Resized;
			ZED_BOOL		m_Moved;
			ZED_BOOL		m_MouseGrabbed;
			ZED_BOOL		m_ConfineMouse;
			ZED_BOOL		m_MouseCentred;
			ZED_SINT32		m_MouseAccelerationNumerator;
			ZED_SINT32		m_MouseAccelerationDenominator;
			ZED_SINT32		m_MouseAccelerationThreshold;

			Cursor NullCursor( );
		};
	}
}

#endif // __ZED_SYSTEM_LINUXWINDOW_HPP__

