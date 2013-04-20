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
			if( m_pKeyboard )
			{
				delete m_pKeyboard;
				m_pKeyboard = ZED_NULL;
			}
		}

		ZED_UINT32 LinuxInputManager::Initialise( )
		{
			return ZED_OK;
		}

		ZED_UINT32 LinuxInputManager::AddDevice( const InputDevice *p_pDevice )
		{
			if( p_pDevice->Type( ) == ZED_INPUT_DEVICE_KEYBOARD )
			{
				m_pKeyboard = new Keyboard( );
				
				return ZED_OK;
			}

			return ZED_FAIL;
		}

		void LinuxInputManager::Update( )
		{
			static XEvent Event;
			static KeySym Key;

			while( XPending( m_pDisplay ) > 0 )
			{
				XNextEvent( m_pDisplay, &Event );

				switch( Event.type )
				{
					case KeyPress:
					{
						if( !m_pKeyboard )
						{
							break;
						}
						Key = XLookupKeysym( &Event.xkey, 0 );
						zedTrace( "Sending key: %d\n", Key );
						zedDebugBreak( );
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
						break;
					}
				}
			}
		}
	}
}

