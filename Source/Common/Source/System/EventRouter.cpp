#include <System/Event.hpp>
#include <System/EventRouter.hpp>
#include <cstring>

namespace ZED
{
	namespace System
	{
		EventRouter::EventRouter( const ZED_CHAR8 *p_pName,
			const ZED_BOOL p_Global, const ZED_UINT32 p_BufferCount )
		{
			if( p_Global )
			{
				g_pEventRouter = this;
			}

			if( p_BufferCount > 2 )
			{
				m_BufferCount = p_BufferCount;
			}
			else
			{
				m_BufferCount = 2;
			}

			m_pEventQueue = new EventQueue[ m_BufferCount ];
		}

		EventRouter::~EventRouter( )
		{
			if( g_pEventRouter == this )
			{
				g_pEventRouter = ZED_NULL;
			}

			if( m_pEventQueue )
			{
				delete [ ] m_pEventQueue;
				m_pEventQueue = ZED_NULL;
			}
		}

		ZED_BOOL EventRouter::Add( const EventListener &p_Listener,
			const EventType &p_Type )
		{
			return ZED_TRUE;
		}

		ZED_BOOL EventRouter::ValidateType( const EventType &p_Type ) const
		{
			// Does this event already exist and is it a duplicate?
			// try to find the type in the set
			// if not at the end, compare against the one found and make sure the strings are the same
			if( p_Type.Name( ) == ZED_NULL )
			{
				return ZED_FALSE;
			}

			if( ( p_Type.ID( ) == 0 ) &&
				( strcmp( p_Type.Name( ), kEventWildCard ) != 0 ) )
			{
				return ZED_FALSE;
			}

			EventTypeSet::const_iterator Itr = m_Types.find( p_Type );

			if( Itr != m_Types.end( ) )
			{
				EventType OtherEvent = *Itr;

				int Comparison = strcmp( p_Type.Name( ), OtherEvent.Name( ) );

				if( Comparison != 0 )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}
	}
}

