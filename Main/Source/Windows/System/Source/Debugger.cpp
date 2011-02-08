#include <Debugger.hpp>
#include <Windows.h>

namespace ZED
{
	namespace System
	{
		void Trace( const char *p_pMessage, ... )
		{
			char CompleteMessage[ 128 ];

			va_list ArgPtr;

			va_start( ArgPtr, p_pMessage );
			vsnprintf( CompleteMessage, 128, p_pMessage, ArgPtr );
			va_end( ArgPtr );

			OutputDebugStringA( CompleteMessage );
		}

		void Debugger::Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... )
		{
			// To make things easy (and save a bunch of ifs), bitwise and
			// will make light work of that
			if( m_TraceLevel & p_Level )
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
}
