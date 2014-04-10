#include <System/MemoryMappedFile.hpp>
#include <sys/mman.h>

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

		ZED_UINT32 MemoryMappedFile::SetFileToMap( const File &p_File )
		{
			m_FileDescriptor = p_File.GetFileDescriptor( );
			m_MappedFileSize = p_File.GetSize( );

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
				munmap( m_pFileAddress, m_Size );
				m_pFileAddress = ZED_NULL;
			}

			return ZED_OK;
		}
	}
}

