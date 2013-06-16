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

			m_pQueue = new EventQueue[ m_BufferCount ];
		}

		EventRouter::~EventRouter( )
		{
			if( g_pEventRouter == this )
			{
				g_pEventRouter = ZED_NULL;
			}

			if( m_pQueue )
			{
				delete [ ] m_pQueue;
				m_pQueue = ZED_NULL;
			}
		}

		ZED_BOOL EventRouter::Add( EventListener * const &p_pListener,
			const EventType &p_Type )
		{
			ZED_UINT32 Error = 0;
			if( this->ValidateType( p_Type, &Error ) != ZED_TRUE )
			{
				zedTrace( "[ZED::System::EventRouter::Add] <ERROR> Failed "
					"to verify event type \"%s\" [ %d ]\n", p_Type.Name( ),
					p_Type.ID( ) );
				return ZED_FALSE;
			}

			EventTypeSet::iterator TypeItr;

			// If the event type was already in the set, continue
			if( Error != ZED_EVENTTYPE_ALREADYINSET )
			{
				EventTypeSetInsRes InsRes = m_Types.insert( p_Type );

				if( InsRes.second == ZED_FALSE )
				{
					zedTrace( "[ZED::System::EventRouter::Add] <ERROR> "
						"An event type already exists of this type\n"
						"\t%s [ %d ]\n" );
					return ZED_FAIL;
				}
				
				if( InsRes.first == m_Types.end( ) )
				{
					zedTrace( "[ZED::System::EventRouter::Add] <ERROR> "
						"Failed to insert type \"%s\" [ %d ] into the "
						"type set\n", p_Type.Name( ), p_Type.ID( ) );
					return ZED_FAIL;
				}

				TypeItr = InsRes.first;
			}

			// Find the listener map entry
			EventListenerTypeMap::iterator ELTMItr =
				m_Registry.find( p_Type.ID( ) );
			
			// Could not find it, so create it
			if( ELTMItr == m_Registry.end( ) )
			{
				EventListenerTypeMapInsRes ELTMInsRes = m_Registry.insert(
					EventListenerTypeMapEntry( p_Type.ID( ),
					EventListenerList( ) ) );

				if( ELTMInsRes.second == ZED_FALSE )
				{
					zedTrace( "[ZED::System::EventRouter::Add] <ERROR> "
						"A mapping between type \"%s\" [ %d ] and Listener "
						"\"%s\" already exists\n", p_Type.Name( ),
						p_Type.ID( ), p_pListener->Name( ) );

					return ZED_FALSE;
				}

				if( ELTMInsRes.first == m_Registry.end( ) )
				{
					zedTrace( "[ZED::System::EventRouter::Add] <ERROR> "
						"Failed to create a mapping between event type "
						"\"%s\" [ %d ] and event listener [ %s ]\n",
						p_Type.Name( ), p_Type.ID( ), p_pListener->Name( ) );
					return ZED_FALSE;
				}

				ELTMItr = ELTMInsRes.first;
			}

			// Replace the event listener discovered
			EventListenerList &ELList = ( *ELTMItr ).second;

			// First, make sure that this is a new event listener being added
			for( EventListenerList::iterator Itr = ELList.begin( ),
				ItrEnd = ELList.end( ); Itr != ItrEnd; ++Itr )
			{
				ZED_BOOL Match = ( *Itr == p_pListener );

				if( Match )
				{
					return ZED_FAIL;
				}
			}

			// This must be a unique event listener
			ELList.push_back( p_pListener );

			return ZED_TRUE;
		}

		ZED_BOOL EventRouter::Remove( EventListener * const &p_pListener )
		{
			ZED_BOOL Return = ZED_FALSE;

			for( EventListenerTypeMap::iterator Itr = m_Registry.begin( ),
				ItrEnd = m_Registry.end( ); Itr != ItrEnd; ++Itr )
			{
				EventListenerList &ELList = Itr->second;

				for( EventListenerList::iterator Itr2 = ELList.begin( ),
					ItrEnd2 = ELList.end( ); Itr2 != ItrEnd2; ++Itr2 )
				{
					if( *Itr2 == p_pListener )
					{
						ELList.erase( Itr2 );
						Return = ZED_TRUE;
						break;
					}
				}
			}

			return Return;
		}

		ZED_BOOL EventRouter::ValidateType( const EventType &p_Type,
			ZED_UINT32 *p_pError ) const
		{
			// Does this event already exist and is it a duplicate?
			// try to find the type in the set
			// if not at the end, compare against the one found and make sure
			// the strings are the same
			if( p_Type.Name( ) == ZED_NULL )
			{
				( *p_pError ) = ZED_EVENTTYPE_INVALIDNAME;
				return ZED_FALSE;
			}

			if( ( p_Type.ID( ) == 0 ) &&
				( strcmp( p_Type.Name( ), kEventWildCard ) != 0 ) )
			{
				( *p_pError ) = ZED_EVENTTYPE_WILDCARDMISMATCH;
				return ZED_FALSE;
			}

			EventTypeSet::const_iterator Itr = m_Types.find( p_Type );

			if( Itr != m_Types.end( ) )
			{
				EventType OtherEvent = *Itr;

				int Comparison = strcmp( p_Type.Name( ), OtherEvent.Name( ) );

				if( Comparison != 0 )
				{
					( *p_pError ) = ZED_EVENTTYPE_NAMECOLLISION;
					return ZED_FALSE;
				}
			}
			else
			{
				( *p_pError ) = ZED_EVENTTYPE_ALREADYINSET;
			}

			return ZED_TRUE;
		}
	}
}

