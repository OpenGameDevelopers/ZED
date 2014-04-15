#ifndef __ZED_SYSTEM_MEMORYMAPPEDFILE_HPP__
#define __ZED_SYSTEM_MEMORYMAPPEDFILE_HPP__

#include <System/File.hpp>

namespace ZED
{
	namespace System
	{
		typedef enum __ZED_MMOFFSETTYPE
		{
			OFFSET_32 = 1,
			OFFSET_64,
			OFFSET_UNKNOWN
		}MMOFFSETTYPE;

		class MemoryMappedFile : public File
		{
		public:
			MemoryMappedFile( );
			virtual ~MemoryMappedFile( );

			ZED_SINT32 GetOffset32( ) const;
			ZED_SINT64 GetOffset64( ) const;

			ZED_UINT32 SetFileToMap32( const File &p_File,
				const ZED_SINT32 p_Offset, const ZED_MEMSIZE p_Size );
			ZED_UINT32 SetFileToMap64( const File &p_File,
				const ZED_SINT64 p_Offset, const ZED_MEMSIZE p_Size );

			virtual ZED_UINT32 Open( const ZED_CHAR8 *p_pFileName,
				const ZED_UINT32 p_Access );
			virtual ZED_UINT32 Close( );

			virtual ZED_UINT32 Seek( const ZED_MEMSIZE p_Offset,
				const FILE_SEEK p_Start );
			virtual ZED_MEMSIZE GetPosition( ) const;
			virtual ZED_UINT32 Rewind( );

			virtual ZED_UINT32 WriteByte( const ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			virtual ZED_UINT32 WriteUInt32( const ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			virtual ZED_UINT32 WriteString( const ZED_CHAR8 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			
			virtual ZED_UINT32 ReadByte( ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );
			virtual ZED_UINT32 ReadUInt32( ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );
			virtual ZED_UINT32 ReadString( ZED_CHAR8 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );

		private:
			MemoryMappedFile( const MemoryMappedFile &p_Copy );
			MemoryMappedFile &operator=( const MemoryMappedFile &p_Copy );

			void			*m_pFileAddress;
			ZED_SINT32		m_Offset32;
			ZED_SINT64		m_Offset64;
			ZED_MEMSIZE		m_MappedFileSize;
			ZED_MEMSIZE		m_CurrentOffset;
			MMOFFSETTYPE	m_OffsetType;
		};
	}
}

#endif // __ZED_SYSTEM_MEMORYMAPPEDFILE_HPP__

