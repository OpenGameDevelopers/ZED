#include <System/NativeFile.hpp>
#include <System/Debugger.hpp>
#include <cstring>
#include <errno.h>

namespace ZED
{
	namespace System
	{
		NativeFile::NativeFile( ) :
			m_pFile( ZED_NULL )
		{
		}

		NativeFile::~NativeFile( )
		{
			this->Close( );
		}

		ZED_UINT32 NativeFile::Open( const ZED_CHAR8 *p_pFileName,
			const ZED_UINT32 p_Access )
		{
			this->Close( );

			char Access[ 8 ] = { '\0' };
			ZED_MEMSIZE AccessPosition = 0;

			if( p_Access & FILE_ACCESS_READ )
			{
				if( p_Access & FILE_ACCESS_WRITE )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine read and write access\n" );

					return ZED_FAIL;
				}
				if( p_Access & FILE_ACCESS_APPEND )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine read and append access\n" );

					return ZED_FAIL;
				}

				Access[ AccessPosition ] = 'r';
				++AccessPosition;
			}
			if( p_Access & FILE_ACCESS_WRITE )
			{
				if( p_Access & FILE_ACCESS_READ )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine write and read access\n" );

					return ZED_FAIL;
				}
				if( p_Access & FILE_ACCESS_APPEND )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine write and append access\n" );

