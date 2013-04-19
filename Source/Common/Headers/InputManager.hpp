#ifndef __ZED_SYSTEM_INPUTMANAGER_HPP__
#define __ZED_SYSTEM_INPUTMANAGER_HPP__

#include <DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		class InputDevice;
		class InputManager
		{
		public:
			virtual ~InputManager( ) { }

			virtual ZED_UINT32 Initialise( )=0;
			virtual ZED_UINT32 AddDevice( const InputDevice p_Device )=0;
			virtual void Update( )=0;
		};
	}
}

#endif

