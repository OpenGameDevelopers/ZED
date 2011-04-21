#ifndef __ZEDDATATYPE_HPP__
#define __ZEDDATATYPE_HPP__

// Mac OS platform needed for testing before implementation!
#if ZED_PLATFORM_WIN32_X86
	#include <DataTypes_Win32_x86.hpp>
#elif ZED_PLATFORM_WIN64_X86
	#include <DataTypes_Win64_x86.hpp>
#elif ZED_PLATFORM_PANDORA
	#include <DataTypes_Pandora.hpp>
#elif ZED_PLATFORM_LINUX32_X86
	#include <DataTypes_Linux32_x86.hpp>
#elif ZED_PLATFORM_LINUX64_X86
	#include <DataTypes_Linux64_x86.hpp>
#elif ZED_PLATFORM_XBOX
	#include <DataTypes_Xbox.hpp>
#endif

// Also include compiler-specific header
#include <Compiler.hpp>

#include <String.hpp>

// A few more basic types
typedef ZED_UCHAR8	ZED_BYTE;
typedef ZED_UINT32	ZED_BOOL;

// Similes for the long data type
typedef ZED_INT32	ZED_LONG;
typedef ZED_UINT32	ZED_ULONG;
typedef ZED_INT32	ZED_LONG32;
typedef ZED_UINT32	ZED_ULONG32;

// Floating point types (single and double)
typedef float	ZED_FLOAT32;
typedef double	ZED_FLOAT64;

// Endian swapping
ZED_INLINE ZED_UINT16 zedSwapEndian16( ZED_UINT16 p_Swap )
{
	return	( ( p_Swap & 0x00FF ) << 8 ) |
			( ( p_Swap & 0xFF00 ) >> 8 );
}

ZED_INLINE ZED_UINT32 zedSwapEndian32( ZED_UINT32 p_Swap )
{
	return	( ( p_Swap & 0x000000FF ) << 24 ) |
			( ( p_Swap & 0x0000FF00 ) << 8 ) |
			( ( p_Swap & 0x00FF0000 ) >> 8 ) |
			( ( p_Swap & 0xFF000000 ) >> 24 );
}

#if ZED_ENDIAN_LITTLE == 1
	#define zedEndianToBig16( p_Swap )	zedSwapEndian16( p_Swap )
	#define zedEndianToLittle16( p_Swap )
	#define zedEndianToBig32( p_Swap )	zedSwapEndian32( p_Swap )
	#define zedEndianToLittle32( p_Swap )
#elif ZED_ENDIAN_BIG == 1
	#define zedEndianToBig16( p_Swap )
	#define zedEndianToLittle16( p_Swap )	zedSwapEndian16( p_Swap )
	#define zedEndianToBig32( p_Swap )
	#define zedEndianToLittle32( p_Swap )	zedSwapEndian32( p_Swap )
#else
	/*REMINDER! 
	zedCompileTimeError( "Endianess is not set" );
	!REMINDER*/
#endif

// Alignment for data
#define zedAlign( p_Obj, p_Size )	( ( ( size_t )( p_Obj )+( p_Size )- 1 ) &\
									( ~( ( p_Size ) - 1 ) ) )
#define zedAlignUp( p_Obj, p_Size ) ( zedAlign( ( p_Obj ), ( p_Size ) ) +\
									( p_Size ) )

// Common alignment helpers
#define zedAlign4( p_Obj )		zedAlign( p_Obj, 4 )
#define zedAlign8( p_Obj )		zedAlign( p_Obj, 8 )
#define zedAlign16( p_Obj )		zedAlign( p_Obj, 16 )
#define zedAlign32( p_Obj )		zedAlign( p_Obj, 32 )
#define zedAlign64( p_Obj )		zedAlign( p_Obj, 64 )
#define zedAlign128( p_Obj )		zedAlign( p_Obj, 128 )

#define zedIsAligned( p_Obj, p_Size )( ( ( size_t )( p_Obj )&( ( p_Size )- 1 )\
									 ) == 0 )

// Define True and False (just for cross-platform's sake )
#define ZED_TRUE	1
#define ZED_FALSE	0
#define ZED_NULL	0

typedef size_t		ZED_MEMSIZE;
typedef size_t		ZED_DISCSIZE;

// As enums can vary in size, pad them out.
const ZED_INT32	ZED_ENUM_PADDING	= 0x7FFFFFFF;

// Return status
const ZED_UINT32 ZED_OK			= 0x00000001;
const ZED_UINT32 ZED_EQUAL		= ZED_OK;
const ZED_UINT32 ZED_PLANARFACE	= ZED_OK;
const ZED_UINT32 ZED_LESS		= 0x00000002;
const ZED_UINT32 ZED_BACKFACE	= ZED_LESS;
const ZED_UINT32 ZED_GREATER	= 0x00000004;
const ZED_UINT32 ZED_FRONTFACE	= ZED_GREATER;
const ZED_UINT32 ZED_FAIL		= 0x7FFFFFFF;

// Define the chunk IDs
const ZED_UINT16	ZED_R1_HEADER	= 0x0100;
const ZED_UINT16	ZED_R1_VERTEX	= 0x0200;
const ZED_UINT16	ZED_R1_FACE		= 0x0300;
const ZED_UINT16	ZED_R1_MESH		= 0x0400;
const ZED_UINT16	ZED_R1_MATERIAL	= 0x0500;
const ZED_UINT16	ZED_R1_END		= 0xFFFF;

// The largest possible path (though, this could be platform dependent)
const ZED_UINT32	ZED_MAX_PATH	= 256;

#endif

