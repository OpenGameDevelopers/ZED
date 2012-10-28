#ifndef __ZEDDATATYPE_WIN32_X86_HPP__
#define __ZEDDATATYPE_WIN32_X86_HPP__

#include <wchar.h>

typedef char				ZED_CHAR8;
typedef signed char			ZED_SCHAR8;
typedef unsigned char		ZED_UCHAR8;
typedef signed char			ZED_INT8;
typedef unsigned char		ZED_UINT8;
typedef wchar_t				ZED_CHAR16;
typedef signed short		ZED_INT16;
typedef unsigned short		ZED_UINT16;
typedef signed int			ZED_INT32;
typedef unsigned int		ZED_UINT32;
typedef signed __int64		ZED_INT64;
typedef unsigned __int64	ZED_UINT64;
typedef unsigned long		ZED_DWORD;

// Define the platform as x86 Endian (Little)
#define ZED_BIG_ENDIAN		0
#define ZED_LITTLE_ENDIAN	1

#define ZED_32BIT	1
#define ZED_64BIT	0

// For Doxygen
/**
	\mainpage ZED Engine [Ver. Windows x86_32]

	\section overview Overview

	The ZED Engine is a cross-platform C, C++ and (in some instances) Assembly
	files which will be as consistent as possible across a wide range of
	platforms.

	\section supported_platforms Supported Platforms
	- Linux x86 (32- and 64-bit)
	- Windows x86 (32- and 64-bit)
	- Xbox
	- Pandora
*/

#endif
