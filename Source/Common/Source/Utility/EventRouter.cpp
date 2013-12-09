#include <Utility/Event.hpp>
#include <Utility/EventRouter.hpp>
#include <cstring>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Utility
	{
		EventRouter::EventRouter( const ZED_CHAR8 *p_pName,
			const ZED_BOOL p_Global, const ZED_UINT32 p_BufferCount )
		{
			if( p_Global )
			{
				g_pEventRouter = this;
				zedTrace( "[ZED::Utility::EventRouter::EventRouter] <INFO> "
					"Making \"%s\" the global event router\n", p_pName );
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
			m_Types.clear( );
			ZED_MEMSIZE NameLength = strlen( p_pName );
			m_pName = new ZED_CHAR8[ NameLength + 1 ];
			strncpy( m_pName, p_pName, NameLength );
			m_pName[ NameLength ] = '\0';
		}

		EventRouter::~EventRouter( )
		{
			if( g_pEventRouter == this )
			{
				zedTrace( "[ZED::Utility::EventRouter::~EventRouter] <INFO> "
					"Removing \"%s\" from the global event router\n",
					m_pName );

				g_pEventRouter = ZED_NULL;
			}

			zedSafeDeleteArray( m_pQueue );
			zedSafeDeleteArray( m_pName );
		}

		ZED_BOOL EventRouter::Add( EventListener * const &p_pListener,
			const EventType &p_Type )
		{
			ZED_UINT32 Error = 0;
			if( this->ValidateType( p_Type, &Error ) != ZED_TRUE )
			{
				zedTrace( "[ZED::Utility::EventRouter::Add] <ERROR> Failed "
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
					zedTrace( "[ZED::Utility::EventRouter::Add] <ERROR> "
						"An event type already exists of this type\n"
						"\t%s [ %d ]\n" );
					return ZED_FAIL;
				}
				
				if( InsRes.first == m_Types.end( ) )
				{
					zedTrace( "[ZED::Utility::EventRouter::Add] <ERROR> "
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
					zedTrace( "[ZED::Utility::EventRouter::Add] <ERROR> "
						"A mapping between type \"%s\" [ %d ] and Listener "
						"\"%s\" already exists\n", p_Type.Name( ),
						p_Type.ID( ), p_pListener->Name( ) );

					return ZED_FALSE;
				}

				if( ELTMInsRes.first == m_Registry.end( ) )
				{
					zedTrace( "[ZED::Utility::EventRouter::Add] <ERROR> "
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

		ZED_BOOL EventRouter::Send( const Event &p_Event )
		{
			ZED_UINT32 Error = 0;

			if( this->ValidateType( p_Event.Type( ), &Error ) != ZED_TRUE )
			{
				zedTrace( "[ZED::Utility::EventRouter::Send] <ERROR> "
					"Failed to verify event type \"%s\" [ %d ]\n",
					p_Event.Type( ).Name( ), p_Event.Type( ).ID( ) );
				return ZED_FALSE;
			}
			
			// Get wildcard events, first
			EventListenerTypeMap::const_iterator Itr = m_Registry.find( 0 );

			if( Itr != m_Registry.end( ) )
			{
				const EventListenerList &ELList = Itr->second;

				for( EventListenerList::const_iterator Itr2 = ELList.begin( ),
					ItrEnd2 = ELList.end( ); Itr2 != ItrEnd2; ++Itr )
				{
					( *Itr2 )->HandleEvent( p_Event );
				}
			}

			Itr = m_Registry.find( p_Event.Type( ).ID( ) );

			if( Itr == m_Registry.end( ) )
			{
				zedTrace( "[ZED::Utility::EventRouter::Send] <WARN> "
					"No event listener found for \"%s\" [ %d ]\n",
					p_Event.Type( ).Name( ), p_Event.Type( ).ID( ) );
				return ZED_FAIL;
			}

			const EventListenerList &ELList = Itr->second;

			ZED_BOOL EventProcessed = ZED_FALSE;

			for( EventListenerList::const_iterator Itr2 = ELList.begin( ),
				ItrEnd2 = ELList.end( ); Itr2 != ItrEnd2; ++Itr2 )
			{
				if( ( *Itr2 )->HandleEvent( p_Event ) == ZED_TRUE )
				{
#if defined ZED_DEBUG_EVENTMESSAGES
					zedTrace( "[ZED::Utility::EventRouter::Send] <DEBUG> "
						"Event \"%s\" [ %d ] handled by %s\n",
						p_Event.Type( ).Name( ), p_Event.Type( ).ID( ),
						( *Itr2 )->Name( ) );
#endif
					EventProcessed = ZED_TRUE;
				}
			}

			return EventProcessed;
		}


		ZED_BOOL EventRouter::Process( const ZED_UINT64 p_MaxMicroSeconds )
		{
			ZED_BOOL Return = ZED_FALSE;
/*			ZED_UINT64 TheTime = ZED::Utility::GetTimeMiS( );
			ZED_UINT64 MaxTime = ( p_MaxMicroSeconds == ZED_INFINITE_TIME ) ?
				ZED_INFINITE_TIME : ( TheTime + p_MaxMicroSeconds );
			ZED_SINT32 CurrentBuffer = m_ActiveBuffer;
			m_ActiveBuffer = ( m_ActiveBuffer + 1 ) % m_BufferCount;

			m_pQueue[ m_ActiveBuffer ].clear( );

			while( m_pQueue[ CurrentBuffer ].size( ) > 0 )
			{
				Event *pEvent = m_pQueue[ CurrentBuffer ].front( );

				if( pEvent->Time( ) + pEvent->TimeDelay( ) >
					ZED::Utility::GetTimeMiS( ) )
				{
					break;
				}

				m_pQueue[ CurrentBuffer ].pop_front( );

				const EventType &Type = pEvent->Type( );

				EventListenerTypeMap::const_iterator ListenItr =
					m_Registry.find( Type.ID( ) );

				if( ListenItr != m_Registry.end( ) )
				{
					const EventListenerList &ELList = ListenItr->second;

					for( EventListenerList::const_iterator
						Itr2 = ELList.begin( ), ItrEnd2 = ELList.end( );
						Itr2 != ItrEnd2; ++Itr2 )
					{
						( *Itr2 )->HandleEvent( *pEvent );
					}
				}
			}*/

			return Return;
		}

		ZED_BOOL EventRouter::ValidateType( const EventType &p_Type,
			ZED_UINT32 *p_pError ) const
		{
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

