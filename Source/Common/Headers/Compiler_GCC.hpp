#ifndef __ZEDCOMPILER_GCC_HPP__
#define __ZEDCOMPILER_GCC_HPP__

// Saves on elaborate checking [40401 == GCC 4.4.1]
#define ZED_GCCVER	( __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + \
	__GNUC_PATCHLEVEL__ )

#define ZED_PACKDATA_BEGIN( p_Align )
#define ZED_PACKDATA_END( p_Align )	__attribute__( ( __aligned__( p_Align ) ) )

#define ZED_INLINE					__inline__
#define ZED_FORCE_INLINE			__forceinline__

#define ZED_EXPLICIT				explicit
#define ZED_ASM						__asm__

// GCC 4.6.0 supports true null!
#if ZED_GCCVER >= 40600
#define ZED_NULL	nullptr
#else
#define ZED_NULL	0
#endif

#endif

