#include <Renderer/Targa.hpp>
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
				return ZED_FAIL;
			}

			LPTARGA_HEADER pHeader = ZED_NULL;

			ZED_MEMSIZE FileLength = 0;
			ZED_MEMSIZE TotalFileSize = 0;

			TotalFileSize = fseek( pFile, 0L, SEEK_END );

			if( TotalFileSize < sizeof( TARGA_HEADER ) )
			{
				if( pFile )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}

				return ZED_FAIL;
			}

			rewind( pFile );

			FileLength = fread( pHeader, sizeof( TARGA_HEADER ), 1, pFile );

			if( FileLength == 0 )
			{
				if( pFile )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}

				return ZED_FAIL;
			}

			fseek( pFile, pHeader->IDLength, SEEK_SET );

			ZED_MEMSIZE DataBytesInFile = 0;
			ZED_MEMSIZE ExpectedImageSize = pHeader->Width * pHeader->Height *
				pHeader->BitsPerPixel;

			if( TotalFileSize < ( ExpectedImageSize - sizeof( TARGA_HEADER ) -
				sizeof( ZED_BYTE ) ) )
			{
				if( pFile )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}
				return ZED_FAIL;
			}

			fread( m_pData, sizeof( ZED_BYTE ), ExpectedImageSize, pFile );

			if( pFile )
			{
				fclose( pFile );
				pFile = ZED_NULL;
			}

			return ZED_OK;
		}

		ZED_BYTE const *Targa::GetImageData( ) const
		{
			return m_pData;
		}
	}
}

