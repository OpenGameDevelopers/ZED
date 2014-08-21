#include <System/InputDevice.hpp>

namespace ZED
{
	namespace System
	{
		InputDevice::InputDevice( ) :
			m_DeviceOpen( ZED_FALSE ),
			m_Connected( ZED_FALSE ),
			m_Unified( ZED_FALSE )
		{
		}

		InputDevice::~InputDevice( )
		{
			if( m_DeviceOpen )
			{
			}
		}

		ZED_BOOL InputDevice::IsConnected( ) const
		{
			return m_Connected;
		}

		void InputDevice::SetConnected( )
		{
			m_Connected = ZED_TRUE;
		}

		void InputDevice::SetDisconnected( )
		{
			m_Connected = ZED_FALSE;
		}

		ZED_BOOL InputDevice::IsUnified( ) const
		{
			return m_Unified;
		}

		void InputDevice::SetUnified( )
		{
			m_Unified = ZED_TRUE;
		}

		void InputDevice::SetIndividual( )
		{
			m_Unified = ZED_FALSE;
		}
	}
}

