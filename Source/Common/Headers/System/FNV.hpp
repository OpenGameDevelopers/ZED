#ifndef __ZED_SYSTEM_FNV_HPP__
#define __ZED_SYSTEM_FNV_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_UINT32 FNV32_PRIME	= 0x01000193;
		const ZED_UINT32 FNV32_OFFSET	= 0x811C9DC5;

		ZED_UINT32 HashBufferFNV1a( void *p_pBuffer,
			const ZED_MEMSIZE p_Length,
			ZED_UINT32 p_PreviousHash = FNV32_OFFSET );

		ZED_UINT32 HashStringFNV1a( ZED_CHAR8 *p_pString,
			ZED_UINT32 p_PreviousHash = FNV32_OFFSET );
	}
}

#endif // __ZED_SYSTEM_FNV_HPP__

