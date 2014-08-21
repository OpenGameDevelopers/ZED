#include <System/MemoryMappedFile.hpp>
#include <System/Debugger.hpp>
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
			m_MappedFileSize( 0 ),
			m_CurrentOffset( 0 ),
			m_OffsetType( OFFSET_UNKNOWN )
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
				zedTrace( "[ZED::System::MemoryMappedFile::SetFileToMap32] "
					"<ERROR> The offset + size requested is larger than the "
					"file currently being mapped\n"
					"\tRequested offset: %d\n"
					"\tRequested size:   %d\n"
					"\tCombined:         %d\n"
					"\tFile size:        %d\n",
					p_Offset, p_Size, p_Offset + p_Size, m_MappedFileSize );

				return ZED_FAIL;
			}

			m_Offset32 = p_Offset;
			m_Size = p_Size;
			m_OffsetType = OFFSET_32;

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
				zedTrace( "[ZED::System::MemoryMappedFile::SetFileToMap64] "
					"<ERROR> The offset + size requested is larger than the "
					"file currently being mapped\n"
					"\tRequested offset: %d\n"
					"\tRequested size:   %d\n"
					"\tCombined:         %d\n"
					"\tFile size:        %d\n",
					p_Offset, p_Size, p_Offset + p_Size, m_MappedFileSize );

				return ZED_FAIL;
			}

			m_Offset64 = p_Offset;
			m_Size = p_Size;
			m_OffsetType = OFFSET_64;

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::Open( const ZED_CHAR8 *p_pFileName,
			const ZED_UINT32 p_Access )
		{
			if( m_FileDescriptor == ZED_INVALID_FILE_DESCRIPTOR )
			{
				zedTrace( "[ZED::System::MemoryMappedFile::Open] <ERROR> "
					"File descriptor invalid\n" );

				return ZED_FAIL;
			}

			if( m_Size == 0 )
			{
				zedTrace( "[ZED::System::MemoryMappedFile::Open] <ERROR> "
					"Size is zero\n" );

				return ZED_FAIL;
			}

			if( m_Open )
			{
				this->Close( );
			}

			if( m_OffsetType == OFFSET_32 )
			{
				if( ( m_Offset32 + m_Size ) > m_MappedFileSize )
				{
					zedTrace( "[ZED::System::MemoryMappedFile::Open] <ERROR> "
						"The current offset + size go beyond the size of the "
						"file being mapped\n"
						"\tCurrent offset: %d\n"
						"\tCurrent size:   %d\n"
						"\tCombined:       %d\n"
						"\tFile size:      %d\n",
						m_Offset32, m_Size, m_Offset32 + m_Size,
						m_MappedFileSize );

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

				zedTrace( "m_Offset32: %d\n", m_Offset32 );

				m_pFileAddress = mmap( NULL, m_Size, PROT_READ, MAP_SHARED,
					m_FileDescriptor, m_Offset32 );

				if( m_pFileAddress == MAP_FAILED )
				{
					zedTrace( "[ZED::System::MemoryMappedFile::Open] <ERROR> "
						"Failed to map file\n" );

					return ZED_FAIL;
				}
			}
			else if( m_OffsetType == OFFSET_64 )
			{
				zedTrace( "TODO: Handle m_OffsetType in %s [%d]\n", __FILE__,
					__LINE__ );
				zedAssert( ZED_FALSE );
			}
			else
			{
				zedTrace( "[ZED::System::MemoryMappedFile::Open] <ERROR> "
					"SetFileToMap has not been called, either SetFileToMap32 "
					"or SetFileToMap64 must be called and succeed before "
					"calling ZED::System::MemoryMappedFile::Open\n" );

				return ZED_FAIL;
			}

			m_CurrentOffset = 0;

			zedTrace( "The first ten bytes of the memory-mapped file:\n" );

			zedTrace( "\t" );
			const char *pAddress =
				reinterpret_cast< const char * >( m_pFileAddress );
			for( ZED_MEMSIZE i = 0; i < 10; ++i )
			{
				zedTrace( "0x%02X ", pAddress[ i ] );
			}
			zedTrace( "\n" );

			return ZED_OK;
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
			const FILE_SEEK p_Start )
		{
			switch( p_Start )
			{
				case FILE_SEEK_SET:
				{
					if( p_Offset < 0 )
					{
						zedTrace( "[ZED::System::MemoryMappedFile::Seek] "
							"<ERROR> Seeking from the beginning of the file "
							"with an offset less than zero\n" );

						return ZED_FAIL;
					}

					if( p_Offset > m_Size )
					{
						zedTrace( "[ZED::System::MemoryMappedFile::Seek] "
							"<ERROR> Seeking from the beginning of the file "
							"with an offset greater than the file size\n" );

						return ZED_FAIL;
					}

					m_CurrentOffset = p_Offset;

					break;
				}
				case FILE_SEEK_CURRENT:
				{
					if( ( m_CurrentOffset + p_Offset ) < 0 )
					{
						zedTrace( "[ZED::System::MemoryMappedFile::Seek] "
							"<ERROR> Seeking from the current file pointer "
							"location [%d] with an offset of %d will go below "
							"the start of the file\n", m_CurrentOffset,
							p_Offset );

						return ZED_FAIL;
					}
					if( ( m_CurrentOffset + p_Offset ) > m_Size )
					{
						zedTrace( "[ZED::System::MemoryMappedFile::Seek] "
							"<ERROR> Seeking from the current file pointer "
							"location [%d] with an offset of %d will go past "
							"the end of the file\n", m_CurrentOffset,
							p_Offset );

						return ZED_FAIL;
					}

					m_CurrentOffset += p_Offset;

					break;
				}
				case FILE_SEEK_END:
				{
					if( p_Offset > 0 )
					{
						zedTrace( "[ZED::System::MemoryMappedFile::Seek] "
							"<ERROR> Seeking from the end, the offset is "
							"greater than zero\n" );

						return ZED_FAIL;
					}
					if( p_Offset < -m_Size )
					{
						zedTrace( "[ZED::System::MemoryMappedFile::Seek] "
							"<ERROR> Seeking from the end, the offset exceeds "
							"the size of the file\n" );

						return ZED_FAIL;
					}

					m_CurrentOffset = m_Size + p_Offset;

					break;
				}
				default:
				{
					zedTrace( "[ZED::System::MemoryMappedFile::Seek] <ERROR> "
						"An unknown starting location has been specified\n" );
					return ZED_FAIL;
				}
			}

			return ZED_OK;
		}

		ZED_MEMSIZE MemoryMappedFile::GetPosition( ) const
		{
			return m_CurrentOffset;
		}

		ZED_UINT32 MemoryMappedFile::Rewind( )
		{
			m_CurrentOffset = 0;

			return ZED_OK;
		}

		ZED_UINT32 MemoryMappedFile::WriteByte( const ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			zedTrace( "UNIMPLEMENTED FUNCTION: %s | %s [%d]\n",
				"ZED::System::MemoryMappedFile::WriteByte", __FILE__,
				__LINE__ );

			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::WriteUInt32( const ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			zedTrace( "UNIMPLEMENTED FUNCTION: %s | %s [%d]\n",
				"ZED::System::MemoryMappedFile::WriteUInt32", __FILE__,
				__LINE__ );

			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::WriteString( const ZED_CHAR8 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			zedTrace( "UNIMPLEMENTED FUNCTION: %s | %s [%d]\n",
				"ZED::System::MemoryMappedFile::WriteString", __FILE__,
				__LINE__ );

			return ZED_FAIL;
		}

		ZED_UINT32 MemoryMappedFile::ReadByte( ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( ( m_CurrentOffset + p_Count ) > m_Size )
			{
				zedTrace( "[ZED::System::MemoryMappedFile::ReadByte] <ERROR> "
					"The current operation would read past the current bounds "
					"allocated\n"
					"\tAttempting to access: %d, Maximum: %d\n",
					( m_CurrentOffset + p_Count ), m_Size );

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
				zedTrace( "[ZED::System::MemoryMappedFile::ReadUInt32] "
					"<ERROR> The current operation would read past the "
					"current bounds allocated\n"
					"\tAttempting to access: %d, Maximum: %d\n",
					( m_CurrentOffset + p_Count ),
					( m_Size * sizeof( ZED_UINT32 ) ) );

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
				zedTrace( "[ZED::System::MemoryMappedFile::ReadString] "
					"<ERROR> The current operation would read past the "
					"current bounds allocated\n"
					"\tAttempting to access: %d, Maximum: %d\n",
					( m_CurrentOffset + p_Count ), m_Size );

				return ZED_FAIL;
			}

			memcpy( p_pData, m_pFileAddress + m_CurrentOffset, p_Count );

			m_CurrentOffset += p_Count;

			return ZED_OK;
		}
	}
}

