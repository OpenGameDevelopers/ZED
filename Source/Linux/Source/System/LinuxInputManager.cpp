#include <System/LinuxInputManager.hpp>
#include <X11/XKBlib.h>
#include <cstring>
#include <X11/Xutil.h>

static ZED_BYTE s_ScanToKey[ 128 ] =
{
	0, 0, 0, 0, 0, 0, 0, 0,												// 0x00
	0, K_ESCAPE, '1', '2', '3', '4', '5', '6',							// 0x08
	'7', '8', '9', '0', '-', '=', K_BACKSPACE, K_TAB,					// 0x10
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',								// 0x18
	'o', 'p', '[', ']', K_ENTER, K_CTRL, 'a', 's',						// 0x20
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',								// 0x28
	'\'', '`', K_SHIFT, '\\', 'z', 'x', 'c', 'v',						// 0x30
	'b', 'n', 'm', ',', '.', '/', K_SHIFT, K_NP_ASTERISK,				// 0x38
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5,				// 0x40
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0, K_HOME,					// 0x48
	K_UPARROW, K_PGUP, K_NP_MINUS, K_LEFTARROW, K_NP_5, K_RIGHTARROW,	// 0x50
		K_NP_PLUS, K_END,
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '\\', K_F11,				// 0x58
	K_F12, K_HOME, K_UPARROW, K_PGUP, K_LEFTARROW, 0, K_RIGHTARROW,		// 0x60
	K_END,
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, K_ENTER, K_CTRL, K_PAUSE, 0,		// 0x68
	'/', K_ALT, 0, 0, 0, 0, 0, 0,										// 0x70
	0, 0, 0, 0, 0, 0, 0, 0												// 0x78
};

namespace ZED
{
	namespace System
	{
		LinuxInputManager::LinuxInputManager( const WINDOWDATA &p_WindowData )
		{
			m_pDisplay = p_WindowData.pX11Display;
			m_Window = p_WindowData.X11Window;
			m_pKeyboard = ZED_NULL;
			m_pMouse = ZED_NULL;
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
				XAutoRepeatOn( m_pDisplay );
								
				return ZED_OK;
			}

			if( p_pDevice->Type( ) == ZED_INPUT_DEVICE_MOUSE )
			{
				m_pMouse = dynamic_cast< Mouse * >( p_pDevice );
				m_Types |= ZED_INPUT_DEVICE_MOUSE;

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
					case ZED_INPUT_DEVICE_MOUSE:
					{
						( *p_pDevice ) = m_pMouse;
						break;
					}
				}
			}

			return ZED_FAIL;
		}

		ZED_BYTE LinuxInputManager::MapKeyToChar( const ZED_SINT32 p_Key )
		{
			int			Key;
			XkbStateRec	KeyboardState;
			XEvent		Event;
			KeySym		Symbol;
			int			LookupRet;
			char		Buffer[ 5 ];

			XkbGetState( m_pDisplay, XkbUseCoreKbd, &KeyboardState );

			for( Key = 0; Key < 128; ++Key )
			{
				if( p_Key == s_ScanToKey[ Key ] )
				{
					break;
				}
			}

			if( Key == 128 )
			{
				zedTrace( "[ZED::System::LinuxInputManager::MapKeyToZED] "
					"<WARNING> Mapping key 0x%02X to a keycode failed\n",
					p_Key );
				return static_cast< ZED_BYTE >( p_Key );
			}

			memset( &Event, 0, sizeof( XEvent ) );
			Event.xkey.type = KeyPress;
			Event.xkey.display = m_pDisplay;
			Event.xkey.time = CurrentTime;
			Event.xkey.keycode = Key;
			Event.xkey.state = KeyboardState.group << 13;

			LookupRet = XLookupString( ( XKeyEvent * )&Event, Buffer,
				sizeof( Buffer ), &Symbol, ZED_NULL );

			if( LookupRet <= 0 )
			{
				zedTrace( "[ZED::System::LinuxInputManager::MapKeyToZED] "
					"<WARNING> Failed to lookup key 0x%X\n", p_Key );
				return static_cast< ZED_BYTE >( p_Key );
			}
			if( LookupRet > 1 )
			{
				zedTrace( "[ZED::System::LinuxInputManager::MapKeyToZED] "
					"<WARNING> XLookupString returned: '%s'\n", Buffer );
				return static_cast< ZED_BYTE >( p_Key );
			}

			zedTrace( "Buffer: 0x%02X | %c\n", Buffer[ 0 ], Buffer[ 0 ] );

			return Buffer[ 0 ];
		}

		void LinuxInputManager::Update( )
		{
			static XEvent Event;
			static XKeyEvent *pKeyEvent =
				reinterpret_cast< XKeyEvent * >( &Event );
			static XButtonEvent *pButtonEvent =
				reinterpret_cast< XButtonEvent * >( &Event );
			static XMotionEvent *pMotionEvent =
				reinterpret_cast< XMotionEvent * >( &Event );

			int Pending = XPending( m_pDisplay );
			Time ButtonPressTime[ 2 ] = { 0, 0 };
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
						if( !this->RepeatKeyPress( &Event ) )
						{
							pKeyEvent->keycode &= 0x7F;
							m_pKeyboard->KeyUp(
								s_ScanToKey[ pKeyEvent->keycode ] );
						}
						break;
					}
					case ButtonPress:
					{
						if( !m_pMouse )
						{
							break;
						}

						m_pMouse->ButtonDown( pButtonEvent->button );

						if( ( pButtonEvent->button == 4 ) ||
							( pButtonEvent->button == 5 ) )
						{
							ButtonPressTime[ pButtonEvent->button - 4 ] =
								pButtonEvent->time;
						}
						
						break;
					}
					case ButtonRelease:
					{
						if( !m_pMouse )
						{
							break;
						}
						
						if( ( pButtonEvent->button == 4 ) )
						{
							if( pButtonEvent->time == ButtonPressTime[ 0 ] )
							{
								XEvent NewEvent;
								memcpy( &NewEvent, &Event, sizeof( Event ) );
								NewEvent.xbutton.time++;
								XSendEvent( m_pDisplay, m_Window, True,
									ButtonPressMask, &NewEvent );
								break;
							}
						}
						if( pButtonEvent->button == 5 )
						{
							if( pButtonEvent->time == ButtonPressTime[ 1 ] )
							{
								XEvent NewEvent;
								memcpy( &NewEvent, &Event, sizeof( Event ) );
								NewEvent.xbutton.time++;
								XSendEvent( m_pDisplay, m_Window, True,
									ButtonPressMask, &NewEvent );
								break;
							}
						}

						m_pMouse->ButtonUp( pButtonEvent->button );

						break;
					}
					case MotionNotify:
					{
						if( !m_pMouse )
						{
							break;
						}

						m_pMouse->Position( pMotionEvent->x, pMotionEvent->y );

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

