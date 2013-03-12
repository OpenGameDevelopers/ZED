#include <Debugger.hpp>
#include <Windows.h>

namespace ZED
{
	namespace System
	{
		ZED_SINT32 Trace( const char *p_pMessage, ... )
		{
			char CompleteMessage[ 4096 ];
			ZED_SINT32 Ret;

			va_list ArgPtr;

			va_start( ArgPtr, p_pMessage );
			Ret =
#ifdef ZED_COMPILER_MSVC
				_vsnprintf( CompleteMessage, 4096, p_pMessage, ArgPtr );
#elif defined ZED_COMPILER_GCC
				vsnprintf( CompleteMessage, 4096, p_pMessage, ArgPtr );
#else
#error Unknown compiler
#endif
			va_end( ArgPtr );

			OutputDebugStringA( CompleteMessage );
			
			return Ret;
		}

		ZED_SINT32 Debugger::Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... )
		{
			ZED_SINT32 Ret = 0;
			// To make things easy (and save a bunch of ifs), bitwise and
			// will make light work of that
			if( m_TraceLevel & p_Level )
			{
				// Too much, or not enough?  Should this be user-defined?
				char CompleteMessage[ 4096 ];

				va_list ArgPtr;

				va_start( ArgPtr, p_pMessage );
				Ret =
#ifdef ZED_COMPILER_MSVC
					_vsnprintf( CompleteMessage, 4096, p_pMessage, ArgPtr );
#elif defined ZED_COMPILER_GCC
					vsnprintf( CompleteMessage, 4096, p_pMessage, ArgPtr );
#else
#error Unknown compiler
#endif
				va_end( ArgPtr );

				OutputDebugStringA( CompleteMessage );
			}
			return Ret;
		}
	}
}
