#ifndef __ZED_SYSTEM_DATATYPES_WINDOWS_X86_64_HPP__
#define __ZED_SYSTEM_DATATYPES_WINDOWS_X86_64_HPP__

#ifdef __GNUC__
#include <stdint.h>
#include <cstddef>
#endif

#include <wchar.h>

typedef char				ZED_CHAR8;
typedef signed char			ZED_SCHAR8;
typedef unsigned char		ZED_UCHAR8;
typedef wchar_t				ZED_CHAR16;
typedef signed char			ZED_SINT8;
typedef unsigned char		ZED_UINT8;
typedef signed short		ZED_SINT16;
typedef unsigned short		ZED_UINT16;
typedef signed int			ZED_SINT32;
typedef unsigned int		ZED_UINT32;
#ifdef _MSC_VER
typedef signed __int64		ZED_SINT64;
typedef unsigned __int64	ZED_UINT64;
#elif __GNUC__
typedef int64_t				ZED_SINT64;
typedef uint64_t			ZED_UINT64;
#else
#error Unknown compiler
#endif
typedef unsigned long		ZED_DWORD;

// Define the platform as x86 Endian (Little)
#define ZED_BIG_ENDIAN		0
#define ZED_LITTLE_ENDIAN	1

#define ZED_32BIT	0
#define ZED_64BIT	1

// For Doxygen
/**
	\mainpage ZED Engine [Ver. Windows x86_64]

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

