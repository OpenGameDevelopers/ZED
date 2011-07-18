#ifndef __ZEDRENDERTYPES_HPP__
#define __ZEDRENDERTYPES_HPP__

// Include patform-specific headers
#if ( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
	#include <RenderTypes_Windows_x86.hpp>
#elif ZED_PLATFORM_XBOX
	#include <xtl.h>
	#include <RenderTypes_Xbox.hpp>
#elif ( ZED_PLATFORM_LINUX32_X86 || ZED_PLATFORM_LINUX64_X86 )
	#include <RenderTypes_Linux_x86.hpp>
#endif

// Used for colour data
typedef struct _ZED_COLOUR
{
	ZED_FLOAT32 Red;
	ZED_FLOAT32 Green;
	ZED_FLOAT32 Blue;
	ZED_FLOAT32 Alpha;
} ZED_COLOUR;

// How should the world be rendered?
typedef enum _ZED_VIEWMODE
{
	// View with Z
	ZED_VIEW_PERSPECTIVE,
	// View without Z
	ZED_VIEW_ORTHOGONAL,
	// View aligned to screen coordinates
	ZED_VIEW_SCREEN
}ZED_VIEWMODE;

// Viewport stucture for rendering different views within a Canvas
// Note:	Rendering to different render targets/textures would be more
//			ideal for newer hardware (nVIDIA GF3> && ATi R7500>)
//			[Performance levels can be queried at runtime]
typedef struct _ZED_VIEWPORT
{
	// Upper-left corner
	ZED_UINT32 X;
	ZED_UINT32 Y;

	ZED_UINT32 Width;
	ZED_UINT32 Height;
}ZED_VIEWPORT;

// Screen-space points
typedef struct _ZED_POINT
{
	ZED_UINT32 X;
	ZED_UINT32 Y;
}ZED_POINT;

// Used to specify shader input variables
// N.B.
//	pName should always be ZED_NULL on Xbox, as it isn't required.
typedef struct __ZED_SHADER_INPUT_MAP
{
	// Index for the user to use for identifying where the input is
	// logically
	ZED_UINT32			Index;
	// The /real/ location of the input
	ZED_UINT32			Location;
	// The type of input, i.e. ZED_VEC[1|2|3|4]
	ZED_SHADER_INPUT_TYPE Type;
	// Name is used for OpenGL shaders
	ZED_CHAR8	*pName;
}ZED_SHADER_INPUT_MAP;

typedef struct __ZED_SHADER_VA_MAP
{
	ZED_UINT32 Index;
	ZED_SHADER_INPUT_TYPE Type;
}ZED_SHADER_VA_MAP;

typedef struct __ZED_GLVERSION
{
	ZED_INT32	Major;
	ZED_INT32	Minor;
}ZED_GLVERSION;

// Define the texture file types
const ZED_UINT32	ZED_PNG = 0x00000001;
const ZED_UINT32	ZED_DDS = 0x00000002;
const ZED_UINT32	ZED_BMP = 0x00000004;
const ZED_UINT32	ZED_TGA = 0x00000008;

#endif
