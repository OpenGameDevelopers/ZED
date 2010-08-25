#ifndef __ZEDDEBUGGER_HPP__
#define __ZEDDEBUGGER_HPP__

#include <DataTypes.hpp>
#include <ostream>

// When debugging, correctly redirect, otherwise use sizeof() so the compiler
// crops it away
#if ZED_BUILD_DEBUG
#define zedTrace	ZED::System::Trace
#else
#define zedTrace	sizeof
#endif

typedef std::ostream ZED_Writer;

namespace ZED
{
	namespace System
	{
		void Trace( const char *p_pMessage );
		void Trace( ZED_UINT32 p_Level, const char *p_pMessage, ... );
	}
}

#endif
