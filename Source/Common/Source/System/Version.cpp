#include <System/Version.hpp>
#include <System/GitVersion.hpp>
#include <System/Debugger.hpp>
#include <System/Memory.hpp>
#include <cstring>

namespace ZED
{
	namespace System
	{
		ZED_UINT32 PrintVersionInformation( )
		{
			zedTrace( "ZED Version Information\n" );
			zedTrace( "-----------------------\n" );
			zedTrace( "\tHash:        %s\n", ZED_VERSION_COMMITHASH );
			zedTrace( "\tBuild:       %s\n", ZED_VERSION_BUILD_STRING );
			zedTrace( "\tCommit date: %s\n", ZED_VERSION_COMMITTERDATE );
			zedTrace( "\tBranch:      %s\n", ZED_VERSION_BRANCH );
			zedTrace( "\tTag:         %s\n", ZED_VERSION_TAG );

			return ZED_OK;
		}

		ZED_UINT32 GetVersionInformation(
			VERSION_INFORMATION &p_VersionInformation )
		{
			p_VersionInformation.RollingCount = ZED_VERSION_ROLLINGCOUNT;
			p_VersionInformation.Major = ZED_VERSION_MAJOR;
			p_VersionInformation.Minor = ZED_VERSION_MINOR;
			p_VersionInformation.Revision = ZED_VERSION_REVISION;
			p_VersionInformation.Build = ZED_VERSION_BUILD;

			p_VersionInformation.pTag =
				new ZED_CHAR8[ strlen( ZED_VERSION_TAG ) + 1 ];
			p_VersionInformation.pTag[ strlen( ZED_VERSION_TAG ) ] = '\0';
			strncpy( p_VersionInformation.pTag, ZED_VERSION_TAG,
				strlen( ZED_VERSION_TAG ) );

			p_VersionInformation.pBranch =
				new ZED_CHAR8[ strlen( ZED_VERSION_BRANCH ) + 1 ];
			p_VersionInformation.pBranch[ strlen( ZED_VERSION_BRANCH ) ] =
				'\0';
			strncpy( p_VersionInformation.pBranch, ZED_VERSION_BRANCH,
				strlen( ZED_VERSION_BRANCH ) );

			p_VersionInformation.pCommitHash =
				new ZED_CHAR8[ strlen( ZED_VERSION_COMMITHASH ) + 1 ];
			p_VersionInformation.pCommitHash[
				strlen( ZED_VERSION_COMMITHASH ) ] = '\0';
			strncpy( p_VersionInformation.pCommitHash, ZED_VERSION_COMMITHASH,
				strlen( ZED_VERSION_COMMITHASH ) );

			p_VersionInformation.pCommitHashShort =
				new ZED_CHAR8[ strlen( ZED_VERSION_COMMITHASHSHORT ) + 1 ];
			p_VersionInformation.pCommitHashShort[
				strlen( ZED_VERSION_COMMITHASHSHORT ) ] = '\0';
			strncpy( p_VersionInformation.pCommitHashShort,
				ZED_VERSION_COMMITHASHSHORT,
				strlen( ZED_VERSION_COMMITHASHSHORT ) );

			p_VersionInformation.pCommitDate =
				new ZED_CHAR8[ strlen( ZED_VERSION_COMMITTERDATE ) + 1 ];
			p_VersionInformation.pCommitDate[
				strlen( ZED_VERSION_COMMITTERDATE ) ] = '\0';
			strncpy( p_VersionInformation.pCommitDate,
				ZED_VERSION_COMMITTERDATE,
				strlen( ZED_VERSION_COMMITTERDATE ) );

			return ZED_OK;
		}

		void FreeVersionInformation(
			VERSION_INFORMATION &p_VersionInformation )
		{
			zedSafeDeleteArray( p_VersionInformation.pTag );
			zedSafeDeleteArray( p_VersionInformation.pBranch );
			zedSafeDeleteArray( p_VersionInformation.pCommitHash );
			zedSafeDeleteArray( p_VersionInformation.pCommitHashShort );
			zedSafeDeleteArray( p_VersionInformation.pCommitDate );
		}
	}
}

