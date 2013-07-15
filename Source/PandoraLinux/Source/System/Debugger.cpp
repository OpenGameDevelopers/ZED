#include <System/Debugger.hpp>
#include <stdarg.h>
#include <stdio.h>

namespace ZED
{
	namespace System
	{
		ZED_SINT32 Trace( const char *p_pMessage, ... )
		{
			ZED_MEMSIZE StrLen = 0;
			ZED_SINT32 ReturnVal;
			va_list ArgPtr;

			va_start( ArgPtr, p_pMessage );
			StrLen = vsnprintf( ZED_NULL, 0, p_pMessage, ArgPtr );
			va_end( ArgPtr );

			++StrLen;
			char CompleteMessage[ StrLen ];

			va_start( ArgPtr, p_pMessage );
			ReturnVal = vsnprintf( CompleteMessage, StrLen, p_pMessage,
				ArgPtr );
			va_end( ArgPtr );

			CompleteMessage[ StrLen-1 ] = '\0';

			printf( CompleteMessage );

			return ReturnVal;
		}
	}
}

