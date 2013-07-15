#include <System/ProcessorInformation.hpp>
#include <System/Debugger.hpp>
#include <cstring>

namespace ZED
{
	namespace System
	{
		ProcessorInformation::ProcessorInformation( )
		{
		}
		ProcessorInformation::~ProcessorInformation( )
		{
		}

		ZED_UINT32 ProcessorInformation::Initialise( )
		{
			if( m_Initialised )
			{
				return ZED_OK;
			}

			ZED_UINT32 AReg = 0, BReg = 0, CReg = 0, DReg = 0;
			// FIX!
			// Not currently working
/*
			// Get the vendor's name
			asm(
				"mov	$0,	%%eax\n"
				"cpuid\n"
				"mov	%%ebx,	%0\n"
				"mov	%%ecx,	%1\n"
				"mov	%%edx,	%2\n"
				:	"=b"( BReg ),
					"=c"( CReg ),
					"=d"( DReg )
				);/*:
				: "%eax", "%ebx", "%ecx", "%edx" );*

			// Extract the Vendor ID
			memset( m_pVendorID, '\0', 13 );

			m_pVendorID[ 0 ] = BReg;
			m_pVendorID[ 1 ] = BReg >> 8;
			m_pVendorID[ 2 ] = BReg >> 16;
			m_pVendorID[ 3 ] = BReg >> 24;

			m_pVendorID[ 4 ] = DReg;
			m_pVendorID[ 5 ] = DReg >> 8;
			m_pVendorID[ 6 ] = DReg >> 16;
			m_pVendorID[ 7 ] = DReg >> 24;

			m_pVendorID[ 8 ] = CReg;
			m_pVendorID[ 9 ] = CReg >> 8;
			m_pVendorID[ 10 ] = CReg >> 16;
			m_pVendorID[ 11 ] = CReg >> 24;

			zedTrace( "Vendor ID: %s\n", m_pVendorID );
*/
			// !FIX
			return ZED_OK;
		}
	}
}

