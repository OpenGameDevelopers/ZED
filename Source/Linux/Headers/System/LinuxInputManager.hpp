#ifndef __ZED_SYSTEM_LINUXINPUTMANAGER_HPP__
#define __ZED_SYSTEM_LINUXINPUTMANAGER_HPP__

#include <System/InputManager.hpp>
#include <vector>
#include <map>
#include <X11/extensions/XInput.h>

namespace ZED
{
	namespace System
	{
		class Keyboard;
		class Mouse;

		typedef struct __ZED_NATIVEINPUT
		{
			XDeviceInfo	DeviceInfo;
			XDevice		*pDevice;
			InputDevice	*pInputDevice;
			ZED_UINT32	Index;
			ZED_UINT32	Type;
			ZED_BOOL	Free;
		}NATIVEINPUT;

		class LinuxInputManager : public InputManager
		{
		public:
			LinuxInputManager( );
			virtual ~LinuxInputManager( );

			virtual ZED_UINT32 Initialise( const WindowData &p_WindowData );
			virtual ZED_UINT32 AddDevice( InputDevice *p_pDevice );
			virtual ZED_UINT32 GetDevice( InputDevice **p_ppDevice,
				const ZED_UINT32 p_Type ) const;

			virtual ZED_BYTE MapKeyToChar( const ZED_SINT32 p_Key );

			virtual void Update( );

		private:
			typedef std::map< unsigned char, Mouse * > MouseTypeMap;
			typedef std::pair< unsigned char, Mouse * > MouseTypeMapInsert;
			typedef std::pair< MouseTypeMap::iterator, bool >
				MouseTypeMapInsertResult;

			typedef std::map< unsigned char, Keyboard * >KeyboardTypeMap;
			typedef std::pair< unsigned char, Keyboard * >
				KeyboardTypeMapInsert;
			typedef std::pair< KeyboardTypeMap::iterator, bool >
				KeyboardTypeMapInsertResult;

			Display		*m_pDisplay;
			::Window	m_Window;
			Keyboard	*m_pKeyboard;
			Mouse		*m_pMouse;

			ZED_BOOL	RepeatKeyPress( XEvent *p_pEvent );

			std::vector< NATIVEINPUT > m_RealInputDevices;

			MouseTypeMap	 m_MouseMotionNotifyMap;
			KeyboardTypeMap	m_KeyboardKeyPressMap;

			ZED_SINT32 m_WindowX, m_WindowY;
		};
	}
}

#endif

