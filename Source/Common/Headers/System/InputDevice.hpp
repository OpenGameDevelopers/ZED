#ifndef __ZED_SYSTEM_INPUTDEVICE_HPP__
#define __ZED_SYSTEM_INPUTDEVICE_HPP__

#include <System/InputTypes.hpp>

namespace ZED
{
	namespace System
	{
		class InputDevice
		{
		public:
			virtual ~InputDevice( ) { }

			virtual ZED_UINT32 State( void *p_pState ) const = 0;

			virtual ZED_UINT32 Type( ) const = 0;
		};
	}
}

#endif

