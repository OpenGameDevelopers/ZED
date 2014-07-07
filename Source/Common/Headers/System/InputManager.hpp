#ifndef __ZED_SYSTEM_INPUTMANAGER_HPP__
#define __ZED_SYSTEM_INPUTMANAGER_HPP__

#include <System/DataTypes.hpp>
#include <System/Window.hpp>
#include <vector>
#include <map>

namespace ZED
{
	namespace System
	{
		class InputDevice;
		class InputManager
		{
		public:
			virtual ~InputManager( ) { }

			virtual ZED_UINT32 Initialise( ) = 0;
			virtual ZED_UINT32 AddDevice( InputDevice *p_pDevice ) = 0;
			virtual ZED_UINT32 GetDevice( InputDevice **p_ppDevice,
				const ZED_UINT32 p_Type ) const = 0;
			virtual void Update( ) = 0;
			ZED_INLINE ZED_UINT32 Types( ) const { return m_Types; }

			virtual ZED_UINT32 SetWindowData(
				const WINDOWDATA &p_WindowData ) = 0;

			virtual ZED_BYTE MapKeyToChar( const ZED_SINT32 p_Key ) = 0;

		protected:
			typedef std::vector< InputDevice * > InputDeviceArray;
			typedef std::map< ZED_UINT32, ZED_UINT32 > InputDeviceTypeCountMap;

			ZED_UINT32	m_Types;

			InputDeviceArray		m_InputDevices;
			InputDeviceTypeCountMap	m_InputTypeMap;
		};
	}
}

#endif

