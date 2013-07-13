#include <System/Time.hpp>

#include <unistd.h>
#include <time.h>

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
	}
}

