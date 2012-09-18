#ifndef __ZEDDATATYPE_HPP__
#define __ZEDDATATYPE_HPP__

// Select the platform-specific Data Types
#if ZED_PLATFORM_WIN32_X86
	#include <DataTypes_Win32_x86.hpp>
#elif ZED_PLATFORM_WIN64_X86
	#include <DataTypes_Win64_x86.hpp>
#elif ZED_PLATFORM_PANDORA
	#include <DataTypes_Pandora.hpp>
#elif ZED_PLATFORM_PANDORA_LINUX
	#include <DataTypes_PandoraLinux.hpp>
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

// Helper for FourCC values
ZED_INLINE ZED_UINT32 MakeDword( ZED_BYTE p_A, ZED_BYTE p_B, ZED_BYTE p_C, ZED_BYTE p_D )
{
	return ( ( p_A )*0x1000000+( p_B )*0x10000+( p_C )*0x100+( p_D ) );
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

// Define True, False and NULL (just for cross-platform's sake )
#define ZED_TRUE	1
#define ZED_FALSE	0

typedef size_t		ZED_MEMSIZE;
typedef size_t		ZED_DISCSIZE;

// As enums can vary in size, pad them out.
const ZED_INT32	ZED_ENUM_PADDING	= 0x7FFFFFFF;

// Return status
const ZED_UINT32 ZED_OK				= 0x00000001;
const ZED_UINT32 ZED_EQUAL			= ZED_OK;
const ZED_UINT32 ZED_PLANARFACE		= ZED_OK;
const ZED_UINT32 ZED_LESS			= 0x00000002;
const ZED_UINT32 ZED_BACKFACE		= ZED_LESS;
const ZED_UINT32 ZED_GREATER		= 0x00000004;
const ZED_UINT32 ZED_FRONTFACE		= ZED_GREATER;
const ZED_UINT32 ZED_GRAPHICS_ERROR	= 0x00000004;
const ZED_UINT32 ZED_FAIL			= 0x7FFFFFFF;

// Define the chunk IDs for models
const ZED_UINT16	ZED_MODEL_HEADER	= 0x0001;
const ZED_UINT16	ZED_MODEL_MESH		= 0x0002;
const ZED_UINT16	ZED_MODEL_SKELETON	= 0x0004;
const ZED_UINT16	ZED_MODEL_ANIMATION	= 0x0008;
const ZED_UINT16	ZED_MODEL_IKCHAIN	= 0x0010;
const ZED_UINT16	ZED_MODEL_MATERIAL	= 0x0012;
const ZED_UINT16	ZED_MODEL_END		= 0xFFFF;

// The largest possible path (though, this could be platform dependent)
const ZED_UINT32	ZED_MAX_PATH	= 256;

// For padding out enums to 32 bits
#ifdef ZED_64BIT
const ZED_MEMSIZE ZED_ENUM_PAD = 0x7FFFFFFFFFFFFFFF;
#elif ZED_32BIT
const ZED_MEMSIZE ZED_ENUM_PAD = 0x7FFFFFFF;
#endif

#endif

