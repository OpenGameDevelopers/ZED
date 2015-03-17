#include <System/Window.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace System
	{
		Window::~Window( )
		{
			zedSafeDeleteArray( m_pTitle );
		}
	}
}

