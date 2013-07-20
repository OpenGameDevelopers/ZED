#include <System/LinuxInputManager.hpp>

static ZED_BYTE s_ScanToKey[ 128 ] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, K_ESCAPE, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', K_BACKSPACE, K_TAB,
	'q', 'w', 'e', 'r', 't', 'y', 'i',
	'o', 'p', '[', ']', K_ENTER, K_CTRL, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', K_SHIFT, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', K_SHIFT, K_NP_ASTERISK,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5,
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0, K_HOME,
	K_UPARROW, K_PGUP, K_NP_MINUS, K_LEFTARROW, K_NP_5, K_RIGHTARROW,
		K_NP_PLUS, K_END,
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, K_ENTER, K_CTRL, K_PAUSE, 0,
	'/', K_ALT, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

namespace ZED
{
	namespace System
	{
		LinuxInputManager::LinuxInputManager( const Display *p_pDisplay )
		{
			m_pDisplay = const_cast< Display * >( p_pDisplay );
			m_pKeyboard = ZED_NULL;
			m_Types = 0x00000000;
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
			if( !p_pDevice )
			{
				zedTrace( "[ZED::System::LinuxInputManager::AddDevice] <ERROR>"
					" Device is not valid\n" );
				return ZED_FAIL;
			}

			if( p_pDevice->Type( ) == ZED_INPUT_DEVICE_KEYBOARD )
			{
				m_pKeyboard = dynamic_cast< Keyboard * >( p_pDevice );
				m_Types |= ZED_INPUT_DEVICE_KEYBOARD;
								
				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 LinuxInputManager::GetDevice( InputDevice **p_pDevice,
			const ZED_UINT32 p_Type ) const
		{
			if( m_Types & p_Type )
			{
				switch( p_Type )
				{
					case ZED_INPUT_DEVICE_KEYBOARD:
					{
						( *p_pDevice ) = m_pKeyboard;
						break;
					}
				}
			}

			return ZED_FAIL;
		}

		void LinuxInputManager::Update( )
		{
			static XEvent Event;
			static XKeyEvent *pKeyEvent =
				reinterpret_cast< XKeyEvent * >( &Event );

			int Pending = XPending( m_pDisplay );
			for( int i = 0; i < Pending; ++i )
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
						pKeyEvent->keycode &= 0x7F;
						m_pKeyboard->KeyDown( s_ScanToKey[ pKeyEvent->keycode ] );
						break;
					}
					case KeyRelease:
					{
						if( !m_pKeyboard )
						{
							break;
						}
						pKeyEvent->keycode &= 0x7F;
						m_pKeyboard->KeyUp( s_ScanToKey[ pKeyEvent->keycode ] );
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

		ZED_BOOL LinuxInputManager::RepeatKeyPress( XEvent *p_pEvent )
		{
			ZED_BOOL	Repeat = ZED_FALSE;
			XEvent		Peek;
			int			LookupRet;
			char		Buff[ 5 ];
			KeySym		Key;

			if( XPending( m_pDisplay ) )
			{
				XPeekEvent( m_pDisplay, &Peek );

				if( ( Peek.type == KeyPress ) &&
					( Peek.xkey.keycode == p_pEvent->xkey.keycode ) &&
					( Peek.xkey.time == p_pEvent->xkey.time ) )
				{
					Repeat = ZED_TRUE;

					XNextEvent( m_pDisplay, &Peek );
				}
			}

			return Repeat;
		}
	}
}

