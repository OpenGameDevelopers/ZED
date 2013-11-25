#include <System/NativeFile.hpp>
#include <System/Debugger.hpp>
#include <cstring>
#include <errno.h>

namespace ZED
{
	namespace System
	{
		NativeFile::NativeFile( )
		{
			m_pFile = ZED_NULL;
		}

		NativeFile::~NativeFile( )
		{
			this->Close( );
		}

		ZED_UINT32 NativeFile::Open( const ZED_CHAR8 *p_pFileName,
			const ZED_UINT32 p_Access )
		{
			if( m_pFile )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
			}

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

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::Close( )
		{
			if( m_pFile )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
			}

			return ZED_OK;
		}

		ZED_UINT32 NativeFile::Seek( const ZED_MEMSIZE p_Offset,
			const ZED_UINT32 p_Origin )
		{
			return ZED_OK;
		}

		ZED_UINT32 NativeFile::Rewind( )
		{
			return ZED_OK;
		}

		ZED_UINT32 NativeFile::WriteByte( const ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Length, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_OK;
		}

		ZED_UINT32 NativeFile::ReadByte( ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Length, ZED_MEMSIZE *p_pRead )
		{
			fread( p_pData, p_Length, ( *p_pRead ), m_pFile );

			if( p_Length != ( *p_pRead ) )
			{
				zedTrace( "[ZED::System::NativeFile::ReadByte] <WARNING> "
					"Length read in does not match requested length\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}
	}
}

