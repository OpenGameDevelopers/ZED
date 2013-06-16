#ifndef __ZED_SYSTEM_EVENTROUTER_HPP__
#define __ZED_SYSTEM_EVENTROUTER_HPP__

#include <System/Event.hpp>
#include <System/EventListener.hpp>
#include <DataTypes.hpp>
#include <Debugger.hpp>
#include <set>
#include <queue>
#include <map>
#include <list>

namespace ZED
{
	namespace System
	{
		class EventRouter;

		// Helper typedefs
		typedef std::set< EventType > EventTypeSet;
		typedef std::pair< EventTypeSet::iterator, ZED_BOOL > EventTypeSetInsRes;
		typedef std::priority_queue< Event * > EventQueue;
		typedef std::list< EventListener * > EventListenerList;
		typedef std::map< ZED_UINT32, EventListenerList > EventListenerTypeMap;
		typedef std::pair< ZED_UINT32, EventListenerList >
			EventListenerTypeMapEntry;
		typedef std::pair< EventListenerTypeMap::iterator, ZED_BOOL >
			EventListenerTypeMapInsRes;

		EventRouter *g_pEventRouter;
		const ZED_UINT64 kInfiniteTime = 0xFFFFFFFFFFFFFFFF;

		class EventRouter
		{
		public:
			ZED_EXPLICIT EventRouter( const ZED_CHAR8 * const p_pName,
				const ZED_BOOL p_Global, const ZED_UINT32 p_BufferCount );

			~EventRouter( );

			// Associate an event type with an event listener
			ZED_BOOL Add( EventListener * const &p_Listener,
				const EventType &p_Type );

			// Disassociate an event type from an event listener
			ZED_BOOL Remove( const EventListener &p_Listener,
				const EventType &p_Type );

			// Broadcast an event
			ZED_BOOL Send( const Event &p_Event ); 

			// Add a delay to an event to be dispatched at a later date
			ZED_BOOL Queue( const Event &p_Event,
				const ZED_UINT64 p_DeliveryTime = 0UL );

			// Remove the last event of the event type, optionally removing all
			// of them
			ZED_BOOL Abort( const EventType &p_Type,
				const ZED_BOOL p_All = ZED_FALSE );

			// Process events given an amount of time to process them
			ZED_BOOL Process(
				const ZED_UINT64 p_MaxMicroSeconds = kInfiniteTime );

			// Check if an event type has not lost its integrity
			ZED_BOOL ValidateType( const EventType &p_Type,
				ZED_UINT32 *p_pError ) const;

		private:
			static EventRouter *Get( ) { return g_pEventRouter; }

			friend ZED_BOOL AddEventListener( EventListener * const &p_Listener,
				const EventType &p_Type );
			friend ZED_BOOL RemoveEventListener(
				const EventListener &p_Listener, const EventType &p_Type );
			friend ZED_BOOL SendEvent( const Event &p_Event );
			friend ZED_BOOL QueueEvent( const Event &p_Event,
				const ZED_UINT64 p_DeliveryTime = 0UL );
			friend ZED_BOOL AbortEvent( const EventType &p_Event,
				const ZED_BOOL p_All = ZED_FALSE );
			friend ZED_BOOL ProcessEvents(
				const ZED_UINT64 p_MaxMicroSeconds = kInfiniteTime );
			friend ZED_BOOL ValidateEventType( const EventType &p_Type,
				ZED_UINT32 *p_pError );

			EventTypeSet			m_Types;
			EventQueue				*m_pQueue;
			EventListenerTypeMap	m_Registry;

			ZED_UINT32	m_BufferCount;
			ZED_UINT32	m_ActiveBuffer;
		};

		ZED_BOOL AddEventListener( EventListener * const &p_Listener,
			const EventType &p_Type )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Add( p_Listener, p_Type );
		}

		ZED_BOOL RemoveEventListener( const EventListener &p_Listener,
			const EventType &p_Type )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Remove( p_Listener, p_Type );
		}

		ZED_BOOL SendEvent( const Event &p_Event )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Send( p_Event );
		}

		ZED_BOOL QueueEvent( const Event &p_Event,
			const ZED_UINT64 p_DeliveryTime )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Queue( p_Event, p_DeliveryTime );
		}

		ZED_BOOL AbortEvent( const EventType &p_Event, const ZED_BOOL p_All )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Abort( p_Event, p_All );
		}

		ZED_BOOL ProcessEvents( const ZED_UINT64 p_MaxMicroSeconds )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Process( p_MaxMicroSeconds );
		}

		ZED_BOOL ValidateEventType( const EventType &p_Type,
			ZED_UINT32 *p_pError )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->ValidateType( p_Type, p_pError );
		}
	}
}

#endif

