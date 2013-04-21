#include <LinuxInputManager.hpp>

namespace ZED
{
	namespace System
	{
		LinuxInputManager::LinuxInputManager( const Display *p_pDisplay )
		{
			m_pDisplay = const_cast< Display * >( p_pDisplay );
			m_pKeyboard = ZED_NULL;
		}

		LinuxInputManager::~LinuxInputManager( )
		{
		}

		ZED_UINT32 LinuxInputManager::Initialise( )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxInputManager::AddDevice( InputDevice *p_pDevice )
		{
			if( p_pDevice->Type( ) == ZED_INPUT_DEVICE_KEYBOARD )
			{
				m_pKeyboard = dynamic_cast< Keyboard * >( p_pDevice );
				
				return ZED_OK;
			}

			return ZED_FAIL;
		}

		void LinuxInputManager::Update( )
		{
			static XEvent Event;
			static KeySym Key;

			int Pending = XPending( m_pDisplay );
			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_pDisplay, &Event );
				//zedTrace( "Received event: %d\n", Event.type );

				switch( Event.type )
				{
					case KeyPress:
					{
						if( !m_pKeyboard )
						{
							break;
						}
						Key = XLookupKeysym( &Event.xkey, 0 );
						m_pKeyboard->KeyDown( Key );
						break;
					}
					case KeyRelease:
					{
						if( !m_pKeyboard )
						{
							break;
						}
						Key = XLookupKeysym( &Event.xkey, 0 );
						m_pKeyboard->KeyUp( Key );
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
					default:
					{
						XPutBackEvent( m_pDisplay, &Event );
						break;
					}
				}
			}
		}
	}
}

