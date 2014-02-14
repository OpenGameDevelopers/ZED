#include <Utility/InputBinder.hpp>
#include <System/Debugger.hpp>

namespace ZED
{
	namespace Utility
	{
		InputBinder::InputBinder( )	:
			m_MaximumSlots( 2 )
		{
		}

		InputBinder::InputBinder( const ZED_UINT32 p_MaximumSlots ) :
			m_MaximumSlots( p_MaximumSlots )
		{
		}

		InputBinder::~InputBinder( )
		{
		}

		void InputBinder::SetMaximumSlots( const ZED_UINT32 p_MaximumSlots )
		{
			if( p_MaximumSlots == 0 )
			{
				m_MaximumSlots = 1;
			}
			else
			{
				m_MaximumSlots = p_MaximumSlots;
			}
		}

		ZED_UINT32 InputBinder::GetMaximumSlots( ) const
		{
			return m_MaximumSlots;
		}

		ZED_UINT32 InputBinder::BindKey( const ZED_KEY p_Key,
			const ZED_UINT32 p_Action )
		{
			if( m_KeyToAction.insert( KeyMapEntry( p_Key, p_Action  ) ) ==
				m_KeyToAction.end( ) )
			{
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 InputBinder::GetActionFromKey( const ZED_KEY p_Key ) const
		{
			KeyMap::const_iterator KeyAction = m_KeyToAction.find( p_Key );

			if( KeyAction == m_KeyToAction.end( ) )
			{
				zedTrace( "[ZED::Utility::InputBinder::GetActionFromkey] "
					"<ERROR> Failed to find key %s\n",
						ZED::System::KeyToString( p_Key ) );
				return 0;
			}

			return KeyAction->second;
		}

		ZED_UINT32 InputBinder::GetActionsFromKey( const ZED_KEY p_Key,
			ZED_UINT32 *p_pActionID ) const
		{
			if( p_pActionID == ZED_NULL )
			{
				zedTrace( "[ZED::Utility::InputBinder::GetActionsFromKey] "
					"<ERROR> Action ID pointer is not valid\n" );

				return ZED_FAIL;
			}

			KeyMap::const_iterator KeyActions = m_KeyToAction.begin( );
			ZED_UINT32 ActionCount = 0;

			while( KeyActions != m_KeyToAction.end( ) )
			{
				if( KeyActions->first == p_Key )
				{
					p_pActionID[ ActionCount ] = KeyActions->second;
					++ActionCount;
				}
				++KeyActions;
			}

			return ZED_OK;
		}

		ZED_UINT32 InputBinder::GetActionCountForKey( const ZED_KEY p_Key )
			const
		{
			KeyMap::const_iterator KeyActions = m_KeyToAction.begin( );
			ZED_UINT32 ActionCount = 0;

			while( KeyActions != m_KeyToAction.end( ) )
			{
				if( KeyActions->first == p_Key )
				{
					++ActionCount;
				}
				++KeyActions;
			}

			return ActionCount;
		}

		void InputBinder::PrintBoundKeys( ) const
		{
			KeyMap::const_iterator Keys = m_KeyToAction.begin( );

			zedTrace( "[ZED::System::InputBinder::PrintBoundKeys] <INFO> "
				"Bound keys:\n" );

			while( Keys != m_KeyToAction.end( ) )
			{
				zedTrace( "\t%s = %d\n",
					ZED::System::KeyToString( Keys->first ), Keys->second );
				++Keys;
			}

			zedTrace( "\n" );
		}

		ZED_UINT32 InputBinder::BindMouseAxis( const ZED_MOUSE_AXIS p_Axis,
			const ZED_UINT32 p_Action )
		{
			if( m_MouseAxisToAction.insert( MouseAxisEntry( p_Axis,
				p_Action ) ) == m_MouseAxisToAction.end( ) )
			{
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 InputBinder::GetActionFromMouseAxis(
			const ZED_MOUSE_AXIS p_Axis ) const
		{
			MouseAxisMap::const_iterator MouseAction =
				m_MouseAxisToAction.find( p_Axis );

			if( MouseAction == m_MouseAxisToAction.end( ) )
			{
				zedTrace( "[ZED::Utility::InputBinder::GetActionFromMouseAxis]"
					" <ERROR> Failed to find mouse axis: %s\n",
					ZED::System::MouseAxisToString( p_Axis ) );

				return 0;
			}

			return MouseAction->second;
		}

		ZED_UINT32 InputBinder::GetActionsFromMouseAxis(
			const ZED_MOUSE_AXIS p_Axis, ZED_UINT32 *p_pActions ) const
		{
			if( p_pActions == ZED_NULL )
			{
				zedTrace(
					"[ZED::Utility::InputBinder::GetActionsFromMouseAxis] "
					"<ERROR> Action ID pointer is not valid\n" );

				return ZED_FAIL;
			}

			MouseAxisMap::const_iterator MouseActions =
				m_MouseAxisToAction.begin( );
			ZED_UINT32 ActionCount = 0;

			while( MouseActions != m_MouseAxisToAction.end( ) )
			{
				if( MouseActions->first == p_Axis )
				{
					p_pActions[ ActionCount ] = MouseActions->second;
					++ActionCount;
				}
				++MouseActions;
			}

			return ZED_OK;
		}

		ZED_UINT32 InputBinder::GetActionCountForMouseAxis(
			const ZED_MOUSE_AXIS p_Axis ) const
		{
			MouseAxisMap::const_iterator MouseActions =
				m_MouseAxisToAction.begin( );
			ZED_UINT32 ActionCount = 0;

			while( MouseActions != m_MouseAxisToAction.end( ) )
			{
				if( MouseActions->first == p_Axis )
				{
					++ActionCount;
				}
				++MouseActions;
			}

			return ActionCount;
		}

		void InputBinder::PrintBoundMouseAxes( ) const
		{
			MouseAxisMap::const_iterator MouseAxes =
				m_MouseAxisToAction.begin( );

			zedTrace( "[ZED::Utility::InputBinder::PrintBoundMouseAxes] <INFO>"
				" Bound mouse axes:\n" );

			while( MouseAxes != m_MouseAxisToAction.end( ) )
			{
				zedTrace( "\t%s = %d\n",
					ZED::System::MouseAxisToString( MouseAxes->first ),
					MouseAxes->second );

				++MouseAxes;
			}

			zedTrace( "\n" );
		}
	}
}

