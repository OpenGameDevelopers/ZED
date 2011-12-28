#include <GLTexture.hpp>
#include <cstdio>

namespace ZED
{
	namespace Renderer
	{
		ZED_MEMSIZE FormatToBytes( const ZED_FORMAT p_Format )
		{
			ZED_MEMSIZE Size = 0;
			
			switch( p_Format )
			{
				case ZED_FORMAT_XRGB8:
				case ZED_FORMAT_ARGB8:
				{
					Size = 4;
					break;
				}
				case ZED_FORMAT_RGB565:
				{
					Size = 2;
					break;
				}
			}

			return Size;
		}

		GLTexture::GLTexture( )
		{
			m_pData = ZED_NULL;

			m_Width = 0;
			m_Height = 0;

			m_Format = ZED_FORMAT_UNDEFINED;
		}

		GLTexture::~GLTexture( )
		{
			if( m_pData != ZED_NULL )
			{
				delete [ ] m_pData;
				m_pData = ZED_NULL;
			}
		}

		ZED_UINT32 GLTexture::Load( const ZED_CHAR8 *p_pFilename )
		{
			FILE *pFile = fopen( p_pFilename, "rb" );

			if( pFile == ZED_NULL )
			{
				return ZED_FAIL;
			}

			// Get the header information
			ZEDT_HEADER TexHeader;

			fread( &TexHeader, sizeof( ZEDT_HEADER ), 1, pFile );

			if( ( TexHeader.ID[ 0 ] != 'Z' ) ||
				( TexHeader.ID[ 1 ] != 'E' ) ||
				( TexHeader.ID[ 2 ] != 'D' ) ||
				( TexHeader.ID[ 3 ] != 'T' ) )
			{
				fclose( pFile );
				return ZED_FAIL;
			}

			// Skip the version information for now
			fseek( pFile, sizeof( ZED_UINT32 )*3, SEEK_SET );

			// Store the width, height, and format
			fread( &m_Width, sizeof( ZED_UINT32 ), 1, pFile );
			fread( &m_Height, sizeof( ZED_UINT32 ), 1, pFile );
			fread( &m_Format, sizeof( ZED_FORMAT ), 1, pFile );

			// Now the data is stored immediately after
			// Calculate the size of the data to load
			ZED_MEMSIZE DataSize = FormatToBytes( m_Format );
			DataSize *= ( m_Width*m_Height );

			m_pData = new ZED_BYTE[ DataSize ];

			fread( m_pData, DataSize, 1, pFile );

			if( pFile != ZED_NULL )
			{
				fclose( pFile );
				pFile = ZED_NULL;
			}

			return ZED_OK;
		}
	}
}

