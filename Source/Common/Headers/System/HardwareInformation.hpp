#ifndef __ZED_SYSTEM_HARDWAREINFORMATION_HPP__
#define __ZED_SYSTEM_HARDWAREINFORMATION_HPP__

// TEMP!
#define	USESTLSTRING
// !TEMP
#include <System/DataTypes.hpp>
/*
// Define the SIMD feature flags
const ZED_UINT32 ZED_CPUMMX =	0x00800000;
const ZED_UINT32 ZED_CPUSSE =	0x02000000;
const ZED_UINT32 ZED_CPUSSE2 =	0x04000000;

// AMD-specific
const ZED_UINT32 ZED_CPU3DNOW =	0x80000000;
const ZED_UINT32 ZED_CPUMMXEX =	0x00400000;

// For execution of SIMD operations
const ZED_UINT32 ZED_CPUFEATURE_SSE	=		0x00000001;
const ZED_UINT32 ZED_CPUFEATURE_SSE2 =		0x00000002;
const ZED_UINT32 ZED_CPUFEATURE_SSE3 =		0x00000004;
const ZED_UINT32 ZED_CPUFEATURE_SSE4 =		0x00000008;
const ZED_UINT32 ZED_CPUFEATURE_MMX =		0x00000010;
const ZED_UINT32 ZED_CPUFEATURE_MMXEX =		0x00000020;
const ZED_UINT32 ZED_CPUFEATURE_3DNOW =		0x00000040;
const ZED_UINT32 ZED_CPUFEATURE_3DNOWEX =	0x00000080;*/

// Define the structures for the hardware information
typedef struct _ZED_PROCINFO
{
	ZED_CHAR8	VendorName[ 13 ];
	ZED_CHAR8	ModelName[ 30 ];
	ZED_UINT64	Frequency;
	ZED_SINT32	Family;
	ZED_SINT32	Model;
	ZED_SINT32	Stepping;
	ZED_SINT32	Feature;
	ZED_SINT32	CheckMask;
	ZED_UINT32	Count;
	ZED_UINT32	CoreCount;
	ZED_UINT32	VirtualCoreCount;
	ZED_UINT32	L1Cache;
	ZED_UINT32	L2Cache;
	ZED_UINT32	L3Cache;
	ZED_BOOL	Endianess;
	// Does the OS support SIMD features?
	ZED_BOOL	OSSIMD;
	ZED_BOOL	ExtendedFeatures;
} ZED_PROCINFO;

typedef struct _ZED_MEMINFO
{
	ZED_UINT64	m_TotalMemory;
	ZED_UINT32	m_AccessSpeed;
} ZED_MEMINFO;

typedef struct _ZED_DRIVEINFO
{
	ZED_UINT32	HDDReadTime;
	ZED_UINT32	HDDSeekTime;
	ZED_UINT32	OpticalDriveReadTime;
	ZED_UINT32	OpticalDriveSeekTime;
} ZED_DRIVEINFO;

namespace ZED
{
	namespace System
	{
		class HardwareInformation
		{
		public:
			HardwareInformation( );
			~HardwareInformation( );

			ZED_UINT32 GetProcessorInformation( ZED_PROCINFO *p_pProcInfo );
			ZED_UINT32 GetMemoryInformation( ZED_MEMINFO *p_pMemInfo );
			ZED_UINT32 GetDriveInformation( ZED_DRIVEINFO *p_pDriveInfo );
			ZED_STRING GetOS( );

		private:
			// Processor-related
			ZED_UINT32	m_ProcessorCount;
			ZED_UINT32	m_ProcessorCoreCount;
			ZED_UINT32	m_ProcessorVirtualCoreCount;
			ZED_BOOL	m_Endianess;
			ZED_UINT64	m_ProcessorSpeed;
			ZED_UINT32	m_L1Cache;
			ZED_UINT32	m_L2Cache;
			ZED_UINT32	m_L3Cache;
			// SSE, SSE2, SSE3, SSE4, MMX, MMXEX, 3DNOW, 3DNOWEX
			ZED_BOOL	m_pSIMDSupported[ 8 ];
			ZED_STRING	m_ProcessorName;

			// Memory-related
			ZED_UINT64	m_TotalMemory;
			ZED_UINT32	m_MemoryAccessSpeed;

			// Platform-specific
			ZED_STRING	m_OS;

			// Disk-related
			ZED_BOOL	m_HasHDD;
			ZED_UINT32	m_HDDReadTime;
			ZED_UINT32	m_HDDSeekTime;
			ZED_BOOL	m_HasOpticalDrive;
			ZED_UINT32	m_OpticalDriveReadTime;
			ZED_UINT32	m_OpticalDriveSeekTime;

			// Functions that aren't necessary for outside use
			ZED_UINT32 OSSupportSIMD( ZED_SINT32 p_Feature );
		};
	}
}

#endif
