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

			ZED_UINT32 BindKey( const ZED_KEY p_Key,
				const ZED_UINT32 p_Action );

			ZED_UINT32 GetActionFromKey( const ZED_KEY p_Key ) const;

		private:
			// Keyboard
			typedef std::map< ZED_KEY, ZED_UINT32 > KeyMap;
			typedef std::pair< ZED_KEY, ZED_UINT32 > KeyMapEntry;
			typedef std::pair< KeyMap::iterator, bool > KeyMapInsertResult;
			// Mouse
			typedef std::map< ZED_MOUSE_BUTTON, ZED_UINT32 > MouseButtonMap;
			typedef std::map< ZED_MOUSE_AXIS, ZED_UINT32 > MouseAxisMap;
			// Gamepad
			typedef std::map< ZED_GAMEPAD, ZED_UINT32 > GamepadMap;

			InputBinder( const InputBinder & );
			InputBinder &operator=( const InputBinder & );

			KeyMap			m_KeyToAction;
			MouseButtonMap	m_MouseButtonToAction;
			MouseAxisMap	m_MouseCoordinateAxisToAction;
			GamepadMap		m_GamepadToAction;
		};
	}
}

#endif

