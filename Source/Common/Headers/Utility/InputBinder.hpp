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
			ZED_EXPLICIT InputBinder( const ZED_UINT32 p_MaximumSlots );
			~InputBinder( );

			void SetMaximumSlots( const ZED_UINT32 p_MaximumSlots );
			ZED_UINT32 GetMaximumSlots( ) const;

			ZED_UINT32 BindKey( const ZED_KEY p_Key,
				const ZED_UINT32 p_Action );

			ZED_UINT32 GetActionFromKey( const ZED_KEY p_Key ) const;

			ZED_UINT32 GetActionsFromKey( const ZED_KEY p_Key,
				ZED_UINT32 *p_pActionID ) const;

			ZED_UINT32 GetActionCountForKey( const ZED_KEY p_Key ) const;

			void PrintBoundKeys( ) const;

			ZED_UINT32 BindMouseAxis( const ZED_MOUSE_AXIS p_Axis,
				const ZED_UINT32 p_Action );

			ZED_UINT32 GetActionFromMouseAxis(
				const ZED_MOUSE_AXIS p_Axis ) const;

			ZED_UINT32 GetActionsFromMouseAxis( const ZED_MOUSE_AXIS p_Axis,
				ZED_UINT32 *p_pActions ) const;

			ZED_UINT32 GetActionCountForMouseAxis(
				const ZED_MOUSE_AXIS p_Axis ) const;

			void PrintBoundMouseAxes( ) const;

		private:
			// Keyboard
			typedef std::multimap< ZED_KEY, ZED_UINT32 > KeyMap;
			typedef std::pair< ZED_KEY, ZED_UINT32 > KeyMapEntry;
			// Mouse
			typedef std::multimap< ZED_MOUSE_BUTTON, ZED_UINT32 >
				MouseButtonMap;
			typedef std::pair< ZED_MOUSE_BUTTON, ZED_UINT32	> MouseButtonEntry;
			typedef std::multimap< ZED_MOUSE_AXIS, ZED_UINT32 > MouseAxisMap;
			typedef std::pair< ZED_MOUSE_AXIS, ZED_UINT32 > MouseAxisEntry;
			// Gamepad
			typedef std::multimap< ZED_GAMEPAD, ZED_UINT32 > GamepadMap;
			typedef std::pair< ZED_GAMEPAD, ZED_UINT32 > GamepadEntry;

			InputBinder( const InputBinder & );
			InputBinder &operator=( const InputBinder & );

			KeyMap			m_KeyToAction;
			MouseButtonMap	m_MouseButtonToAction;
			MouseAxisMap	m_MouseAxisToAction;
			GamepadMap		m_GamepadToAction;

			ZED_UINT32		m_MaximumSlots;
		};
	}
}

#endif

