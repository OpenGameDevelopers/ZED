#ifndef __ZED_RENDERER_LINUXWINDOW_HPP__
#define __ZED_RENDERER_LINUXWINDOW_HPP__

#include <Window.hpp>
#include <X11/Xlib.h>

namespace ZED
{
	namespace Renderer
	{
		class LinuxWindow : public ZED::Renderer::Window
		{
		public:
			LinuxWindow( );
			virtual ~LinuxWindow( );

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual void Destroy( );

			virtual ZED_UINT32 Update( );

			virtual ZED_WINDOWDATA WindowData( ) const { return m_WindowData; }

			virtual void HideCursor( );
			virtual void ShowCursor( );
			virtual ZED_BOOL ToggleCursor( );

		private:
			::Window		m_Window;
			Display			*m_pDisplay;
			Screen			m_Screen;
			XVisualInfo		*m_pVisualInfo;
			ZED_WINDOWDATA	m_WindowData;
			ZED_BOOL		m_CursorHidden;

			Cursor NullCursor( );
		};
	}
}

#endif

