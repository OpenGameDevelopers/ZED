#include <System/MemoryMappedFile.hpp>
#include <sys/mman.h>
#include <cstring>

namespace ZED
{
	namespace System
	{
		MemoryMappedFile::MemoryMappedFile( ) :
			File( ),
			m_pFileAddress( ZED_NULL ),
			m_Offset32( 0U ),
			m_Offset64( 0ULL ),
			m_MappedFileSize( 0 )
		{
		}

		MemoryMappedFile::~MemoryMappedFile( )
		{
			this->Close( );
		}

		ZED_SINT32 MemoryMappedFile::GetOffset32( ) const
		{
			return m_Offset32;
		}

		ZED_SINT64 MemoryMappedFile::GetOffset64( ) const
		{
			return m_Offset64;
		}

		ZED_UINT32 MemoryMappedFile::SetFileToMap32( const File &p_File,
			const ZED_SINT32 p_Offset, const ZED_MEMSIZE p_Size )
		{
			m_FileDescriptor = p_File.GetFileDescriptor( );
			m_MappedFileSize = p_File.GetSize( );
			m_FileAccess = p_File.GetFileAccess( );

			if( ( p_Size + p_Offset ) > m_MappedFileSize )
			{
				return ZED_FAIL;
			}

			m_Offset32 = p_Offset;
			m_Size = p_Size;

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::SetFileToMap64( const File &p_File,
			const ZED_SINT64 p_Offset, const ZED_MEMSIZE p_Size )
		{
			m_FileDescriptor = p_File.GetFileDescriptor( );
			m_MappedFileSize = p_File.GetSize( );
			m_FileAccess = p_File.GetFileAccess( );

			if( ( p_Size + p_Offset ) > m_MappedFileSize )
			{
				return ZED_FAIL;
			}

			m_Offset64 = p_Offset;
			m_Size = p_Size;

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::Open( const ZED_CHAR8 *p_pFileName,
			const ZED_UINT32 p_Access )
		{
			if( m_FileDescriptor == ZED_INVALID_FILE_DESCRIPTOR )
			{
				return ZED_FAIL;
			}

			if( m_Size == 0 )
			{
				return ZED_FAIL;
			}

			if( ( m_Offset32 != 0U ) && ( m_Offset64 != 0 ) )
			{
				return ZED_FAIL;
			}

			if( m_Open )
			{
				this->Close( );
			}

			if( m_Offset32 )
			{
				if( ( m_Offset32 + m_Size ) > m_MappedFileSize )
				{
					return ZED_FAIL;
				}

				int Protection = 0;
				int Flags = 0;

				if( m_FileAccess & FILE_ACCESS_READ )
				{
					Protection |= PROT_READ;
					Flags = MAP_PRIVATE;
				}
				if( m_FileAccess & FILE_ACCESS_WRITE )
				{
					Protection |= PROT_WRITE;
					Flags = MAP_SHARED;
				}

				// Allow for updating the file
				if( ( m_FileAccess & FILE_ACCESS_READ ) &&
					( m_FileAccess & FILE_ACCESS_WRITE ) )
				{
					Flags = MAP_SHARED;
				}

				m_pFileAddress = mmap( NULL, m_Size, PROT_READ, MAP_SHARED,
					m_FileDescriptor, m_Offset32 );
			}
			else if( m_Offset64 )
			{
			}
			else
			{
				return ZED_FAIL;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::Close( )
		{
			if( m_pFileAddress )
			{
				if( m_FileAccess & FILE_ACCESS_WRITE )
				{
					msync( m_pFileAddress, m_Size, MS_SYNC );
				}

				munmap( m_pFileAddress, m_Size );
				m_pFileAddress = ZED_NULL;
			}

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::Seek( const ZED_MEMSIZE p_Offset,
			const ZED_UINT32 p_Ahead )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::Rewind( )
		{
			m_CurrentOffset = 0;

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::WriteByte( const ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::WriteUInt32( const ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::WriteString( const ZED_CHAR8 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::ReadByte( ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( ( m_CurrentOffset + p_Count ) > m_Size )
			{
				return ZED_FAIL;
			}

			memcpy( p_pData, m_pFileAddress + m_CurrentOffset, p_Count );

			m_CurrentOffset += p_Count;

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::ReadUInt32( ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( ( m_CurrentOffset + ( p_Count * sizeof( ZED_UINT32 ) ) ) >
				m_Size )
			{
				return ZED_FAIL;
			}

			memcpy( p_pData, m_pFileAddress + m_CurrentOffset,
				p_Count * sizeof( ZED_UINT32 ) );
			
			m_CurrentOffset += p_Count * sizeof( ZED_UINT32 );

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::ReadString( ZED_CHAR8 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( ( m_CurrentOffset + p_Count ) > m_Size )
			{
				return ZED_FAIL;
			}

			memcpy( p_pData, m_pFileAddress + m_CurrentOffset, p_Count );

			m_CurrentOffset += p_Count;

			return ZED_OK;
		}
	}
}

