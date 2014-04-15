#ifndef __ZED_SYSTEM_FILE_HPP__
#define __ZED_SYSTEM_FILE_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_UINT32 FILE_ACCESS_READ	= 0x00000001;
		const ZED_UINT32 FILE_ACCESS_WRITE	= 0x00000002;
		const ZED_UINT32 FILE_ACCESS_APPEND	= 0x00000004;
		const ZED_UINT32 FILE_ACCESS_UPDATE	= 0x00000008;
		const ZED_UINT32 FILE_ACCESS_BINARY	= 0x00000010;
		const ZED_UINT32 FILE_ACCESS_TEXT	= 0x00000020;

		typedef enum __FILE_SEEK
		{
			FILE_SEEK_SET,
			FILE_SEEK_CURRENT,
			FILE_SEEK_END
		}FILE_SEEK;

		// The first parameter contains the absolute path to the executable
		// if this function succeeds
		ZED_UINT32 GetExecutablePath( char **p_ppBuffer, ZED_MEMSIZE p_Size );

		ZED_BOOL FileExists( const ZED_CHAR8 *p_pFilePath,
			const ZED_BOOL p_IncludeSymLinks );

		ZED_BOOL DirectoryExists( const ZED_CHAR8 *p_pDirectory );

		class File
		{
		public:
			File( );
			virtual ~File( );

			virtual ZED_UINT32 Open( const ZED_CHAR8 *p_pFileName,
				const ZED_UINT32 p_Access ) = 0;
			virtual ZED_UINT32 Close( ) = 0;

			// Position functions
			virtual ZED_UINT32 Seek( const ZED_MEMSIZE p_Offset,
				const FILE_SEEK p_Start ) = 0;
			virtual ZED_MEMSIZE GetPosition( ) const = 0;
			virtual ZED_UINT32 Rewind( ) = 0;

			// Write functions
			virtual ZED_UINT32 WriteByte( const ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten ) = 0;
			virtual ZED_UINT32 WriteUInt32( const ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten ) = 0;
			virtual ZED_UINT32 WriteString( const ZED_CHAR8 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten ) = 0;

			// Read functions
			virtual ZED_UINT32 ReadByte( ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead ) = 0;
			virtual ZED_UINT32 ReadUInt32( ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead ) = 0;
			virtual ZED_UINT32 ReadString( ZED_CHAR8 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten ) = 0;

			ZED_MEMSIZE GetSize( ) const;
			ZED_BOOL IsOpen( ) const;
			ZED_FILE_DESCRIPTOR GetFileDescriptor( ) const;
			ZED_UINT32 GetFileAccess( ) const;

		protected:
			ZED_BOOL			m_Open;
			ZED_MEMSIZE			m_Size;
			ZED_FILE_DESCRIPTOR	m_FileDescriptor;
			ZED_UINT32			m_FileAccess;
		};
	}
}

#endif // __ZED_SYSTEM_FILE_HPP__

