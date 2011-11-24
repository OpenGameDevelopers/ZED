#ifndef __ZEDDATATYPE_LINUX64_X86_HPP__
#define __ZEDDATATYPE_LINUX64_X86_HPP__

// int64_t and uint64_t
#include <stdint.h>
// size_t
#include <linux/types.h>

typedef char				ZED_CHAR8;
typedef signed char			ZED_SCHAR8;
typedef unsigned char		ZED_UCHAR8;
typedef wchar_t				ZED_CHAR16;
typedef signed char			ZED_INT8;
typedef unsigned char		ZED_UINT8;
typedef signed short		ZED_INT16;
typedef unsigned short		ZED_UINT16;
typedef signed int			ZED_INT32;
typedef unsigned int		ZED_UINT32;
typedef int64_t				ZED_INT64;
typedef uint64_t			ZED_UINT64;

// Define the platform as x86 Endian (Little)
#define ZED_BIG_ENDIAN		0
#define ZED_LITTLE_ENDIAN	1

#endif

