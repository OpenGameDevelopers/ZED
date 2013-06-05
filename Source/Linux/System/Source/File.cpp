#include <System/File.hpp>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <Debugger.hpp>

namespace ZED
{
	namespace System
	{
		// http://www.flipcode.com/archives/Path_To_Executable_On_Linux.shtml
		ZED_UINT32 GetExecutablePath( char **p_ppBuffer, ZED_MEMSIZE p_Size )
		{
			// /proc/[pid]/exe
			char LinkName[ 64 ];
			pid_t PID;
			int Ret;

			PID = getpid( );

			// Fins the name of the link in /proc
			if( snprintf( LinkName, sizeof( LinkName ),
				"/proc/%i/exe", PID ) < 0 )
			{
				return ZED_FAIL;
			}

			char FullPath[ p_Size ];
			Ret = readlink( LinkName, FullPath, p_Size );

			if( Ret == -1 )
			{
				zedTrace( "[ZED::Systme::GetExecutablePath] <ERROR> "
					"Failed to read symbolic link for: %s\n", LinkName );
				zedAssert( ZED_FALSE );
				return ZED_FAIL;
			}

			// The buffer probably wasn't large enough
			if( Ret >= p_Size )
			{
				errno = ERANGE;
				zedTrace( "[ZED::System::GetExecutablePath] <ERROR> "
					"Buffer size insufficient\n" );
				zedAssert( ZED_FALSE );
				return ZED_FAIL;
			}

			FullPath[ Ret ] = 0;

			std::string ExePath( FullPath );
			ZED_MEMSIZE SlashLoc = ExePath.find_last_of( "/" );
			ExePath.resize( SlashLoc );
			ExePath.append( "/" );

			strncpy( *p_ppBuffer, ExePath.c_str( ), ExePath.size( ) );

			return ZED_OK;
		}
	}
}

