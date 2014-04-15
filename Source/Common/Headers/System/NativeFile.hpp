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
				const FILE_SEEK p_Start );
			virtual ZED_MEMSIZE GetPosition( ) const;
			virtual ZED_UINT32 Rewind( );

			// Write functions
			virtual ZED_UINT32 WriteByte( const ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			virtual ZED_UINT32 WriteUInt32( const ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			virtual ZED_UINT32 WriteString( const ZED_CHAR8 *p_pString,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );

			// Read functions
			virtual ZED_UINT32 ReadByte( ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );
			virtual ZED_UINT32 ReadUInt32( ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );
			virtual ZED_UINT32 ReadString( ZED_CHAR8 *p_pString,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );

		private:
#if defined ZED_PLATFORM_LINUX
			FILE		*m_pFile;
#elif defined ZED_PLATFORM_WINDOWS
			HANDLE		m_File;
#else
#error Unknown platform
#endif
		};
	}
}

#endif // __ZED_SYSTEM_NATIVEFILE_HPP__

