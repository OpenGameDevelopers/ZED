#ifndef __ZEDWINDOWSRENDERTYPES_HPP__
#define __ZEDWINDOWSRENDERTYPES_HPP__

// Format types for the different buffers, etc.
typedef enum _ZED_FORMAT
{
	// Hope to hell that nothing takes this (other than more undefined formats)
	ZED_FORMAT_UNDEFINED = 0xFFFFFFFF,

	// Colour
	ZED_FORMAT_A8R8G8B8 =	0x00000001,
	ZED_FORMAT_X8R8G8B8 =	0x00000002,

	// Depth/Stencil
	ZED_FORMAT_D24S8 =		0x0000000F,
} ZED_FORMAT;

// Types to use for a shader [Vertex, Fragment, and Geometry]
typedef enum __ZED_SHADER_INPUT_TYPE
{
	ZED_FLOAT1		= 0x00000001,
	ZED_FLOAT2		= 0x00000002,
	ZED_FLOAT3		= 0x00000003,
	ZED_FLOAT4		= 0x00000004,

	ZED_INT1		= 0x00000005,
	ZED_INT2		= 0x00000006,
	ZED_INT3		= 0x00000007,
	ZED_INT4		= 0x00000008,

	ZED_MAT2X2		= 0x00000009,
	ZED_MAT3X3		= 0x00000010,
	ZED_MAT4X4		= 0x00000011,
}ZED_SHADER_INPUT_TYPE;


typedef enum __ZED_SHADER_TYPE
{
	ZED_VERTEX_SHADER	= 0x00000001,
	ZED_FRAGMENT_SHADER	= 0x00000002,
	ZED_GEOMETRY_SHADER = 0x00000003,
	ZED_UNKNOWN_SHADER	= 0xFFFFFFFF
}ZED_SHADER_TYPE;
#endif
