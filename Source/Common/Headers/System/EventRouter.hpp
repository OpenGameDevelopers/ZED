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

		typedef std::set< EventType > EventTypeSet;
		typedef std::priority_queue< Event > EventQueue;
		typedef std::list< EventListener * > EventListenerList;
		typedef std::map< ZED_UINT32, EventListenerList > EventListenerTypeMap;

		EventRouter *g_pEventRouter;
		const ZED_UINT64 kInfiniteTime = 0xFFFFFFFFFFFFFFFF;

		class EventRouter
		{
		public:
			ZED_EXPLICIT EventRouter( const ZED_CHAR8 * const p_pName,
				const ZED_BOOL p_Global, const ZED_UINT32 p_BufferCount );

			~EventRouter( );

			ZED_BOOL Add( const EventListener &p_Listener,
				const EventType &p_Type );

			ZED_BOOL Delete( const EventListener &p_Listener,
				const EventType &p_Type );

			ZED_BOOL Trigger( const Event &p_Event ); 

			ZED_BOOL Queue( const Event &p_Event );

			ZED_BOOL Abort( const EventType &p_Type,
				const ZED_BOOL p_All = ZED_FALSE );

			ZED_BOOL Process(
				const ZED_UINT64 p_MaxMicroSeconds = kInfiniteTime );

			ZED_BOOL ValidateType( const EventType &p_Type ) const;

		private:
			static EventRouter *Get( ) { return g_pEventRouter; }

			friend ZED_BOOL AddEventListener( const EventListener &p_Listener,
				const EventType &p_Type );
			friend ZED_BOOL DeleteEventListener(
				const EventListener &p_Listener, const EventType &p_Type );
			friend ZED_BOOL TriggerEvent( const Event &p_Event );
			friend ZED_BOOL QueueEvent( const Event &p_Event );
			friend ZED_BOOL AbortEvent( const EventType &p_Event,
				const ZED_BOOL p_All = ZED_FALSE );
			friend ZED_BOOL ProcessEvents(
				const ZED_UINT64 p_MaxMicroSeconds = kInfiniteTime );
			friend ZED_BOOL ValidateEventType( const EventType &p_Type );

			EventTypeSet			m_Types;
			EventQueue				*m_pEventQueue;
			EventListenerTypeMap	m_ListenerToType;

			ZED_UINT32	m_BufferCount;
			ZED_UINT32	m_ActiveBuffer;
		};

		ZED_BOOL AddEventListener( const EventListener &p_Listener,
			const EventType &p_Type )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Add( p_Listener, p_Type );
		}

		ZED_BOOL DeleteEventListener( const EventListener &p_Listener,
			const EventType &p_Type )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Delete( p_Listener, p_Type );
		}

		ZED_BOOL TriggerEvent( const Event &p_Event )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Trigger( p_Event );
		}

		ZED_BOOL QueueEvent( const Event &p_Event )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->Queue( p_Event );
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

		ZED_BOOL ValidateEventType( const EventType &p_Type )
		{
#ifdef ZED_BUILD_DEBUG
			zedAssert( EventRouter::Get( ) );
#endif
			return EventRouter::Get( )->ValidateType( p_Type );
		}
	}
}

#endif

