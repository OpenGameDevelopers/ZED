#ifndef __ZED_SYSTEM_X11WINDOWDATA_HPP__
#define __ZED_SYSTEM_X11WINDOWDATA_HPP__

#include <System/WindowData.hpp>

#include <X11/Xlib.h>

namespace ZED
{
	namespace System
	{
		class X11WindowData : public WindowData
		{
		public:
			X11WindowData( );
			X11WindowData( Display *p_pDisplay,
				const ::Window &p_Window, Screen *p_pScreen );
			X11WindowData( const X11WindowData &p_WindowData );
			virtual ~X11WindowData( );

			X11WindowData &operator=( const X11WindowData &p_Other );

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

#endif // __ZED_SYSTEN_X11WINDOWDATA_HPP__

