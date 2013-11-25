#ifndef __ZED_SYSTEM_NATIVEFILE_HPP__
#define __ZED_SYSTEM_NATIVEFILE_HPP__

#include <System/File.hpp>
#include <cstdio>

namespace ZED
{
	namespace System
	{
		class NativeFile : public File
		{
		public:
			NativeFile( );
			virtual ~NativeFile( );

			// File reference functions
			virtual ZED_UINT32 Open( const ZED_CHAR8 *p_pFileName,
				const ZED_UINT32 p_Access );
			virtual ZED_UINT32 Close( );

			// Position functions
			virtual ZED_UINT32 Seek( const ZED_MEMSIZE p_Offset,
				const ZED_UINT32 p_Origin );
			virtual ZED_UINT32 Rewind( );

			// Read functions
			virtual ZED_UINT32 WriteByte( const ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Length, ZED_MEMSIZE *p_pWritten );

			// Write functions
			virtual ZED_UINT32 ReadByte( ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Length, ZED_MEMSIZE *p_pRead );

		private:
#if defined ZED_PLATFORM_LINUX
			FILE		*m_pFile;
#elif defined ZED_PLATFORM_WINDOWS
			HANDLE		m_File;
#else
#error Unknown platform
#endif
			ZED_MEMSIZE	m_Size;
		};
	}
}

#endif

