#ifndef __ZED_SYSTEM_LINUXINPUTMANAGER_HPP__
#define __ZED_SYSTEM_LINUXINPUTMANAGER_HPP__

#include <System/InputManager.hpp>
#include <System/Keyboard.hpp>
#include <X11/Xlib.h>

namespace ZED
{
	namespace System
	{
		class LinuxInputManager : public InputManager
		{
		public:
			ZED_EXPLICIT LinuxInputManager( const Display *p_pDisplay );
			virtual ~LinuxInputManager( );

			virtual ZED_UINT32 Initialise( );
			virtual ZED_UINT32 AddDevice( InputDevice *p_pDevice );
			virtual ZED_UINT32 GetDevice( InputDevice **p_ppDevice,
				const ZED_UINT32 p_Type ) const;

			virtual ZED_BYTE MapKeyToChar( const ZED_SINT32 p_Key );

			virtual void Update( );

		private:
			Display		*m_pDisplay;
			Keyboard	*m_pKeyboard;

			ZED_BOOL	RepeatKeyPress( XEvent *p_pEvent );
		};
	}
}

#endif

