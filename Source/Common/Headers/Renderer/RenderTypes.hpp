#ifndef __ZED_RENDERER_RENDERTYPES_HPP__
#define __ZED_RENDERER_RENDERTYPES_HPP__

#include <System/DataTypes.hpp>

// -- CONSTANTS --
const ZED_MEMSIZE ZED_ENABLE = 1;
const ZED_MEMSIZE ZED_DISABLE = 0;

// Define the texture file types
const ZED_UINT32	ZED_PNG = 0x00000001;
const ZED_UINT32	ZED_DDS = 0x00000002;
const ZED_UINT32	ZED_BMP = 0x00000004;
const ZED_UINT32	ZED_TGA = 0x00000008;

// -- ENUMS --
typedef enum __ZED_RENDERER_BACKEND
{
	ZED_RENDERER_BACKEND_OPENGL			= 0x00000001,
	ZED_RENDERER_BACKEND_OPENGLES2		= 0x00000002,
	ZED_RENDERER_BACKEND_DIRECT3D		= 0x00000010,
	ZED_RENDERER_BACKEND_DIRECT3D_XBOX	= 0x00000020,

	ZED_RENDERER_BACKEND_UNKNOWN	= ZED_ENUM_PAD,
}ZED_RENDERER_BACKEND;

// How should the world be rendered?
typedef enum __ZED_VIEWMODE
{
	// View with Z
	ZED_VIEWMODE_PERSPECTIVE,
	// View without Z
	ZED_VIEWMODE_ORTHOGONAL,
	// View aligned to screen coordinates
	ZED_VIEWMODE_SCREEN,

	ZED_VIEWMODE_INVALID = ZED_ENUM_PAD
}ZED_VIEWMODE;

// Format types for render buffers
typedef enum __ZED_FORMAT
{
	// Colour formats
	ZED_FORMAT_ARGB8	=	0x00000001,
	ZED_FORMAT_XRGB8	=	0x00000002,
	ZED_FORMAT_RGB565	=	0x00000003,
	ZED_FORMAT_ARGB1555	=	0x00000004,
	ZED_FORMAT_RGB8		=	0x00000005,

	// Depth/Stencil formats
	ZED_FORMAT_D24S8	=	0x00000011,

	ZED_FORMAT_UNDEFINED	=	0x0000FFFF,

	ZED_FORMAT_UNKNOWN	=	ZED_ENUM_PAD,
}ZED_FORMAT;

// The ZED_RENDERSTATEs are used for setting the various graphics functions
// on the GPU
typedef enum __ZED_RENDERSTATE
{
	ZED_RENDERSTATE_DEPTH,
	ZED_RENDERSTATE_FILLMODE,
	ZED_RENDERSTATE_CULLMODE,

	ZED_RENDERSTATE_PADDING	= ZED_ENUM_PAD,
}ZED_RENDERSTATE;

// Fill mode is only applicable to fixed function pipeline GPUs
typedef enum __ZED_FILLMODE
{
	ZED_FILLMODE_WIREFRAME,
	ZED_FILLMODE_SOLID,
	ZED_FILLMODE_POINT,

	ZED_FILLMODE_PADDING = ZED_ENUM_PAD,
}ZED_FULLMODE;

// Culling is CCW by default
typedef enum __ZED_CULLMODE
{	
	ZED_CULLMODE_NONE,
	ZED_CULLMODE_CCW,
	ZED_CULLMODE_CW,

	ZED_CULLMODE_PADDING = ZED_ENUM_PAD,
}ZED_CULLMODE;

typedef enum __ZED_SHADER_CONSTANT_TYPE
{
	ZED_FLOAT1	=	0x00000001,
	ZED_FLOAT2	=	0x00000002,
	ZED_FLOAT3	=	0x00000003,
	ZED_FLOAT4	=	0x00000004,
	
	ZED_INT1	=	0x00000011,
	ZED_INT2	=	0x00000012,
	ZED_INT3	=	0x00000013,
	ZED_INT4	=	0x00000014,

	ZED_MAT2X2	=	0x00000021,
	ZED_MAT3X3	=	0x00000022,
	ZED_MAT4X4	=	0x00000023,

	ZED_SHADER_CONSTANT_TYPE_UNKNOWN	=	ZED_ENUM_PAD,
}ZED_SHADER_CONSTANT_TYPE;

