#include <System/InputDevice.hpp>

namespace ZED
{
	namespace System
	{
		InputDevice::InputDevice( )
		{
			m_Connected = ZED_FALSE;
		}

		InputDevice::~InputDevice( )
		{
			if( m_DeviceOpen )
			{
			}
		}

		ZED_BOOL InputDevice::IsConnected( )
		{
			return m_Connected;
		}
	}
}

