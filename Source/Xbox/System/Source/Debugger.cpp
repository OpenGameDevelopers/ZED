#include <xtl.h>
#include <Debugger.hpp>

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
			Ret = _vsnprintf( CompleteMessage, sizeof( CompleteMessage ),
				p_pMessage, ArgPtr );
			va_end( ArgPtr );

			OutputDebugStringA( CompleteMessage );

			return Ret;
		}

		ZED_INT32 Debugger::Trace( ZED_UINT32 p_Level, const char *p_pMessage,
			... )
		{
			char CompleteMessage[ 128 ];
			ZED_INT32 Ret = 0;

			// If the tracelevel matches the current kind of trace level
			// wanted, go ahead and output it
			if( m_TraceLevel & p_Level )
			{
				va_list ArgPtr;

				va_start( ArgPtr, p_pMessage );
				Ret = _vsnprintf( CompleteMessage, sizeof( CompleteMessage ),
					p_pMessage, ArgPtr );
				va_end( ArgPtr );

				OutputDebugStringA( CompleteMessage );
			}

			return Ret;
		}
	}
}
