#ifndef __ZEDCOMPILER_GCC_HPP__
#define __ZEDCOMPILER_GCC_HPP__

#define ZED_PACKDATA_BEGIN( Align )
#define ZED_PACKDATA_END( Align )	__attribute__( ( __aligned__( Align ) ) )

#define ZED_INLINE					__inline__
#define ZED_FORCE_INLINE			__forceinline__

#define ZED_EXPLICIT				explicit

#endif

