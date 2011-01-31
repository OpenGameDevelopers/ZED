#ifndef __ZEDDEBUGGER_HPP__
#define __ZEDDEBUGGER_HPP__

#include <DataTypes.hpp>
#include <DebuggerTypes.hpp>
#include <ostream>

// When debugging, correctly redirect, otherwise use sizeof() so the compiler
// crops it away
#if ZED_BUILD_DEBUG
#define zedTrace	ZED::System::Trace
#else
#define zedTrace	sizeof
#endif

namespace ZED
{
	namespace System
	{
		// Convenience detour
		typedef std::ostream Writer;

		void Trace( const char *p_pMessage );

		class Debugger
		{
		public:
			ZED_INLINE ZED_EXPLICIT Debugger( ZED_UINT32 p_TraceLevel ) :
				m_TraceLevel( p_TraceLevel ){ }
			ZED_INLINE ~Debugger( ){ }

			void SetTraceLevel( ZED_UINT32 p_Level ){ m_TraceLevel = p_Level; }

			void Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... );
		private:
			ZED_UINT32 m_TraceLevel;
		};
	}
}

#endif
