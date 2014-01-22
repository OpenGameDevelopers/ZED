#include <System/File.hpp>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <System/Debugger.hpp>
#include <sys/types.h>
#include <sys/stat.h>

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

			// Find the name of the link in /proc
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

			( *p_ppBuffer) = new char [ ExePath.size( )+1 ];
			strncpy( *p_ppBuffer, ExePath.c_str( ), ExePath.size( ) );
			( *p_ppBuffer )[ ExePath.size( ) ] = '\0';

			return ZED_OK;
		}

		ZED_BOOL FileExists( const ZED_CHAR8 *p_pFileName,
			const ZED_BOOL p_IncludeSymLinks )
		{
			struct stat FileStat;

			if( lstat( p_pFileName, &FileStat ) < 0 )
			{
				switch( errno )
				{
					case EACCES:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"Permission denied for %s\n", p_pFileName );
						break;
					}
					case EBADF:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"Bad file descriptor for %s\n", p_pFileName );
						break;
					}
					case EFAULT:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"Bad address for %s\n", p_pFileName );
						break;
					}
					case ELOOP:
					{
						zedTrace( "[ZED::Systme::FileExists] <ERROR> "
							"Too many symbolic links for %s\n", p_pFileName );
						break;
					}
					case ENAMETOOLONG:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"Path name too long for %s [%d characters]\n",
							p_pFileName, strlen( p_pFileName ) );
						break;
					}
					case ENOENT:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"File is not valid: %s", p_pFileName );
						break;
					}
					case ENOMEM:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"Ran out of memory for %s\n", p_pFileName );
						break;
					}
					case EOVERFLOW:
					{
						zedTrace( "[ZED::System::FileExists] <ERROR> "
							"Overflow occurred (maybe file is too large?) for "
							"%s\n", p_pFileName );
						break;
					}
					default:
					{
						break;
					}
				}
				return ZED_FALSE;
			}

			if( S_ISREG( FileStat.st_mode ) )
			{
				return ZED_TRUE;
			}

			if( p_IncludeSymLinks == ZED_TRUE )
			{
				if( S_ISLNK( FileStat.st_mode ) )
				{
					return ZED_TRUE;
				}
			}

			zedTrace( "[ZED::System::FileExits] <ERROR> File found but "
				"is of type " );

			if( S_ISLNK( FileStat.st_mode ) )
			{
				zedTrace( "symbolic link (function did not request to include "
					"symbolic links)" );
			}
			else if( S_ISDIR( FileStat.st_mode ) )
			{
				zedTrace( "directory (use ZED::System::DirectoryExists for "
					"directories)" );
			}
			else if( S_ISCHR( FileStat.st_mode ) )
			{
				zedTrace( "character special file" );
			}
			else if( S_ISBLK( FileStat.st_mode ) )
			{
				zedTrace( "block special file" );
			}
			else if( S_ISFIFO( FileStat.st_mode ) )
			{
				zedTrace( "pipe or FIFO" );
			}
			else if( S_ISSOCK( FileStat.st_mode ) )
			{
				zedTrace( "socket" );
			}
			else
			{
				zedTrace( "unknown (not supported file type)" );
			}

			zedTrace( "\n" );

			return ZED_FALSE;
		}

		File::~File( )
		{
		}
	}
}

