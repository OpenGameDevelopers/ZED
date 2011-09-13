#include <Texture.hpp>

namespace ZED
{
	namespace Renderer
	{
		void PNGCRCTableCreate( )
		{
			ZED_LONG	C;
			ZED_INT32	N, K;

			for( N = 0; N < 256; N++ )
			{
				C = static_cast< ZED_LONG >( N );
				for( K = 0; K < 8; K++ )
				{
					if( C & 1 )
					{
						C = 0xEDB88320L ^ ( C >> 1 );
					}
					else
					{
						C = C >> 1;
					}
				}
				g_PNGCRCTable[ N ] = C;
			}
			g_PNGCRCTableInit = ZED_TRUE;
		}

		ZED_ULONG PNGUpdateCRC( ZED_ULONG p_CRC, ZED_BYTE *p_pBuffer,
			ZED_INT32 p_Length )
		{
			ZED_ULONG C = p_CRC;
			ZED_INT32 N;

			if( !g_PNGCRCTableInit )
			{
				PNGCRCTableCreate( );
			}

			for( N = 0; N < p_Length; N++ )
			{
				C = g_PNGCRCTable[ ( C ^ p_pBuffer[ N ] ) & 0xFF ] ^ ( C >> 8 );
			}

			return C;
		}

		ZED_ULONG PNGCRC( ZED_BYTE *p_pBuffer, ZED_INT32 p_Length )
		{
			return PNGUpdateCRC( 0xFFFFFFFFL, p_pBuffer, p_Length ) ^ 0xFFFFFFFFL;
		}

		ZED_UINT32 ReadPNGChunk( )
		{
			return ZED_OK;
		}

		Texture::Texture( )
		{
		}

		Texture::~Texture( )
		{
		}

