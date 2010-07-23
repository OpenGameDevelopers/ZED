#ifndef __ZEDRENDERTYPES_HPP__
#define __ZEDRENDERTYPES_HPP__

// Include patform-specific headers
#if ZED_PLATFORM_WIN32_X86
	#include <RenderTypes_Win32_x86.hpp>
#elif ZED_PLATFORM_XBOX
	#include <RenderTypes_Xbox.hpp>
#endif

#endif
