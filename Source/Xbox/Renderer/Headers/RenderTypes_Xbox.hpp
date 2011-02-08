#ifndef __ZEDRENDERTYPES_XBOX_HPP__
#define __ZEDRENDERTYPES_XBOX_HPP__

// Format types for the different buffers, etc.
typedef enum _ZED_FORMAT
{
	// Hope to hell that nothing takes this (other than more undefined formats)
	ZED_FORMAT_UNDEFINED = D3DFMT_UNKNOWN,

	// Unfortunately, these are defined using the preprocessor.  Is there a
	// more elegant way?

	// Colour
	ZED_FORMAT_A8R8G8B8 =	D3DFMT_A8R8G8B8,
	ZED_FORMAT_X8R8G8B8 =	D3DFMT_X8R8G8B8,

	// Depth/Stencil
	ZED_FORMAT_D24S8 =		D3DFMT_D24S8,
} ZED_FORMAT;

#endif
