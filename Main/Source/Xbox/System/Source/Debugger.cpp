#include <xtl.h>
#include <Debugger.hpp>

namespace ZED
{
	namespace System
	{
		void Trace( const char *p_pMessage )
		{
			OutputDebugStringA( p_pMessage ); 
		}
	}
}
