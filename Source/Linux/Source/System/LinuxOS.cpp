#include <System/OS.hpp>
#include <sys/utsname.h>
#include <cstring>
#include <System/Debugger.hpp>

namespace ZED
{
	namespace System
	{
		ZED_UINT32 GetOSFullName( ZED_CHAR8 **p_ppName )
		{
			struct utsname Name;
			int Error = uname( &Name );

			if( Error == -1 )
			{
				switch( errno )
				{
					case EFAULT:
					{
						zedTrace( "[ZED::System::GetOSFullName] <ERROR> "
							"Failed to call uname, the buffer used was not "
							"valid\n" );
						break;
					}
					default:
					{
						zedTrace( "[ZED::System::GetOSFullName] <ERROR> "
							"Unknown error calling uname\n" );
						break;
					}
				}

				return ZED_FAIL;
			}

			ZED_MEMSIZE BufferSize = 0;

			BufferSize += strlen( Name.sysname );
			++BufferSize;
			BufferSize += strlen( Name.machine );
			++BufferSize;
			BufferSize += strlen( Name.release );
			++BufferSize;
			BufferSize += strlen( Name.version );

			( *p_ppName ) = new ZED_CHAR8[ BufferSize + 1 ];

			memset( ( *p_ppName ), '\0', BufferSize + 1 );

			strncat( ( *p_ppName ), Name.sysname, strlen( Name.sysname ) );
			strncat( ( *p_ppName ), " ", 1 );
			strncat( ( *p_ppName ), Name.machine, strlen( Name.machine ) );
			strncat( ( *p_ppName ), " ", 1 );
			strncat( ( *p_ppName ), Name.release, strlen( Name.release ) );
			strncat( ( *p_ppName ), " ", 1 );
			strncat( ( *p_ppName ), Name.version, strlen( Name.version ) );

			return ZED_OK;
		}
	}
}