		ZED_UINT32 Texture::Load( const ZED_CHAR16 *p_pFilename )
		{
			// Get the file extension and check it against the known types
			// Known Types:
			// TGA
			// PNG - Implementing...
			// BMP
			// DDS
			// ZED - Designing...

			FILE *pFile = ZED_NULL;
			pFile = _wfopen( p_pFilename, L"rb" );
			if( pFile == ZED_NULL )
			{
				// Failed to get the file
				zedTrace( "[ZED|Renderer|Texture::Load] Failed to load %s%25.3ls",
					p_pFilename );
				return ZED_FAIL;
			}

			ZED_MEMSIZE FilenameLen = wcslen( p_pFilename );
			ZED_CHAR16 DotChar = '\0';
			ZED_MEMSIZE ExtChars = 0;
			ZED_MEMSIZE ExtStart = 0;

			// Determine the file type
			for( ZED_MEMSIZE i = FilenameLen-1; i > 0; i-- )
			{
				DotChar = p_pFilename[ i ];
				if( DotChar == '.' )
				{
					// This is where the extension begins, we're done
					ExtStart = i+1;
					break;
				}
				ExtChars++;
			}

			// Is the extension valid?
			if( ExtChars == 0 )
			{
				zedTrace( "[ZED|Renderer|Texture::Load] Failed to get extension "
					"for %s", p_pFilename );
			}

			ZED_CHAR16 *pExtStr = new ZED_CHAR16[ ExtChars ];
			// Copy the extension over
			for( ZED_MEMSIZE i = 0; i < ExtChars; i++ )
			{
				pExtStr[ i ] = p_pFilename[ ExtStart+i ];
			}

			// Check the file extension against known types
			pExtStr = _wcslwr( pExtStr );
			if( wcsncmp( pExtStr, L"zedt", 3 ) == 0 )
			{
				// Process ZED Texture
			}
			else if( wcsncmp( pExtStr, L"png", 3 ) == 0 )
			{
				// Determine if the PNG is a valid PNG
				PNGHEADER Header;
				fread( &Header.Transmission, sizeof( ZED_BYTE ), 1, pFile );
				fread( Header.PNGString, sizeof( ZED_BYTE ), 3, pFile );
				fread( Header.LineEndDOS, sizeof( ZED_BYTE ), 2, pFile );
				fread( &Header.DOSStop, sizeof( ZED_BYTE ), 1, pFile );
				fread( &Header.LineEndUnix, sizeof( ZED_BYTE ), 1, pFile );

				// Check what was just read
				if( Header.Transmission != 0x89 )
				{
					zedTrace( "[ZED|Renderer|Texture::Load] PNG is not valid.\n" );
					fclose( pFile );
					return ZED_FAIL;
				}
				if( Header.PNGString[ 0 ] != 'P' ||
					Header.PNGString[ 1 ] != 'N' ||
					Header.PNGString[ 2 ] != 'G' )
				{
					zedTrace( "[ZED|Renderer|Texture::Load] PNG is not valid.\n" );
					fclose( pFile );
					return ZED_FAIL;
				}
				if( Header.LineEndDOS[ 0 ] != 0x0D ||
					Header.LineEndDOS[ 1 ] != 0x0A )
				{
					zedTrace( "[ZED|Renderer|Texture::Load] PNG is not valid.\n" );
					fclose( pFile );
					return ZED_FAIL;
				}
				if( Header.DOSStop != 0x1A )
				{
					zedTrace( "[ZED|Renderer|Texture::Load] PNG is not valid.\n" );
					fclose( pFile );
					return ZED_FAIL;
				}
				if( Header.LineEndUnix != 0x0A )
				{
					zedTrace( "[ZED|Renderer|Texture::Load] PNG is not valid.\n" );
					fclose( pFile );
					return ZED_FAIL;
				}

				// The first chunk must be IHDR
				PNGCHUNK IHeader;
				memset( &IHeader, 0, sizeof( PNGCHUNK ) );
				fread( &IHeader.Length, sizeof( ZED_UINT32 ), 1, pFile );
				fread( &IHeader.Type, sizeof( ZED_UINT32 ), 1, pFile );

				ZED_UINT32 IHDRCC = zedSwapEndian32( MakeDword( 'I', 'H', 'D', 'R' ) );
				if( IHeader.Type != IHDRCC )
				{
					zedTrace( "[ZED|Renderer|Texture::Load] Invalid Chunk Type.\n" );
					fclose( pFile );
					return ZED_FAIL;
				}

				PNGIHDR IHDR;
				memset( &IHDR, 0, sizeof( PNGIHDR ) );
				ZED_UINT32 TempWidth, TempHeight;
				fread( &TempWidth, sizeof( ZED_UINT32 ), 1, pFile );
				fread( &TempHeight, sizeof( ZED_UINT32 ), 1, pFile );
				IHDR.Width = zedSwapEndian32( TempWidth );
				IHDR.Height = zedSwapEndian32( TempHeight );
				fread( &IHDR.BitDepth, sizeof( ZED_BYTE ), 1, pFile );
				fread( &IHDR.ColourType, sizeof( ZED_BYTE ), 1, pFile );
				fread( &IHDR.Compression, sizeof( ZED_BYTE ), 1, pFile );
				fread( &IHDR.Filter, sizeof( ZED_BYTE ), 1, pFile );
				fread( &IHDR.Interlace, sizeof( ZED_BYTE ), 1, pFile );
				// Calculate the CRC
				ZED_ULONG TestCRC = PNGCRC( ( ZED_BYTE* )&IHDR, zedSwapEndian32( IHeader.Length ) );
				TestCRC = zedSwapEndian32( TestCRC );
				ZED_UINT32 HeaderCRC = 0;
				fread( &HeaderCRC, sizeof( ZED_UINT32 ), 1, pFile );

				ZED_UINT32 Dummy = 0;
			}
			else
			{
				// Uh-oh, file is not valid
				zedTrace( "[ZED|Renderer|Texture::Load] Unknown file type: %s",
					pExtStr );
				return ZED_FAIL;
			}

			fclose( pFile );

			return ZED_OK;
		}
	}
}
