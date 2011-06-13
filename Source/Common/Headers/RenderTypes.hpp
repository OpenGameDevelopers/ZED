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
// NOTE!
// This is VERY OpenGL-specific, it is temporary and will be replaced in a
// later revision
// !NOTE
typedef struct __ZED_SHADER_INPUT_MAP
{
	ZED_UINT32			Location;
	const ZED_UCHAR8	*pName;
}ZED_SHADER_INPUT_MAP;

#endif
