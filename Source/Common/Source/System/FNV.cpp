#include <System/FNV.hpp>

namespace ZED
{
	namespace System
	{
		ZED_UINT32 HashBufferFNV1a( void *p_pBuffer,
			const ZED_MEMSIZE p_Length, ZED_UINT32 p_PreviousHash )
		{
			ZED_BYTE *pStart = static_cast< ZED_BYTE * >( p_pBuffer );
			ZED_BYTE *pEnd = pStart + p_Length;

			// Go byte-by-byte
			while( pStart < pEnd )
			{
				p_PreviousHash ^= static_cast< ZED_UINT32 >( *pStart++ );
				p_PreviousHash *= FNV32_PRIME;
			}

			return p_PreviousHash;
		}

		ZED_UINT32 HashStringFNV1a( ZED_CHAR8 *p_pString,
			ZED_UINT32 p_PreviousHash )
		{
			ZED_BYTE *pString = reinterpret_cast< ZED_BYTE * >( p_pString );

			// Keep going until the null terminator is found
			while( *pString )
			{
				p_PreviousHash ^= static_cast< ZED_UINT32 >( *pString++ );
				p_PreviousHash *= FNV32_PRIME;
			}

			return p_PreviousHash;
		}
	}
}

