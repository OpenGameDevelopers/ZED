#ifndef __ZED_SYSTEM_COMPILER_HPP__
#define __ZED_SYSTEM_COMPILER_HPP__

// Use the compiler-specific settings for compilation
#ifdef _MSC_VER
	#include <System/Compiler_MSVC.hpp>
	#define ZED_COMPILER_MSVC
#endif

#ifdef __GNUC__
	#include <System/Compiler_GCC.hpp>
	#define ZED_COMPILER_GCC
#endif

// Manual data alignment
#ifndef ZED_PACKDATA_BEGIN
	#define ZED_PACKDATA_BEGIN( p_Align )
#endif

#ifndef ZED_PACKDATA_END
	#define ZED_PACKDATA_END( p_Align )
#endif

// Force inline
#ifndef ZED_INLINE
	#define ZED_INLINE			inline
#endif

#ifndef ZED_FORCE_INLINE
	#define ZED_FORCE_INLINE	__force_inline
#endif

#ifndef ZED_API
	#define ZED_API
#endif

#endif

