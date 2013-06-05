#ifndef __ZED_SYSTEM_FILE_HPP__
#define __ZED_SYSTEM_FILE_HPP__

#include <DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		// The first parameter contains the absolute path to the executable
		// if this function succeeds
		ZED_UINT32 GetExecutablePath( char **p_ppBuffer, ZED_MEMSIZE p_Size );
	}
}

#endif

