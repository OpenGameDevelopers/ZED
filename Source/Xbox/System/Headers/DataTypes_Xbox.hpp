#ifndef __ZEDDATATYPE_XBOX_HPP__
#define __ZEDDATATYPE_XBOX_HPP__

#include <tchar.h>

typedef signed char			ZED_CHAR8;
typedef unsigned char		ZED_UCHAR8;
typedef signed char			ZED_INT8;
typedef unsigned char		ZED_UINT8;
typedef wchar_t				ZED_CHAR16;
typedef signed short		ZED_INT16;
typedef unsigned short		ZED_UINT16;
typedef signed int			ZED_INT32;
typedef unsigned int		ZED_UINT32;
typedef __int64				ZED_INT64;
typedef unsigned __int64	ZED_UINT64;
typedef unsigned long		ZED_DWORD;

// Define the platform as x86 Endian (Little)
#define ZED_BIG_ENDIAN		0
#define ZED_LITTLE_ENDIAN	1

#endif
