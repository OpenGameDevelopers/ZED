#ifndef __ZED_SYSTEM_TIME_HPP__
#define __ZED_SYSTEM_TIME_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_UINT64 ZED_INFINITE_TIME = 0xFFFFFFFFFFFFFFFF;

		typedef struct __BEAT_TIME
		{
			ZED_UINT32	Beat;
			ZED_UINT32	CentiBeat;
		}BEAT_TIME,*LPBEAT_TIME;

		ZED_BOOL StartTime( );
		// Seconds
		ZED_UINT64 GetTimeS( );
		// Milliseconds
		ZED_UINT64 GetTimeMS( );
		// Microseconds
		ZED_UINT64 GetTimeMiS( );
		// Nanoseconds
		ZED_UINT64 GetTimeNS( );

		ZED_UINT32 GetBeatTime( BEAT_TIME &p_BeatTime );
	}
}

#endif

