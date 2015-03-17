#ifndef __ZED_SYSTEM_LINUXWINDOWDATA_HPP__
#define __ZED_SYSTEM_LINUXWINDOWDATA_HPP__

#include <System/WindowData.hpp>

#include <X11/Xlib.h>

namespace ZED
{
	namespace System
	{
		class LinuxWindowData : public WindowData
		{
		public:
			LinuxWindowData( );
			LinuxWindowData( Display *p_pDisplay,
				const ::Window &p_Window, Screen *p_pScreen );
			LinuxWindowData( const LinuxWindowData &p_WindowData );
			virtual ~LinuxWindowData( );

			LinuxWindowData &operator=( const LinuxWindowData &p_Other );

			Display *GetDisplay( ) const;
			::Window GetWindow( ) const;
			Screen *GetScreen( ) const;

		private:
			Display		*m_pDisplay;
			::Window	m_Window;
			Screen		*m_pScreen;
		};
	}
}

#endif // __ZED_SYSTEN_LINUXWINDOWDATA_HPP__

