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

			virtual ZED_UINT32 Create( );

		private:
			::Window	m_Window;
			Display		*m_pDisplay;
			Screen		m_Screen;
		};
	}
}

#endif

