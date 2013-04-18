#ifndef __ZED_SYSTEM_INPUTDEVICE_HPP__
#define __ZED_SYSTEM_INPUTDEVICE_HPP__

#include <InputTypes.hpp>

namespace ZED
{
	namespace System
	{
		class InputDevice
		{
		public:
			virtual ~InputDevice( ) { }

			virtual int State( void *p_pState ) = 0;

			virtual ZED_INPUT_DEVICE_TYPE Type( )=0;
		};
	}
}

#endif

