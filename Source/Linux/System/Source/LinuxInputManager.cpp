#include <LinuxInputManager.hpp>

namespace ZED
{
	namespace System
	{
		LinuxInputManager::LinuxInputManager( const Display *p_pDisplay )
		{
			m_pDisplay = const_cast< Display * >( p_pDisplay );
		}

		LinuxInputManager::~LinuxInputManager( )
		{
		}

		ZED_UINT32 LinuxInputManager::Initialise( )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxInputManager::AddDevice( const InputDevice &p_Device )
		{
			return ZED_OK;
		}

		void LinuxInputManager::Update( )
		{
			static XEvent Event;

			while( XPending( m_pDisplay ) )
			{
				XNextEvent( m_pDisplay, &Event );

				switch( Event.type )
				{
					case KeyPress:
					{
						break;
					}
					case KeyRelease:
					{
						break;
					}
					case ButtonPress:
					{	
						break;
					}
					case ButtonRelease:
					{
						break;
					}
					case MotionNotify:
					{
						break;
					}
				}
			}
		}
	}
}

