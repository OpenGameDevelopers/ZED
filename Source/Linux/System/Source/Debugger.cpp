#include <Debugger.hpp>
#include <stdarg.h>
#include <stdio.h>

namespace ZED
{
	namespace System
	{
		ZED_INT32 Trace( const char *p_pMessage, ... )
		{
			char CompleteMessage[ 128 ];
			ZED_INT32 ReturnVal;

			va_list ArgPtr;

			va_start( ArgPtr, p_pMessage );
			ReturnVal = vsnprintf( CompleteMessage, 128, p_pMessage, ArgPtr );
			va_end( ArgPtr );
			
			printf( CompleteMessage );

			return ReturnVal;
		}
	}
}

