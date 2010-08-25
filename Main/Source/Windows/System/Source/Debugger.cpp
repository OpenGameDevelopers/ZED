#include <Debugger.hpp>
#include <Windows.h>

namespace ZED
{
	namespace System
	{
		void Trace( const char *p_pMessage )
		{
			// Yeah, this is pretty weak.  I'm working on it.
			OutputDebugStringA( p_pMessage );
		}

		void Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... )
		{
			// Too much, or not enough?  Should this be user-defined?
			char CompleteMessage[ 128 ];

			va_list ArgPtr;

			va_start( ArgPtr, p_pMessage );
			vsnprintf( CompleteMessage, 128, p_pMessage, ArgPtr );
			va_end( ArgPtr );

			OutputDebugStringA( CompleteMessage );
		}
	}
}
