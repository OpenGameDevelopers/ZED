#ifndef __ZED_SYSTEM_EVENT_HPP__
#define __ZED_SYSTEM_EVENT_HPP__

namespace ZED
{
	namespace System
	{
		const ZED_CHAR8 * const EventWildCard = "*";

		class EventType
		{
		public:
			ZED_EXPLICIT EventType( const ZED_CHAR8 * const p_pName ) :
				m_ID( HashEventName( p_pName ) ),
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
			ZED_CHAR8	*m_pName;
			ZED_UINT32	m_ID;
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
				ZED_UINT64 p_DispatchTime, //= GetTime
				EventData *p_pData = ZED_NULL ) :
				m_Type( p_pName ),
				m_DispatchTime( p_DispatchTime ),
				m_pData( p_pData )
			{ }
			virtual ~Event( ){ }

			ZED_UINT64 DispatchTime( ) { return m_DispatchTime; }

			template < typename T > T* Data( ) const
				{ return reinterpret_cast< T* >( m_pData ); }
		private:
			EventData	*m_pData;
			EventType	m_Type;
			ZED_UINT64	m_DispatchTime;
		};


		ZED_UINT32 EventType::HashName( const ZED_CHAR8 *p_pName )
		{
			if( p_pString == ZED_NULL )
			{
				return 0;
			}

			if( strcmp( p_pName, ZED::System::EventWildCard ) == 0 )
			{
				return 0;
			}

			const ZED_UINT32 Base = 65521;
			const ZED_UINT32 Max = 5552;

			ZED_UINT32 Str1 = 0, Str2 = 0;

#define DO1( Buff, i ) { Str1 += tolower( Buff[ i ] ); Str2 += Str1; }
#define DO2( Buff, i ) DO1( Buff, i ); DO1( Buff, i+1 );
#define DO4( Buff, i ) DO2( Buff, i ); DO2( Buff, i+2 );
#define DO8( Buff, i ) DO4( Buff, i ); DO4( Buff, i+4 );
#define DO16( Buff, i ) DO8( Buff, i ); DO8( Buff, 0 );
			
			for( ZED_MEMSIZE StrLen = strlen( p_pName ); StrLen > 0; )
			{
				ZED_UINT32 K = StrLen < Max ? StrLen : Max;

				StrLen -= K;

				while( K >= 16 )
				{
					DO16( p_pName );
					p_pName += 16;
					K -= 16;
				}

				if( K != 0 )
				{
					do
					{
						Str1 += *p_pString++;
						Str2 += Str1;
					}while( --K );
				}

				Str1 % Base;
				Str2 % Base;
			}

			return ( ( Str2 << 16 ) | Str1 );
			
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
		}
	}
}

#endif

