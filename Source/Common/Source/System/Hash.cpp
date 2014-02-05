#include <System/Hash.hpp>
#include <cstring>

namespace ZED
{
	namespace System
	{
		ZED_UINT32 HashString( const ZED_CHAR8 *p_pString )
		{
			if( p_pString == ZED_NULL )
			{
				return 0;
			}

			const ZED_UINT32 Base = 65521;
			const ZED_UINT32 Max = 5552;

			ZED_UINT32 String1 = 0, String2 = 0;

#define DO1( Buffer, i ) { String1 += tolower( Buffer[ i ] );\
	String2 += String1; }
#define DO2( Buffer, i ) DO1( Buffer, i ); DO1( Buffer, i+1 );
#define DO4( Buffer, i ) DO2( Buffer, i ); DO2( Buffer, i+2 );
#define DO8( Buffer, i ) DO4( Buffer, i ); DO4( Buffer, i+4 );
#define DO16( Buffer ) DO8( Buffer, 0 ); DO8( Buffer, 0 );
			
			ZED_MEMSIZE StringLength = strlen( p_pString );

			for( ; StringLength > 0; )
			{
				ZED_UINT32 K = StringLength < Max ? StringLength : Max;

				StringLength -= K;

				while( K >= 16 )
				{
					DO16( p_pString );
					p_pString += 16;
					K -= 16;
				}

				if( K != 0 )
				{
					do
					{
						String1 += *p_pString++;
						String2 += String1;
					}while( --K );
				}

				String1 %= Base;
				String2 %= Base;
			}

			return ( ( String2 << 16 ) | String1 );

#undef DO16
#undef DO8
#undef DO4
#undef DO2
#undef DO1
		}
	}
}

