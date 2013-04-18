#ifndef __ZED_SYSTEM_INPUTDEVICE_HPP__
#define __ZED_SYSTEM_INPUTDEVICE_HPP__

namespace ZED
{
	namespace System
	{
		class InputDevice
		{
		public:
			virtual ~InputDevice( ) { }

			virtual int State( void *p_pState,
				const ZED_INPUT_CONTROL_TYPE p_Type ) = 0;

			virtual ZED_INPUT_DEVICE_TYPE Type( )=0;
		};
	}
}

#endif

