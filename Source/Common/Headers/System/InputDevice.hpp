#ifndef __ZED_SYSTEM_INPUTDEVICE_HPP__
#define __ZED_SYSTEM_INPUTDEVICE_HPP__

#include <System/InputTypes.hpp>

#ifdef ZED_WINDOWSYSTEM_X11
#include <X11/extensions/XInput.h>
#else
#error Unknown windowing system
#endif

namespace ZED
{
	namespace System
	{
		class InputDevice
		{
		public:
			InputDevice( );
			virtual ~InputDevice( );

			virtual ZED_UINT32 State( void *p_pState ) const = 0;

			virtual ZED_UINT32 Type( ) const = 0;

			ZED_BOOL IsConnected( ) const;

			void SetConnected( );
			void SetDisconnected( );

			ZED_BOOL IsUnified( ) const;

			void SetUnified( );
			void SetIndividual( );

#ifdef ZED_WINDOWSYSTEM_X11
//			void SetXDeviceInfo( const XDeviceInfo &
#endif

		protected:
#ifdef ZED_WINDOWSYSTEM_X11
			XDeviceInfo	m_XDeviceInfo;
#endif
			ZED_BOOL	m_DeviceOpen;
			ZED_BOOL	m_Connected;
			ZED_BOOL	m_Unified;
		};
	}
}

#endif // __ZED_SYSTEM_INPUTDEVICE_HPP__

