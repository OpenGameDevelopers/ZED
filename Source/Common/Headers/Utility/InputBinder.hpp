#ifndef __ZED_UTILITY_INPUTBINDER_HPP__
#define __ZED_UTILITY_INPUTBINDER_HPP__

#include <System/InputTypes.hpp>
#include <map>

namespace ZED
{
	namespace Utility
	{
		class InputBinder
		{
		public:
			InputBinder( );
			~InputBinder( );

		private:
			InputBinder( const InputBinder & );
			InputBinder &operator=( const InputBinder & );

			std::map< ZED_KEY, ZED_UINT32 > m_KeyToAction;
			std::map< ZED_MOUSE_BUTTON, ZED_UINT32 > m_MouseButtonToAction;
			std::map< ZED_MOUSE_COORDINATE_AXIS, ZED_UINT32 >
				m_MouseCoordinateAxisToAction;
			std::map< ZED_GAMEPAD, ZED_UINT32 > m_GamepadToAction;
		};
	}
}

#endif

