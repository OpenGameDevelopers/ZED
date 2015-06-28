#ifndef __ZED_SYSTEM_VERSION_HPP__
#define __ZED_SYSTEM_VERSION_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		typedef struct __zedVERSION_INFORMATION
		{
			ZED_CHAR8	*pTag;
			ZED_CHAR8	*pBranch;
			ZED_CHAR8	*pCommitHash;
			ZED_CHAR8	*pCommitHashShort;
			ZED_CHAR8	*pCommitDate;
			ZED_UINT64	RollingCount;
			ZED_UINT32	Major;
			ZED_UINT32	Minor;
			ZED_UINT32	Revision;
			ZED_UINT32	Build;
		}VERSION_INFORMATION;

		ZED_UINT32 PrintVersionInformation( );

		ZED_UINT32 GetVersionInformation(
			VERSION_INFORMATION &p_VersionInformation );
	}
}

#endif
