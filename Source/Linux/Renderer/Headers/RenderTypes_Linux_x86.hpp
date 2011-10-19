#ifndef __ZED_RENDERER_LINUXRENDERTYPES_HPP__
#define __ZED_RENDERER_LINUXRENDERTYPES_HPP__

// Format types for the different buffers, etc.
typedef enum __ZED_FORMAT
{
	// Hope to hell that nothing takes this (other than more undefined formats)
	ZED_FORMAT_UNDEFINED = 0xFFFFFFFF,

	// Colour
	ZED_FORMAT_A8R8G8B8 =	0x00000001,
	ZED_FORMAT_X8R8G8B8 =	0x00000002,

	// Depth/Stencil
	ZED_FORMAT_D24S8 =		0x0000000F,
} ZED_FORMAT;


// Input types for shader constants
typedef enum __ZED_SHADER_INPUT_TYPE
{
	ZED_FLOAT1	= 0x00000001,
	ZED_FLOAT2	= 0x00000002,
	ZED_FLOAT3 	= 0x00000003,
	ZED_FLOAT4	= 0x00000004,

	ZED_INT1	= 0x00000005,
	ZED_INT2	= 0x00000006,
	ZED_INT3	= 0x00000007,
	ZED_INT4	= 0x00000008,

	ZED_MAT2X2	= 0x00000009,
	ZED_MAT3X3	= 0x0000000A,
	ZED_MAT4X4	= 0x0000000B,
}ZED_SHADER_INPUT_TYPE;


// Different shader program types
typedef enum __ZED_SHADER_FORMAT
{
	ZED_VERTEX_SHADER	= 0x00000001,
	ZED_FRAGMENT_SHADER	= 0x00000002,
	ZED_GEOMETRY_SHADER	= 0x00000003,
	ZED_UNKNOWN_SHADER	= 0x7FFFFFFF,
}ZED_SHADER_FORMAT;

// Antialiasing types supported by Linux
typedef enum __ZED_SAMPLE_TYPE
{
	ZED_SAMPLE_TYPE_NONE	= 0x00000001,
	ZED_SAMPLE_TYPE_MSAA_1	= 0x00000002,
	ZED_SAMPLE_TYPE_MSAA_2	= 0x00000004,
	ZED_SAMPLE_TYPE_MSAA_4	= 0x00000008,
	ZED_SAMPLE_TYPE_MSAA_8	= 0x0000000F,
	ZED_SAMPLE_TYPE_CSAA_1	= 0x00000010,
	ZED_SAMPLE_TYPE_CSAA_2	= 0x00000020,
	ZED_SAMPLE_TYPE_CSAA_4	= 0x00000040,
	ZED_SAMPLE_TYPE_CSAA_8	= 0x00000080,
	ZED_SAMPLE_TYPE_UNKNOWN = 0x7FFFFFFF,
}ZED_SAMPLE_TYPE;

#endif

