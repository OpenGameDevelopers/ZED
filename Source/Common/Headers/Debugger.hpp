#ifndef __ZEDDEBUGGER_HPP__
#define __ZEDDEBUGGER_HPP__

#include <DataTypes.hpp>
#include <DebuggerTypes.hpp>
#include <ostream>
#include <cassert>

// When debugging, correctly redirect, otherwise use sizeof() so the compiler
// crops it away
#if ZED_BUILD_DEBUG
#define zedTrace	ZED::System::Trace
// __asm is not available on Windows x86-64
#ifdef ZED_PLATFORM_WIN64_X86
#define zedDebugBreak( )\
	{\
	}
#elif ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_XBOX
#define zedDebugBreak( )\
	{\
	__asm { int 3 } \
	}
#else
#define zedDebugBreak( )\
	{\
		ZED_ASM( "INT $3;\n" );\
	}
#endif
#endif

#ifdef ZED_BUILD_DEBUG
#define zedAssert( Expr ) \
	if( Expr ) { } \
	else \
	{ \
		zedTrace(	"ASSERTION FAILURE\n%s | FILE: %d | LINE: %d\n\n", \
					#Expr, \
					__FILE__, \
					__LINE__ ); \
		zedDebugBreak( ); \
	}
#else
#define zedAssert( Expr )
#define zedTrace	sizeof
#endif

namespace ZED
{
	namespace System
	{
		// Convenience detour
		typedef std::ostream Writer;

		// Similar to the Debugger::Trace without the levels
		ZED_INT32 Trace( const char *p_pMessage, ... );

		class Debugger
		{
		public:
			ZED_INLINE ZED_EXPLICIT Debugger( ZED_UINT32 p_TraceLevel ) :
				m_TraceLevel( p_TraceLevel ){ }
			ZED_INLINE ~Debugger( ){ }

			void SetTraceLevel( ZED_UINT32 p_Level ){ m_TraceLevel = p_Level; }

			ZED_INT32 Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... );
		private:
			ZED_UINT32 m_TraceLevel;
		};
	}
}

#endif