					return ZED_FAIL;
				}

				Access[ AccessPosition ] = 'w';
				++AccessPosition;
			}
			if( p_Access & FILE_ACCESS_APPEND )
			{
				if( p_Access & FILE_ACCESS_READ )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine append and read access\n" );

					return ZED_FAIL;
				}
				if( p_Access & FILE_ACCESS_WRITE )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine append and write access\n" );

					return ZED_FAIL;
				}

				Access[ AccessPosition ] = 'a';
				++AccessPosition;
			}
			if( p_Access & FILE_ACCESS_UPDATE )
			{
				Access[ AccessPosition ] = '+';
				++AccessPosition;
			}
			if( p_Access & FILE_ACCESS_BINARY )
			{
				if( p_Access & FILE_ACCESS_TEXT )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine binary and text access\n" );

					return ZED_FAIL;
				}

				Access[ AccessPosition ] = 'b';
				++AccessPosition;
			}
			if( p_Access & FILE_ACCESS_TEXT )
			{
				if( p_Access & FILE_ACCESS_BINARY )
				{
					zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
						"Cannot combine text and binary access\n" );

					return ZED_FAIL;
				}

				Access[ AccessPosition ] = 't';
				++AccessPosition;
			}

			if( Access[ 0 ] == '\0' )
			{
				zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
					"Access information not present\n" );

				return ZED_FAIL;
			}
			
			m_pFile = fopen( p_pFileName, Access );

			if( m_pFile == ZED_NULL )
			{
				zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
					"Could not open file: %s\n", strerror( errno ) );

				return ZED_FAIL;
			}

			fseek( m_pFile, 0, SEEK_END );
			m_Size = ftell( m_pFile );
			rewind( m_pFile );

			m_FileDescriptor = fileno( m_pFile );

			if( m_FileDescriptor == ZED_INVALID_FILE_DESCRIPTOR )
			{
				this->Close( );

				switch( errno )
				{
					case EBADF:
					{
						zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
							"Could not obtain file descriptor, file stream "
							"invalid\n" );

						break;
					}
					default:
					{
						zedTrace( "[ZED::System::NativeFile::Open] <ERROR> "
							"Unknown error occured while attempting to get "
							"the file descriptor from the file stream\n" );

						break;
					}
				}

				return ZED_FAIL;
			}

			m_Open = ZED_TRUE;

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::Close( )
		{
			if( m_pFile )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
				m_Open = ZED_FALSE;
				m_FileDescriptor = ZED_INVALID_FILE_DESCRIPTOR;
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::Seek( const ZED_MEMSIZE p_Offset,
			const FILE_SEEK p_Start )
		{
			int ErrNo = 0;

			switch( p_Start )
			{
				case FILE_SEEK_SET:
				{
					if( fseek( m_pFile, p_Offset, SEEK_SET ) == -1 )
					{
						zedTrace( "[ZED::System::NativeFile::Seek] <ERROR> "
							"Seeking from the start of the file failed\n" );
						ErrNo = errno;
					}
					break;
				}
				case FILE_SEEK_CURRENT:
				{
					if( fseek( m_pFile, p_Offset, SEEK_CUR ) == -1 )
					{
						zedTrace( "[ZED::System::NativeFile::Seek] <ERROR> "
							"Seeking from the current position in the file "
							"failed\n" );
						ErrNo = errno;
					}
					break;
				}
				case FILE_SEEK_END:
				{
					if( fseek( m_pFile, p_Offset, SEEK_END ) == -1 )
					{
						zedTrace( "[ZED::System::NativeFile::Seek] <ERROR> "
							"Seeking from the end of the file failed\n" );
						ErrNo = errno;
					}
					break;
				}
				default:
				{
					zedTrace( "[ZED::System::NativeFile::Seek] <ERROR> "
						"Unknown starting position: %d\n", p_Start );

					return ZED_FAIL;
				}
			}

			if( ErrNo )
			{
				switch( ErrNo )
				{
					case EBADF:
					{
						zedTrace( "\tThe stream is not a seekable "
							"stream\n" );
						break;
					}
					case EINVAL:
					{
						zedTrace( "\tThe whence argument passed to "
							"fseek was not valid\n" );
						break;
					}
					default:
					{
						zedTrace( "\tUnknown error\n" );
						break;
					}
				}

				return ZED_FAIL;
			}
			
			return ZED_OK;
		}

		ZED_MEMSIZE NativeFile::GetPosition( ) const
		{
			return ftell( m_pFile );
		}

		ZED_UINT32 NativeFile::Rewind( )
		{
			rewind( m_pFile );

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::WriteByte( const ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			if( !m_Open )
			{
				zedTrace( "[ZED::System::NativeFile::WriteByte] <ERROR> "
					"File is no longer open or valid\n" );

				return ZED_FAIL;
			}

			if( p_pWritten )
			{
				( *p_pWritten ) = fwrite( p_pData, sizeof( ZED_BYTE ), p_Count,
					m_pFile );

				if( p_Count != ( *p_pWritten ) )
				{
					zedTrace( "[ZED::System::NativeFile::WriteByte] <WARNING> "
						"Length written to media does not match requested "
						"length | Wrote %d bytes instead of %d bytes\n",
						( *p_pWritten ), p_Count );

					return ZED_FAIL;
				}
				}
			else
			{
				fwrite( p_pData, sizeof( ZED_BYTE ), p_Count, m_pFile );
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::WriteUInt32( const ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			if( !m_Open )
			{
				zedTrace( "[ZED::System::NativeFile::WriteUInt32] <ERROR> "
					"File is no longer open or valid\n" );

				return ZED_FAIL;
			}

			if( p_pWritten )
			{
				( *p_pWritten ) = fwrite( p_pData, sizeof( ZED_UINT32 ),
					p_Count, m_pFile );

				if( p_Count != ( *p_pWritten ) )
				{
					zedTrace( "[ZED::System::NativeFile::WriteUInt32] "
						"<WARNING> Length written to media does not match "
						"requested length | Wrote %d uint32s instead of %d "
						"uint32s\n", ( *p_pWritten ), p_Count );

					return ZED_FAIL;
				}
			}
			else
			{
				fwrite( p_pData, sizeof( ZED_UINT32 ), p_Count, m_pFile );
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::WriteString( const ZED_CHAR8 *p_pString,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			if( !m_Open )
			{
				zedTrace( "[ZED::System::NativeFile::WriteString] <ERROR> "
					"File is no longer open or valid\n" );

				return ZED_FAIL;
			}

			if( p_pWritten )
			{
				( *p_pWritten ) = fwrite( p_pString, sizeof( ZED_CHAR8 ),
					p_Count, m_pFile );

				if( p_Count != ( *p_pWritten ) )
				{
					zedTrace( "[ZED::System::NativeFile::WriteByte] <WARNING> "
						"Length written to media does not match requested "
						"length | Wrote %d characters instead of %d "
						"characters\n", ( *p_pWritten ), p_Count );

					return ZED_FAIL;
				}
			}
			else
			{
				fwrite( p_pString, sizeof( ZED_CHAR8 ), p_Count, m_pFile );
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::ReadByte( ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( !m_Open )
			{
				zedTrace( "[ZED::System::NativeFile::ReadByte] <ERROR> "
					"File is no longer open or valid\n" );

				return ZED_FAIL;
			}

			if( p_pRead )
			{
				( *p_pRead ) = fread( p_pData, sizeof( ZED_BYTE ), p_Count,
					m_pFile );

				if( p_Count != ( *p_pRead ) )
				{
					zedTrace( "[ZED::System::NativeFile::ReadByte] <WARNING> "
						"Length read in does not match requested length | Read"
						" %d bytes instead of %d bytes\n", ( *p_pRead ),
						p_Count );

					return ZED_FAIL;
				}
			}
			else
			{
				fread( p_pData, sizeof( ZED_BYTE ), p_Count, m_pFile );
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::ReadUInt32( ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( !m_Open )
			{
				zedTrace( "[ZED::System::NativeFile::ReadUInt32] <ERROR> "
					"File is no longer open or valid\n" );

				return ZED_FAIL;
			}

			if( p_pRead )
			{
				( *p_pRead ) = fread( p_pData, sizeof( ZED_UINT32 ), p_Count,
					m_pFile );

				if( p_Count != ( *p_pRead ) )
				{
					zedTrace( "[ZED::Ssytem::NativeFile::ReadUInt32] <WARNING>"
						" Length read in does not match requested length |"
						" Read %d uint32s instead of %d uint32s\n",
						( *p_pRead ), p_Count );

					return ZED_FAIL;
				}
			}
			else
			{
				fread( p_pData, sizeof( ZED_UINT32 ), p_Count, m_pFile );
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::ReadString( ZED_CHAR8 *p_pString,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			if( !m_Open )
			{
				zedTrace( "[ZED::System::NativeFile::ReadString] <ERROR> "
					"File is no longer open or valid\n" );

				return ZED_FAIL;
			}

			if( p_pRead )
			{
				( *p_pRead ) = fread( p_pString, sizeof( ZED_CHAR8 ), p_Count,
					m_pFile );

				if( p_Count != ( *p_pRead ) )
				{
					zedTrace( "[ZED::System::NativeFile::ReadByte] <WARNING> "
						"Length read in does not match requested length | "
						"Read %d characters instead of %d characters\n",
						( *p_pRead ), p_Count );

					return ZED_FAIL;
				}
			}
			else
			{
				fread( p_pString, sizeof( ZED_CHAR8 ), p_Count, m_pFile );
			}

			return ZED_OK;
		}
	}
}

