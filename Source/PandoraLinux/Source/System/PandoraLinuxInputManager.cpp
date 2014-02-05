#include <System/PandoraLinuxInputManager.hpp>

static ZED_BYTE s_ScanToKey[ 128 ] =
{
	0, 0, 0, 0, 0, 0, 0, 0,												// 0x00
	0, ZED_KEY_ESCAPE, ZED_KEY_1, ZED_KEY_2, ZED_KEY_3, ZED_KEY_4,		// 0X08
		ZED_KEY_5, ZED_KEY_6,
	ZED_KEY_7, ZED_KEY_8, ZED_KEY_9, ZED_KEY_0, ZED_KEY_MINUS,			// 0x10
		ZED_KEY_EQUALS, ZED_KEY_BACKSPACE, ZED_KEY_TAB,
	ZED_KEY_Q, ZED_KEY_W, ZED_KEY_E, ZED_KEY_R, ZED_KEY_T, ZED_KEY_Y,	// 0x18
		ZED_KEY_U, ZED_KEY_I,
	ZED_KEY_O, ZED_KEY_P, ZED_KEY_BRACKETLEFT, ZED_KEY_BRACKETRIGHT,	// 0x20
		ZED_KEY_ENTER, ZED_KEY_CTRL, ZED_KEY_A, ZED_KEY_S,
	ZED_KEY_D, ZED_KEY_F, ZED_KEY_G, ZED_KEY_H, ZED_KEY_J, ZED_KEY_K,	// 0x28
		ZED_KEY_L, ZED_KEY_SEMICOLON,
	ZED_KEY_SINGLEQUOTE, ZED_KEY_BACKTICK, ZED_KEY_SHIFT,				// 0x30
		ZED_KEY_BACKSLASH, ZED_KEY_Z, ZED_KEY_X, ZED_KEY_C,	ZED_KEY_V,
	ZED_KEY_B, ZED_KEY_N, ZED_KEY_M, ZED_KEY_COMMA, ZED_KEY_PERIOD,		// 0x38
		ZED_KEY_FORWARDSLASH, ZED_KEY_SHIFT, ZED_KEY_NP_ASTERISK,
	ZED_KEY_ALT, ZED_KEY_SPACE, ZED_KEY_CAPSLOCK, ZED_KEY_F1,			// 0x40
		ZED_KEY_F2, ZED_KEY_F3, ZED_KEY_F4, ZED_KEY_F5,
	ZED_KEY_F6, ZED_KEY_F7, ZED_KEY_F8, ZED_KEY_F9, ZED_KEY_F10,		// 0x48
		ZED_KEY_PAUSE, 0, ZED_KEY_HOME,
	ZED_KEY_UPARROW, ZED_KEY_PGUP, ZED_KEY_NP_MINUS, ZED_KEY_LEFTARROW,	// 0x50
		ZED_KEY_NP_5, ZED_KEY_RIGHTARROW, ZED_KEY_NP_PLUS, ZED_KEY_END,
	ZED_KEY_DOWNARROW, ZED_KEY_PGDN, ZED_KEY_INS, ZED_KEY_DEL, 0, 0,	// 0x58
		ZED_KEY_BACKSLASH, ZED_KEY_F11,
	ZED_KEY_F12, ZED_KEY_HOME, ZED_KEY_UPARROW, ZED_KEY_PGUP,			// 0x60
		ZED_KEY_LEFTARROW, 0, ZED_KEY_RIGHTARROW, ZED_KEY_END,
	ZED_KEY_DOWNARROW, ZED_KEY_PGDN, ZED_KEY_INS, ZED_KEY_DEL,			// 0x68
		ZED_KEY_ENTER, ZED_KEY_CTRL, ZED_KEY_PAUSE, 0,
	ZED_KEY_BACKSLASH, ZED_KEY_ALT, 0, 0, 0, 0, 0, 0,					// 0x70
	0, 0, 0, 0, 0, 0, 0, 0												// 0x78
};

namespace ZED
{
	namespace System
	{
		PandoraLinuxInputManager::PandoraLinuxInputManager(
			const Display *p_pDisplay )
		{
			m_pDisplay = const_cast< Display * >( p_pDisplay );
			m_pKeyboard = ZED_NULL;
			m_Types = 0x00000000;
		}

		PandoraLinuxInputManager::~PandoraLinuxInputManager( )
		{
		}

		ZED_UINT32 PandoraLinuxInputManager::Initialise( )
		{
			return ZED_OK;
		}

		ZED_UINT32 PandoraLinuxInputManager::AddDevice(
			InputDevice *p_pDevice )
		{
			if( !p_pDevice )
			{
				zedTrace( "[ZED::System::PandoraLinuxInputManager::AddDevice] "
					"<ERROR> Device is not valid\n" );
				
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

		ZED_UINT32 PandoraLinuxInputManager::GetDevice(
			InputDevice **p_ppDevice, const ZED_UINT32 p_Type ) const
		{
			if( m_Types & p_Type )
			{
				switch( p_Type )
				{
					case ZED_INPUT_DEVICE_KEYBOARD:
					{
						( *p_ppDevice ) = m_pKeyboard;
						return ZED_OK;
					}
				}
			}

			return ZED_FAIL;
		}

		void PandoraLinuxInputManager::Update( )
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
						m_pKeyboard->KeyDown(
							s_ScanToKey[ pKeyEvent->keycode ] );
						break;
					}
					case KeyRelease:
					{
						if( !m_pKeyboard )
						{
							break;
						}
						pKeyEvent->keycode &= 0x7F;
						m_pKeyboard->KeyUp(
							s_ScanToKey[ pKeyEvent->keycode ] );
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

		ZED_BYTE PandoraLinuxInputManager::MapKeyToChar(
			const ZED_SINT32 p_Key )
		{
			return 0x00;
		}
	}
}

