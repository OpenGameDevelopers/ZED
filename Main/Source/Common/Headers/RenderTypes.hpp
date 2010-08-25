#ifndef __ZEDRENDERTYPES_HPP__
#define __ZEDRENDERTYPES_HPP__

// Include patform-specific headers
#if ( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
	#include <RenderTypes_Windows_x86.hpp>
#elif ZED_PLATFORM_XBOX
	#include <xtl.h>
	#include <RenderTypes_Xbox.hpp>
#endif

// Used for colour data
typedef struct _ZED_COLOUR
{
	ZED_FLOAT32 Red;
	ZED_FLOAT32 Green;
	ZED_FLOAT32 Blue;
	ZED_FLOAT32 Alpha;
} ZED_COLOUR;

#endif
