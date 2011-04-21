#ifndef __ZED_SYSTEM_XBOXINPUTMANAGER_HPP__
#define __ZED_SYSTEM_XBOXINPUTMANAGER_HPP__

#include <InputManager.hpp>

namespace ZED
{
	namespace System
	{
		// As the Xbox has four controller ports, pre-define them
		const ZED_UINT32 PORT_COUNT = 4;
		// Also, each controller has a maximum of two available slots
		const ZED_UINT32 SLOT_COUNT = 2;

		class XboxInputManager : public InputManager
		{
			virtual ZED_UINT32 Initialise( );
			virtual ZED_UINT32 AddDevice( );
			virtual void Update( );
		};
	}
}

#endif
