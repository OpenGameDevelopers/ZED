#include <Utility/InputBinder.hpp>
#include <System/Debugger.hpp>

namespace ZED
{
	namespace Utility
	{
		InputBinder::InputBinder( )
		{
		}

		InputBinder::~InputBinder( )
		{
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
	}
}

