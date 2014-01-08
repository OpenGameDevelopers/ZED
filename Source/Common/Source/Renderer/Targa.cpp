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
			m_Height( 0U )
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

			zedTrace( "Read TARGA file \"%s\":\n", p_pFileName );
			zedTrace( "Width: %u\n", m_Width );
			zedTrace( "Height: %u\n", m_Height );

			return ZED_OK;
		}

		ZED_BYTE const *Targa::GetImageData( ) const
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
	}
}

