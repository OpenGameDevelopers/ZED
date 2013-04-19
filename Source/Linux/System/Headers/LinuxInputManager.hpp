#ifndef __ZED_SYSTEM_LINUXINPUTMANAGER_HPP__
#define __ZED_SYSTEM_LINUXINPUTMANAGER_HPP__

#include <InputManager.hpp>
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
			virtual ZED_UINT32 AddDevice( const InputDevice &p_Device );
			virtual void Update( );

		private:
			Display *m_pDisplay;
		};
	}
}

#endif

