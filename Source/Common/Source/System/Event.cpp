#include <System/Event.hpp>

namespace ZED
{
	namespace System
	{
		ZED_UINT32 EventType::HashName( const ZED_CHAR8 *p_pName )
		{
			if( p_pName == ZED_NULL )
			{
				return 0;
			}

			if( strcmp( p_pName, ZED::System::kEventWildCard ) == 0 )
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
#define DO16( Buff ) DO8( Buff, 0 ); DO8( Buff, 0 );
			
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
						Str1 += *p_pName++;
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

