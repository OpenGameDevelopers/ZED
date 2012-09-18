#ifndef __ZEDCOMPILER_MSVC_HPP__
#define __ZEDCOMPILER_MSVC_HPP__

#define ZED_PACKDATA_BEGIN( p_Align )	__declspec( align( p_Align ) )
#define ZED_PACKDATA_END( p_Align )

#define ZED_INLINE						__inline
#define ZED_FORCE_INLINE				__forceinline

#define ZED_EXPLICIT					explicit

// C++ 11 NULL is real!
#if( _MSC_VER >= 1600 )
#define ZED_NULL	nullptr
#define ZED_CPPVER_03
#else
#define ZED_NULL	0
#define ZED_CPPVER_03
#endif

// Switch off benign warnings
// -Unreferenced formal parameter
#pragma warning( disable : 4100 )
// -Copy constructor should not be generated
#pragma warning( disable : 4511 )
// -Assignment constructor should not be generated
#pragma warning( disable : 4512 )
// -C++ language change
#pragma warning( disable : 4663 )
// -ID was truncated to 255 characters
#pragma warning( disable : 4786 )

#endif