typedef enum __ZED_SHADER_VERTEXATTRIBUTE_TYPE
{
}ZED_SHADER_VERTEXATTRIBUTE_TYPE;

typedef enum __ZED_SHADER_TYPE
{
	ZED_VERTEX_SHADER	=	0x00000001,
	ZED_FRAGMENT_SHADER	=	0x00000002,
	ZED_GEOMETRY_SHADER	=	0x00000002,

	ZED_UNKNOWN_SHADER	=	ZED_ENUM_PAD,
}ZED_SHADER_TYPE;

typedef enum __ZED_SAMPLE_TYPE
{
	ZED_SAMPLE_TYPE_NONE	=	0x00000001,

	ZED_SAMPLE_TYPE_MSAA_1	=	0x00000002,
	ZED_SAMPLE_TYPE_MSAA_2	=	0x00000004,
	ZED_SAMPLE_TYPE_MSAA_4	=	0x00000008,
	ZED_SAMPLE_TYPE_MSAA_8	=	0x00000010,
	ZED_SAMPLE_TYPE_MSAA_16	=	0x00000020,

	ZED_SAMPLE_TYPE_CSAA_1	=	0x00000040,
	ZED_SAMPLE_TYPE_CSAA_2	=	0x00000080,
	ZED_SAMPLE_TYPE_CSAA_4	=	0x00000100,
	ZED_SAMPLE_TYPE_CSAA_8	=	0x00000200,
	ZED_SAMPLE_TYPE_CSAA_16	=	0x00000400,

	ZED_SAMPLE_TYPE_UNKNOWN	=	ZED_ENUM_PAD,
}ZED_SAMPLE_TYPE;

// -- STRUCTS --
// Used for colour data
typedef struct _ZED_COLOUR
{
	ZED_FLOAT32 Red;
	ZED_FLOAT32 Green;
	ZED_FLOAT32 Blue;
	ZED_FLOAT32 Alpha;
} ZED_COLOUR;

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
typedef struct __ZED_POINT
{
	ZED_SINT32 X;
	ZED_SINT32 Y;
}ZED_POINT;

typedef struct __ZED_SCREEN_POINT
{
	ZED_UINT32	X;
	ZED_UINT32	Y;
}ZED_SCREEN_POINT;

// General-purpose rectangle
typedef struct __ZED_RECTANGLE
{
	ZED_SINT32	X;
	ZED_SINT32	Y;
	ZED_SINT32	Width;
	ZED_SINT32	Height;
}ZED_RECTANGLE;

// Used to specify shader uniform variables
// N.B.
//	pName should always be ZED_NULL on Xbox, as it isn't required.
typedef struct __ZED_SHADER_CONSTANT_MAP
{
	// Index for the user to use for identifying where the input is
	// logically
	ZED_UINT32			Index;
	// The /real/ location of the input
	ZED_UINT32			Location;
	// The type of input, i.e. ZED_VEC[1|2|3|4]
	ZED_SHADER_CONSTANT_TYPE Type;
	// Name is used for OpenGL shaders
	ZED_CHAR8	*pName;
}ZED_SHADER_CONSTANT_MAP;

typedef struct __ZED_SHADER_VERTEXATTRIBUTE
{
	ZED_SHADER_VERTEXATTRIBUTE_TYPE	Type;
	ZED_UINT32						Index;
	ZED_UINT32						Offset;
}ZED_SHADER_VERTEXATTRIBUTE;

namespace ZED
{
	namespace Renderer
	{
		const ZED_CHAR8 *FormatToString( const ZED_FORMAT p_Format );
		ZED_MEMSIZE FormatToBytes( const ZED_FORMAT p_Format );
	}
}

// Include patform-specific headers
#if defined ZED_PLATFORM_WINDOWS || ZED_PLATFORM_LINUX
	#include <Renderer/OGL/GLRenderTypes.hpp>
#elif defined ZED_PLATFORM_PANDORA_LINUX
	#include <Renderer/OGLES2/GLES2RenderTypes.hpp>
#elif defined ZED_PLATFORM_XBOX
	#include <xtl.h>
	#include <RenderTypes_Xbox.hpp>
#else
	#error Unknown platform
#endif

#endif

