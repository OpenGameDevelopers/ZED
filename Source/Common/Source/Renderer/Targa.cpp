#include <Renderer/Targa.hpp>
#include <System/Memory.hpp>
#include <System/Debugger.hpp>
#include <cstdio>

namespace ZED
{
	namespace Renderer
	{
		Targa::Targa( ) :
			m_pData( ZED_NULL ),
			m_Width( 0U ),
			m_Height( 0U ),
			m_Format( ZED_FORMAT_UNDEFINED )
		{
		}

		Targa::~Targa( )
		{
			zedSafeDeleteArray( m_pData );
		}

		ZED_UINT32 Targa::Load( const ZED_CHAR8 *p_pFileName )
		{
			// FIXME
			// This is not optimised at all for unified memory architectures,
			// such as the Xbox, which will need to load the texture into an
			// area of RAM for texture memory

			FILE *pFile = ZED_NULL;
			pFile = fopen( p_pFileName, "rb" );

			if( pFile == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::Targa::Load] <ERROR> Failed to "
					"load file \"%s\"\n", p_pFileName );

				return ZED_FAIL;
			}

			TARGA_HEADER TargaHeader;

			ZED_MEMSIZE FileLength = 0;
			ZED_MEMSIZE TotalFileSize = 0;

			fseek( pFile, 0L, SEEK_END );
			TotalFileSize = ftell( pFile );

			if( TotalFileSize < sizeof( TARGA_HEADER ) )
			{
				if( pFile )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}

				zedTrace( "[ZED::Renderer::Targa::Load] <ERROR> File size "
					"does not exceed the header size\n" );

				return ZED_FAIL;
			}

			rewind( pFile );

			FileLength = fread( &TargaHeader, sizeof( TargaHeader ), 1,
				pFile );

			if( FileLength == 0 )
			{
				if( pFile )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}

				zedTrace( "[ZED::Renderer::Targa::Load] <ERROR> Could not "
					"read the TARGA header\n" );

				return ZED_FAIL;
			}

			fseek( pFile, TargaHeader.IDLength, SEEK_SET );

			ZED_MEMSIZE DataBytesInFile = 0;
			ZED_MEMSIZE ExpectedImageSize =
				TargaHeader.Width * TargaHeader.Height *
				( TargaHeader.BitsPerPixel / 8 );

			if( ( TotalFileSize - sizeof( TargaHeader ) -
				TargaHeader.IDLength )
					< ExpectedImageSize )
			{
				if( pFile )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}

				zedTrace( "[ZED::Renderer::Targa::Load] <ERROR> Image data "
					"in the file is not of the expected size "
					"[expected %d, got %d]\n", ExpectedImageSize,
					TotalFileSize - sizeof( TARGA_HEADER ) -
						TargaHeader.IDLength );

				return ZED_FAIL;
			}

			m_pData = new ZED_BYTE[ ExpectedImageSize ];

			fread( m_pData, sizeof( ZED_BYTE ), ExpectedImageSize, pFile );

			if( pFile )
			{
				fclose( pFile );
				pFile = ZED_NULL;
			}

			m_Width = TargaHeader.Width;
			m_Height = TargaHeader.Height;

			if( TargaHeader.ImageType != 0x02 )
			{
				zedTrace( "[ZED::Renderer::Targa::Load] <ERROR> Only "
					"uncompressed true colour images are supported at this "
					"time\n" );
				return ZED_FAIL;
			}

			switch( TargaHeader.BitsPerPixel )
			{
				case 16:
				{
					m_Format = ZED_FORMAT_ARGB1555;
					break;
				}
				case 24:
				{
					m_Format = ZED_FORMAT_RGB8;
					break;
				}
				case 32:
				{
					m_Format = ZED_FORMAT_ARGB8;
					break;
				}
				default:
				{
					zedTrace( "[ZED::Renderer::Targa::Load] <ERROR> Bit size "
						"for image incorrect: expected either 16-bit, 24-bit, "
						"or 32-bit; received %d-bit\n",
						TargaHeader.BitsPerPixel );

					return ZED_FAIL;
				}
			}

#if defined ZED_SHOW_TARGA_INFO

			zedTrace( "[ZED::Renderer::Targa::Load] <INFO> Read TARGA file "
				"\"%s\":\n", p_pFileName );
			zedTrace( "[ZED::Renderer::Targa::Load] <INFO> Width: %u\n",
				m_Width );
			zedTrace( "[ZED::Renderer::Targa::Load] <INFO> Height: %u\n",
				m_Height );
			zedTrace( "[ZED::Renderer::Targa::Load] <INFO> Format: %s\n",
				ZED::Renderer::FormatToString( m_Format ) );
#endif

			return ZED_OK;
		}

		ZED_UINT32 Targa::Load( ZED::System::File *p_pFile )
		{
			TARGA_HEADER TargaHeader;
			ZED_MEMSIZE Read = 0;

			p_pFile->ReadByte( reinterpret_cast< ZED_BYTE * >( &TargaHeader ),
				sizeof( TargaHeader ), &Read );

			zedTrace( "Targa Header information:\n" );
			zedTrace( "\tID length: %d\n", TargaHeader.IDLength );
			zedTrace( "\tWidth: %d\n", TargaHeader.Width );
			zedTrace( "\tHeight: %d\n", TargaHeader.Height );

			ZED_MEMSIZE ExpectedImageSize = TargaHeader.Width *
				TargaHeader.Height * ( TargaHeader.BitsPerPixel / 8 );

			m_pData = new ZED_BYTE[ ExpectedImageSize ];

			p_pFile->ReadByte( m_pData, ExpectedImageSize, &Read );

			return ZED_OK;
		}

		ZED_BYTE * const Targa::GetImageData( ) const
		{
			return m_pData;
		}

		ZED_UINT32 Targa::GetWidth( ) const
		{
			return m_Width;
		}

		ZED_UINT32 Targa::GetHeight( ) const
		{
			return m_Height;
		}

		ZED_FORMAT Targa::GetFormat( ) const
		{
			return m_Format;
		}
	}
}

