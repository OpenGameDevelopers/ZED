#ifndef __ZED_SYSTEM_PROCESSORINFORMATION_HPP__
#define __ZED_SYSTEM_PROCESSORINFORMATION_HPP__

#include <DataTypes.hpp>

// Define all SIMD flags
const ZED_UINT32 ZED_MMX	= 0x00800000;
const ZED_UINT32 ZED_SSE	= 0x02000000;
const ZED_UINT32 ZED_SSE2	= 0x04000000;

// AMD-specific SIMD flags
const ZED_UINT32 ZED_3DNOW	= 0x80000000;
const ZED_UINT32 ZED_MMXEX	= 0x00400000;

namespace ZED
{
	namespace System
	{
		class ProcessorInformation
		{
		public:
			ProcessorInformation( );
			~ProcessorInformation( );

			ZED_UINT32 Initialise( );

		private:
			ZED_CHAR8	m_pVendorID[ 13 ];
			ZED_CHAR8	m_pModelName[ 50 ];
			ZED_UINT64	m_Frequency;
			ZED_UINT32	m_Family;
			ZED_UINT32	m_Model;
			ZED_UINT32	m_Stepping;
			ZED_UINT32	m_FeatureSet1;
			ZED_UINT32	m_FeatureSet2;
			ZED_BOOL	m_Initialised;
		};
	}
}

#endif

