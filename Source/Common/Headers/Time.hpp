#ifndef __ZED_SYSTEM_TIME_HPP__
#define __ZED_SYSTEM_TIME_HPP__

#include <DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_UINT64 ZED_INFINITE_TIME = 0xFFFFFFFFFFFFFFFF;

		ZED_BOOL StartTime( );
		// Seconds
		ZED_UINT64 GetTimeS( );
		// Milliseconds
		ZED_UINT64 GetTimeMS( );
		// Microseconds
		ZED_UINT64 GetTimeMiS( );
		// Nanoseconds
		ZED_UINT64 GetTimeNS( );
	}
}

#endif

