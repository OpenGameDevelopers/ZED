#include <System/LinuxInputManager.hpp>
#include <X11/XKBlib.h>
#include <cstring>
#include <X11/Xutil.h>
#include <System/Keyboard.hpp>
#include <System/Mouse.hpp>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include <System/Memory.hpp>
#include <X11/Xatom.h>

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
		LinuxInputManager::LinuxInputManager( ) :
			m_pDisplay( ZED_NULL ),
			m_Window( ( ::Window )0 ),
			m_pKeyboard( ZED_NULL ),
			m_pMouse( ZED_NULL )
		{
			ZED::System::InputManager::m_Types = 0x00000000;
		}

		LinuxInputManager::~LinuxInputManager( )
		{
			std::vector< NATIVEINPUT >::iterator NativeItr =
				m_RealInputDevices.begin( );

			while( NativeItr != m_RealInputDevices.end( ) )
			{
				if( ( *NativeItr ).Free == ZED_FALSE )
				{
					XCloseDevice( m_pDisplay, ( *NativeItr ).pDevice );
				}

				++NativeItr;
			}
		}

		ZED_UINT32 LinuxInputManager::Initialise(
			const WINDOWDATA &p_WindowData )
		{
			if( p_WindowData.pX11Display == ZED_NULL )
			{
				zedTrace( "[ZED::System::LinuxInputManager::SetWindowData] "
					"<ERROR> Display invalid\n" );

				return ZED_FAIL;
			}

			if( p_WindowData.X11Window == ( ::Window )0 )
			{
				zedTrace( "[ZED::System::LinuxInputManager::SetWindowData] "
					"<ERROR> Window invalid\n" );

				return ZED_FAIL;
			}
			
			m_pDisplay = p_WindowData.pX11Display;
			m_Window = p_WindowData.X11Window;

			int DeviceCount = 0;
			XListInputDevices( m_pDisplay, &DeviceCount );

			XDeviceInfo *pXDevices = new XDeviceInfo[ DeviceCount ];

			pXDevices = XListInputDevices( m_pDisplay, &DeviceCount );

			Atom MouseAtom = XInternAtom( m_pDisplay, XI_MOUSE, True );
			Atom KeyboardAtom = XInternAtom( m_pDisplay, XI_KEYBOARD, True );

			zedTrace( "[ZED::System::LinuxInputManager::Initialise] <INFO> "
				"Registering %d connected devices\n", DeviceCount );

			ZED_UINT32 MouseIndex = 0U;
			ZED_UINT32 KeyboardIndex = 0U;

			for( int i = 0; i < DeviceCount; ++i )
			{
				zedTrace( "Device %d: %s\n", i, pXDevices[ i ].name );

				NATIVEINPUT NativeInput;
				NativeInput.DeviceInfo = pXDevices[ i ];
				NativeInput.pDevice = ZED_NULL;
				NativeInput.pInputDevice = ZED_NULL;
				NativeInput.Free = ZED_TRUE;

				ZED_BOOL DeviceUnknown = ZED_FALSE;

				if( pXDevices[ i ].use == IsXExtensionKeyboard )
				{
					zedTrace( "Type: Keyboard\n" );
					NativeInput.Type = ZED_INPUT_DEVICE_KEYBOARD;
					NativeInput.Index = KeyboardIndex;
					++KeyboardIndex;
				}
				else if( pXDevices[ i ].use == IsXExtensionPointer )
				{
					zedTrace( "Type: Pointer\n" );
					NativeInput.Type = ZED_INPUT_DEVICE_MOUSE;
					NativeInput.Index = MouseIndex;

					++MouseIndex;
				}
				else
				{
					zedTrace( "Type: Unknown\n" );
					DeviceUnknown = ZED_TRUE;
				}

				if( DeviceUnknown == ZED_FALSE )
				{

					int Properties;
					XDevice *pXDevice = XOpenDevice( m_pDisplay,
						pXDevices[ i ].id );
					Atom *pProperties = XListDeviceProperties( m_pDisplay,
						pXDevice, &Properties );
					ZED_BOOL ValidDevice = ZED_TRUE;

					for( int Property = 0; Property < Properties; ++Property )
					{
						Atom AtomType;
						char *pName;
						int AtomFormat;
						unsigned long ItemCount, BytesLeft;
						unsigned char *pData, *pPtr;

						pName = XGetAtomName( m_pDisplay,
							pProperties[ Property ] );


						if( XGetDeviceProperty( m_pDisplay, pXDevice,
							pProperties[ Property ], 0, 1000, False,
							AnyPropertyType, &AtomType, &AtomFormat,
							&ItemCount, &BytesLeft, &pData ) == Success )
						{
							size_t Size = 0;
							pPtr = pData;

							switch( AtomFormat )
							{
								case 8:
								{
									Size = 1;
									break;
								}
								case 16:
								{
									Size = 2;
									break;
								}
								case 32:
								{
									Size = 4;
									break;
								}
							}

							bool Done = false;

							for( int j = 0; j < ItemCount; ++j )
							{
								switch( AtomType )
								{
									case XA_INTEGER:
									{
										switch( AtomFormat )
										{
											case 8:
											{
												if( ( strncmp( pName,
													"XTEST Device", 12 ) == 0 )
													||
													( strncmp( pName,
													"Virtual Device", 14 ) ==
														0 ) )
												{
													if( *( ( char * )pPtr ) ==
														1 )
													{
														ValidDevice =
															ZED_FALSE;
													}
												}
												break;
											}
											case 16:
											{
												break;
											}
											case 32:
											{
												break;
											}
										}
									}
									default:
									{
										Done = true;
										break;
									}
								}

								pPtr += Size;

								if( Done == true )
								{
									break;
								}
							}
						}

						XFree( pData );
						XFree( pName );
					}

					if( ValidDevice )
					{
						m_RealInputDevices.push_back( NativeInput );
					}
					else
					{
						zedTrace( "Device is invalid (it may be a virtual "
							"or XTEST device)\n" );
					}

					XCloseDevice( m_pDisplay, pXDevice );
				}

				zedTrace( "\n------------------------------------------\n\n" );
			}

			zedTrace( "Total devices added: %d\n",
				m_RealInputDevices.size( ) );

			// Gamepads are not recognised under XInput (I'm not sure why not)
			// In order to get gamepads, /dev/js[x] will need to be opened
			// until there are no more js[x] devices to look at
			// For other devices, such as the Oculus Rift and Razer Hydra, they
			// use a different API which will need to be researched before
			// integration

			zedTrace( "[ZED::System::LinuxInputManager::Initialise] <INFO> "
				"Finished registering devices\n" );

			XFreeDeviceList( pXDevices );

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

			if( p_pDevice->IsUnified( ) )
			{
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
			}
			else
			{
				if( p_pDevice->Type( ) == ZED_INPUT_DEVICE_KEYBOARD )
				{
					zedTrace( "Registering keyboard...\n" );

					for( size_t i = 0; i < m_RealInputDevices.size( ); ++i )
					{
						zedTrace( "Checking device %d of %d for a keyboard "
							"slot\n", i+1, m_RealInputDevices.size( ) );
						if( ( m_RealInputDevices[ i ].Type ==
								ZED_INPUT_DEVICE_KEYBOARD ) &&
							( m_RealInputDevices[ i ].Free == ZED_TRUE ) )
						{
							zedTrace( "Found a slot\n" );

							m_RealInputDevices[ i ].Free = ZED_FALSE;
							m_RealInputDevices[ i ].pInputDevice =
								dynamic_cast< Keyboard * >( p_pDevice );

							XDevice *pXDevice = XOpenDevice( m_pDisplay,
								m_RealInputDevices[ i ].DeviceInfo.id );

							m_RealInputDevices[ i ].pDevice = pXDevice;

							XEventClass EventClass;
							unsigned char EventTypeBase;

							DeviceKeyPress( pXDevice, EventTypeBase,
								EventClass );

							XSelectExtensionEvent( m_pDisplay, m_Window,
								&EventClass, 1 );

							Keyboard *pKeyboardDevice =
								dynamic_cast< Keyboard * >(
									m_RealInputDevices[ i ].pInputDevice );

							for( int ICI = 0; ICI < pXDevice->num_classes;
								++ICI )
							{
								XInputClassInfo ClassInfo =
									pXDevice->classes[ ICI ];

								zedTrace( "Event type base: #%d: %d\n", ICI,
									ClassInfo.event_type_base );
								zedTrace( "Input class: %d\n",
									ClassInfo.input_class );

								if( ClassInfo.input_class == KeyClass )
								{
									KeyboardTypeMapInsertResult InsertRes;

									InsertRes = m_KeyboardKeyPressMap.insert(
										KeyboardTypeMapInsert(
											ClassInfo.event_type_base,
											pKeyboardDevice ) );

									if( InsertRes.second == false )
									{
										zedTrace( "Failed to add device "
											"mapping\n" );
									}
									else
									{
										zedTrace( "Mapped successfully!\n" );
									}
								}
							}

							break;
						}
					}

					m_Types |= ZED_INPUT_DEVICE_KEYBOARD;
									
					return ZED_OK;
				}

				if( p_pDevice->Type( ) == ZED_INPUT_DEVICE_MOUSE )
				{
					zedTrace( "Registering mouse...\n" );
					for( size_t i = 0; i < m_RealInputDevices.size( ); ++i )
					{
						zedTrace( "Checking device %d of %d for a mouse "
							"slot\n", i+1, m_RealInputDevices.size( ) );
						if( ( m_RealInputDevices[ i ].Type ==
								ZED_INPUT_DEVICE_MOUSE ) &&
							( m_RealInputDevices[ i ].Free == ZED_TRUE ) )
						{
							zedTrace( "Found a slot\n" );
							m_RealInputDevices[ i ].Free = ZED_FALSE;
							m_RealInputDevices[ i ].pInputDevice =
								dynamic_cast< Mouse * >( p_pDevice );
							XDevice *pXDevice = XOpenDevice(
								m_pDisplay,
								m_RealInputDevices[ i ].DeviceInfo.id );

							m_RealInputDevices[ i ].pDevice = pXDevice;

							XEventClass EventClass;
							unsigned char EventTypeBase;
							
							DeviceMotionNotify( pXDevice, EventTypeBase,
								EventClass );
							XSelectExtensionEvent( m_pDisplay, m_Window,
								&EventClass, 1 );

							Mouse *pMouseDevice =
								dynamic_cast< Mouse * >(
									m_RealInputDevices[ i ].pInputDevice );

							for( int ICI = 0; ICI < pXDevice->num_classes;
								++ICI )
							{
								XInputClassInfo ClassInfo =
									pXDevice->classes[ ICI ];

								zedTrace( "Event type base #%d: %d\n", ICI,
									ClassInfo.event_type_base );
								zedTrace( "Input class: %d\n",
									ClassInfo.input_class );

								if( ClassInfo.input_class == ValuatorClass )
								{
									MouseTypeMapInsertResult InsertRes;
									InsertRes = m_MouseMotionNotifyMap.insert( 
										MouseTypeMapInsert(
											ClassInfo.event_type_base,
											pMouseDevice ) );

									if( InsertRes.second == false )
									{
										zedTrace( "Failed to add device "
											"mapping\n" );
									}
									else
									{
										zedTrace( "Mapped successfully!\n" );
									}
								}
							}

							zedTrace( "Event Type Base: %d\n", EventTypeBase );

							break;
						}
					}

					m_Types |= ZED_INPUT_DEVICE_MOUSE;

					return ZED_OK;
				}
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
			static XDeviceMotionEvent *pDeviceMotionEvent =
				reinterpret_cast< XDeviceMotionEvent * >( &Event );
			static XDeviceKeyEvent *pDeviceKeyEvent =
				reinterpret_cast< XDeviceKeyEvent * >( &Event );

			XFlush( m_pDisplay );
			int Pending = XEventsQueued( m_pDisplay, QueuedAlready );
			XEvent QueuedEvents[ Pending ];
			memset( &QueuedEvents, 0, sizeof( XEvent ) * Pending );
			int Resend = 0;
			Time ButtonPressTime[ 2 ] = { 0, 0 };

			std::vector< NATIVEINPUT >::iterator NativeItr =
				m_RealInputDevices.begin( );

			for( ; NativeItr != m_RealInputDevices.end( ); ++NativeItr )
			{
				if( ( *NativeItr ).Free == ZED_FALSE )
				{
					if( ( *NativeItr ).Type == ZED_INPUT_DEVICE_KEYBOARD )
					{
						XDeviceState *pDeviceState = XQueryDeviceState(
							m_pDisplay, ( *NativeItr ).pDevice );
						XInputClass *pClass;

						if( pDeviceState )
						{
							pClass = pDeviceState->data;

							for( int Class = 0;
								Class < pDeviceState->num_classes; ++Class )
							{
								switch( pClass->c_class )
								{
									case KeyClass:
									{
										XKeyState *pKey =
											( XKeyState * )pClass;

										for( int Key = 0; Key < pKey->num_keys;
											++Key )
										{
											ZED_BYTE KeyValue =
												( pKey->keys[ Key / 8 ]
													& ( 1 << ( Key % 8 ) ) ) ?
													1 : 0;
											
											ZED_BYTE Code = Key;

											if( KeyValue )
											{
												reinterpret_cast< Keyboard * >(
													( *NativeItr ).pInputDevice
														)->KeyDown(
															s_ScanToKey[
																Code ] );
											}
											else
											{
												reinterpret_cast< Keyboard * >(
													( *NativeItr ).pInputDevice
														)->KeyUp(
															s_ScanToKey[
																Code ] );
											}
										}
										break;
									}
									default:
									{
										break;
									}
								}

								pClass = ( XInputClass * )( ( char * )pClass +
									pClass->length );
							}

							XFreeDeviceState( pDeviceState );
						}
					}

					if( ( *NativeItr ).Type == ZED_INPUT_DEVICE_MOUSE )
					{
						XDeviceState *pDeviceState =
							XQueryDeviceState( m_pDisplay,
							( *NativeItr ).pDevice );
						XInputClass *pClass;

						if( pDeviceState )
						{
							pClass = pDeviceState->data;

							for( int Class = 0;
								Class < pDeviceState->num_classes;
								++Class )
							{
								switch( pClass->c_class )
								{
									zedTrace( "Class: %d\n", pClass->c_class );
									case ButtonClass:
									{
										XButtonState *pButtonState =
											( XButtonState * )pClass;
										zedTrace( "Number of buttons: %d\n",
											pButtonState->num_buttons );
										for( int button = 1; button <
											pButtonState->num_buttons;
											++button )
										{
											zedTrace( "Button %d = %s\n",
												button,
												( pButtonState->buttons[
													button / 8 ] &
													( 1 << ( button % 8 ) ) ) ?
													"down" : "up" );
										}
										break;
									}
									case ValuatorClass:
									{
										XValuatorState *pValuator =
											( XValuatorState * )pClass;

										zedTrace( "Valuator count: %d\n",
											pValuator->num_valuators );
										zedTrace( "Valuator mode: %s\n",
											pValuator->mode & 1 ? "Absolute" :
												"Relative" );

										for( int Valuator = 0; Valuator <
											pValuator->num_valuators;
											++Valuator )
										{
											zedTrace( "Valuator %d: %d\n",
												Valuator,
												pValuator->valuators[
													Valuator ] );
										}/*

										if( pValuator->num_valuators >= 2 )
										{
											reinterpret_cast< Mouse * >( 
												( *NativeItr ).pInputDevice
													)->SetPosition(
													pValuator->valuators[ 0 ],
													pValuator->valuators[ 1 ] );
										}*/

										break;
									}
									default:
									{
										break;
									}
								}

								pClass = ( XInputClass * )( ( char * )pClass +
									pClass->length );
							}

							XFreeDeviceState( pDeviceState );
						}
					}
				}
			}

			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_pDisplay, &Event );

				bool EventHandled = false;

				if( !EventHandled )
				{
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
							if( this->RepeatKeyPress( &Event ) )
							{
								continue;
							}

							pKeyEvent->keycode &= 0x7F;
							m_pKeyboard->KeyUp(
								s_ScanToKey[ pKeyEvent->keycode ] );

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
								if( pButtonEvent->time ==
									ButtonPressTime[ 0 ] )
								{
									XEvent NewEvent;
									memcpy( &NewEvent, &Event,
										sizeof( Event ) );
									NewEvent.xbutton.time++;
									XSendEvent( m_pDisplay, m_Window, True,
										ButtonPressMask, &NewEvent );
									break;
								}
							}
							if( pButtonEvent->button == 5 )
							{
								if( pButtonEvent->time ==
									ButtonPressTime[ 1 ] )
								{
									XEvent NewEvent;
									memcpy( &NewEvent, &Event,
										sizeof( Event ) );
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

							m_pMouse->SetPosition( pMotionEvent->x,
								pMotionEvent->y );

							break;
						}
						default:
						{
							QueuedEvents[ Resend ] = Event;
							++Resend;
							break;
						}
					}
				}
			}

			for( int i = Resend; i > 0; --i )
			{
				XPutBackEvent( m_pDisplay, &QueuedEvents[ i-1 ] );
			}
		}

		ZED_BOOL LinuxInputManager::RepeatKeyPress( XEvent *p_pEvent )
		{
			ZED_BOOL	Repeat = ZED_FALSE;
			XEvent		Peek;
			int			LookupRet;
			char		Buff[ 5 ];
			KeySym		Key;

			if( XEventsQueued( m_pDisplay, QueuedAlready ) )
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

