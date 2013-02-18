#include <HardwareInformation.hpp>
#include <Windows.h>
#include <cstring>
#include <Debugger.hpp>

namespace ZED
{
	namespace System
	{
#if ZED_PLATFORM_WIN32_X86
		// TEMP!
		/*
		ZED_UINT32 HardwareInformation::GetProcessorInformation( 
			ZED_PROCINFO *p_pProcInfo )
		{
			// Just in case pProcInfo gets full of invalid data
			ZED_PROCINFO ProcInfo;

			memset( &ProcInfo, 0, sizeof( ZED_PROCINFO ) );

			// Test for CPUID support
			__try
			{
				__asm
				{
					XOR	EAX, EAX;
					CPUID;
				}
			}
			__except( EXCEPTION_EXECUTE_HANDLER )
			{
				return ZED_FAIL;
			}

			// Temporary variables for CPUID extraction
			ZED_CHAR8 *pVendorName = ProcInfo.VendorName;
			// Step 1:  Map the vendor name, and extract the feature bits
			__try
			{
				__asm
				{
					// CPID returns the vendor name
					MOV EAX, 0;
					CPUID;

					// Extract the name in sets of four chars
					MOV ESI, pVendorName;
					MOV [ESI], EBX;
					MOV [ESI+4], EDX;
					MOV [ESI+8], ECX;

					// Get the feature bits (Going from most to least recent)
					MOV EAX, 1;
					CPUID;
					
					TEST EDX, ZED_CPUSSE2;
					JZ _NOSSE2;
					MOV [ ProcInfo.pSIMDSupported+1 ], ZED_TRUE;
_NOSSE2:
					TEST EDX, ZED_CPUSSE;
					JZ _NOSSE;
					MOV [ ProcInfo.pSIMDSupported ], ZED_TRUE;
_NOSSE:
					TEST EDX, ZED_CPUMMX;
					JZ _EXIT1;
					MOV [ ProcInfo.pSIMDSupported+4 ], ZED_TRUE;
_EXIT1:
					// Finished
				}
			}
			__except( EXCEPTION_EXECUTE_HANDLER )
			{
				if( _exception_code( ) == STATUS_ILLEGAL_INSTRUCTION )
				{
					zedTrace( "CPUID failed, illegal instruction encountered" );
					return ZED_FAIL;
				}
				zedTrace( "CPUID, unknown error encountered" );
				return ZED_FAIL;
			}

			// Step 2: Check for extended information
			__asm
			{
				// CPUID will return the extended information
				MOV	EAX, 0x80000000;
				CPUID;
				CMP EAX, 0x80000000;
				// Must be greater than 0x80000000, not equal or less
				JBE _EXIT2;
				MOV [ ProcInfo.ExtendedFeatures ], ZED_TRUE;
				
				// Copy the feature bits to EDX
				MOV EAX, 0x80000001;
				CPUID;
				// 3D Now! support
				TEST EDX, 0x8000000;
				JZ _EXIT2;
				MOV [ ProcInfo.pSIMDSupported+6 ], ZED_TRUE;
_EXIT2:
				// End of the line
			}

			ZED_SINT32 Family;
			ZED_SINT32 *pFamily = &Family;
			// Retrieve the vendor specific information
			if( ( strncmp( reinterpret_cast< const char *>( pVendorName ),
				"GenuineIntel", 12 ) == 0 ) &&
				ProcInfo.ExtendedFeatures )
			{
				__asm
				{
					// Copy extended feature bits
					MOV EAX, 1;
					CPUID;
					// Get family
					MOV ESI, pFamily;
				}

				// Only the lowest eight bits are needed for th ID
				ZED_SINT32 ID = 0;
				memcpy( &ID, pFamily, sizeof( ZED_CHAR8 ) );
				Family = ID;
			}

			return ZED_OK;
		}

		ZED_UINT32 HardwareInformation::OSSupportSIMD( ZED_SINT32 p_Feature )
		{
			__try
			{
				// Execute SIMD instructions specific to each SIMD type
				switch( p_Feature )
				{
				case ZED_CPUFEATURE_SSE:
					{
						__asm
						{
							XORPS XMM0, XMM0;
						}
						break;
					}
				case ZED_CPUFEATURE_SSE2:
					{
						__asm
						{
							XORPD XMM0, XMM0;
						}
						break;
					}
				case ZED_CPUFEATURE_3DNOW:
					{
						__asm
						{
							PFRCP MM0, MM0;
							EMMS;
						}
						break;
					}
				case ZED_CPUFEATURE_MMX:
					{
						__asm
						{
							PXOR MM0, MM0;
							EMMS;
						}
						break;
					}
				default:
					{
						zedTrace( "SIMD feature not recognised" );
						return ZED_FAIL;
					}
				}
			}
			__except( EXCEPTION_EXECUTE_HANDLER )
			{
				if( _exception_code( ) == STATUS_ILLEGAL_INSTRUCTION )
				{
					zedTrace( "OS couldn't perform SIMD opertaton" );
					return ZED_FAIL;
				}
				zedTrace( "OS failed SIMD operation" );
				return ZED_FAIL;
			}
			return ZED_OK;
		}*/
		// !TEMP
#endif
	}
}
