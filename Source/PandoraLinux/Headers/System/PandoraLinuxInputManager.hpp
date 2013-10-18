#ifndef __ZED_SYSTEM_PANDORALINUXINPUTMANAGER_HPP__
#define __ZED_SYSTEM_PANDORALINUXINPUTMANAGER_HPP__

#include <System/InputManager.hpp>
#include <System/Keyboard.hpp>
#include <X11/Xlib.h>

namespace ZED
{
	namespace System
	{
		class InputDevice;

		class PandoraLinuxInputManager : public InputManager
		{
		public:
			ZED_EXPLICIT PandoraLinuxInputManager( const Display *p_pDisplay );
			virtual ~PandoraLinuxInputManager( );

			virtual ZED_UINT32 Initialise( );
			virtual ZED_UINT32 AddDevice( InputDevice *p_pDevice );
			virtual ZED_UINT32 GetDevice( InputDevice **p_ppDevice,
				const ZED_UINT32 p_Type ) const;
			virtual void Update( );
			virtual ZED_BYTE MapKeyToChar( const ZED_SINT32 p_Key );

		private:
			PandoraLinuxInputManager( );

			Display		*m_pDisplay;
			Keyboard	*m_pKeyboard;
		};
	}
}

#endif // __ZED_SYSTEM_PANDORALINUXINPUTMANAGER_HPP__

