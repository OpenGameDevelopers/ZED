#include <System/Time.hpp>
#include <System/Debugger.hpp>
#include <unistd.h>
#include <time.h>
#include <cmath>

namespace ZED
{
	namespace System
	{
		ZED_BOOL StartTime( )
		{
			static ZED_UINT64 TimeStart = 0ULL;
			struct timespec Spec;

			clock_gettime( CLOCK_REALTIME, &Spec );
			ZED_UINT64 Sec, MSec;
			Sec = Spec.tv_sec*1000000ULL;
			MSec = Spec.tv_nsec;

			if( TimeStart == 0ULL )
			{
				TimeStart = Sec+MSec;
				return 0ULL;
			}

			return ZED_TRUE;
		}

		ZED_UINT64 GetTimeS( )
		{
			struct timespec Spec;

			clock_gettime( CLOCK_REALTIME, &Spec );
			ZED_UINT64 Sec;
			Sec = Spec.tv_sec;

			return Sec;
		}

		ZED_UINT64 GetTimeMS( )
		{
			static ZED_UINT64 TimeStart = 0ULL;
			struct timespec Spec;

			clock_gettime( CLOCK_REALTIME, &Spec );
			ZED_UINT64 Sec, MSec;
			Sec = Spec.tv_sec*1000ULL;
			MSec = Spec.tv_nsec;

			return Sec+( ( MSec / 1000000ULL ) );
		}

		ZED_UINT64 GetTimeMiS( )
		{
			static ZED_UINT64 TimeStart = 0ULL;
			struct timespec Spec;

			clock_gettime( CLOCK_REALTIME, &Spec );
			ZED_UINT64 Sec, MiSec;
			Sec = Spec.tv_sec*1000000ULL;
			MiSec = Spec.tv_nsec;

			return Sec+( ( MiSec / 1000ULL ) );
		}

		ZED_UINT64 GetTimeNS( )
		{
			static ZED_UINT64 TimeStart = 0ULL;
			struct timespec Spec;

			clock_gettime( CLOCK_REALTIME, &Spec );
			ZED_UINT64 Sec, NSec;
			Sec = Spec.tv_sec*1000000000ULL;
			NSec = Spec.tv_nsec;
	
			return ( Sec + NSec );
		}

		ZED_UINT32 GetBeatTime( BEAT_TIME &p_BeatTime )
		{
			time_t Current;
			Current = time( NULL );

			struct tm *pTime;
			pTime = gmtime( &Current );

			ZED_FLOAT32 Hour = static_cast< ZED_FLOAT32>( pTime->tm_hour );
			ZED_FLOAT32 Minute = static_cast< ZED_FLOAT32 >( pTime->tm_min );
			ZED_FLOAT32 Second = static_cast< ZED_FLOAT32 >( pTime->tm_sec );
			ZED_FLOAT32 BeatTime = ( Second + ( Minute * 60.0f ) +
				( (Hour + 1.0f ) * 3600.0f ) ) / 86.4f;

			p_BeatTime.Beat = BeatTime;
			p_BeatTime.CentiBeat = fmod( BeatTime, 1.0f ) * 100.0f;
			
			return ZED_OK;
		}
	}
}

