#include <Debugger.hpp>
#include <Windows.h>

namespace ZED
{
	namespace System
	{
		ZED_INT32 Trace( const char *p_pMessage, ... )
		{
			char CompleteMessage[ 128 ];
			ZED_INT32 Ret;

			va_list ArgPtr;

			va_start( ArgPtr, p_pMessage );
			Ret = vsnprintf( CompleteMessage, 128, p_pMessage, ArgPtr );
			va_end( ArgPtr );

			OutputDebugStringA( CompleteMessage );
			
			return Ret;
		}

		ZED_INT32 Debugger::Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... )
		{
			ZED_UINT32 Ret = 0;
			// To make things easy (and save a bunch of ifs), bitwise and
			// will make light work of that
			if( m_TraceLevel & p_Level )
			{
				// Too much, or not enough?  Should this be user-defined?
				char CompleteMessage[ 128 ];

				va_list ArgPtr;

				va_start( ArgPtr, p_pMessage );
				Ret = vsnprintf( CompleteMessage, 128, p_pMessage, ArgPtr );
				va_end( ArgPtr );

				OutputDebugStringA( CompleteMessage );
			}
			return Ret;
		}
	}
}
