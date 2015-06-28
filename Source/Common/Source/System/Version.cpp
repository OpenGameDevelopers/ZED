#include <System/Version.hpp>
#include <System/GitVersion.hpp>
#include <System/Debugger.hpp>

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
	}
}

