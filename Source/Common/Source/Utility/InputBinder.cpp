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
			KeyMapInsertResult Insert = m_KeyToAction.insert(
				KeyMapEntry( p_Key, p_Action  ) );
			
			if( Insert.second == false )
			{
				zedTrace( "[ZED::Utility::InputBinder::BindKey] <ERROR> "
					"Failed to insert key+value pair into the map\n" );

				return ZED_FAIL;
			}

			if( Insert.first == m_KeyToAction.end( ) )
			{
				zedTrace( "[ZED::Utility::InputBinder::BindKey] <ERROR> "
					"Failed to insert key+value pair into the map. At the end "
					"of the map\n" );
				
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
					"<ERROR> Failed to find key %d\n", p_Key );
				return 0;
			}

			return KeyAction->second;
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
	}
}

