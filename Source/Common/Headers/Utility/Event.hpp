#ifndef __ZED_SYSTEM_EVENT_HPP__
#define __ZED_SYSTEM_EVENT_HPP__

#include <System/Time.hpp>
#include <cstring>
#include <cctype>

namespace ZED
{
	namespace System
	{
		const ZED_UINT32 ZED_EVENTTYPE_ALREADYINSET		= 0x00000001;
		const ZED_UINT32 ZED_EVENTTYPE_INVALIDNAME		= 0x00000002;
		const ZED_UINT32 ZED_EVENTTYPE_WILDCARDMISMATCH	= 0x00000004;
		const ZED_UINT32 ZED_EVENTTYPE_NAMECOLLISION	= 0x00000008;
		const ZED_CHAR8 * const kEventWildCard			= "*";

		class EventType
		{
		public:
			ZED_EXPLICIT EventType( const ZED_CHAR8 * const p_pName ) :
				m_ID( HashName( p_pName ) ),
				m_pName( p_pName )
			{ }
			
			ZED_UINT32 ID( ) const { return m_ID; }

			const ZED_CHAR8 * const Name( ) const { return m_pName; }

			bool operator<( const EventType &p_Other ) const
				{ return ( m_ID < p_Other.m_ID ); }

			bool operator==( const EventType &p_Other ) const
				{ return ( m_ID == p_Other.m_ID ); }

			static ZED_UINT32 HashName( const ZED_CHAR8 *p_Name );

		private:
			const ZED_CHAR8	*m_pName;
			ZED_UINT32		m_ID;
		};

		class EventData
		{
		public:
			virtual ~EventData( ) { }
		};

		class Event
		{
		public:
			ZED_EXPLICIT Event( const ZED_CHAR8 * const p_pName,
				EventData *p_pData = ZED_NULL,
				ZED_UINT64 p_DispatchTime = ZED::System::GetTimeMiS( ) ) :
				m_Type( p_pName ),
				m_DispatchTime( p_DispatchTime ),
				m_pData( p_pData )
			{ }
			virtual ~Event( ){ }

			EventType Type( ) const { return m_Type; }

			ZED_UINT64 DispatchTime( ) { return m_DispatchTime; }

			template < typename T > T* Data( ) const
				{ return reinterpret_cast< T* >( m_pData ); }

			// Required for the priority queue
			bool operator<( const Event &p_Event ) const
				{ return ( m_DispatchTime > p_Event.m_DispatchTime ); }

		private:
			EventData	*m_pData;
			EventType	m_Type;
			ZED_UINT64	m_DispatchTime;
		};

		class EventListener
		{
		public:
			virtual ~EventListener( ) { }

			virtual ZED_BOOL HandleEvent( const Event &p_Event ) = 0;

			virtual ZED_CHAR8 *Name( ) const = 0;
		};
	}
}

#endif

