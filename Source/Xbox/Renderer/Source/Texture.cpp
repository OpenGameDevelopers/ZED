#include <Texture.hpp>
#include <xgraphics.h>

namespace ZED
{
	namespace Renderer
	{
		D3DFORMAT ToD3DFormat( const ZED_FORMAT p_Format )
		{
			switch( p_Format )
			{
			case ZED_FORMAT_RGB565:
				return D3DFMT_R5G6B5;
			default:
				return D3DFMT_UNKNOWN;
			}
		}

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
			m_pData = ZED_NULL;
			m_pName = ZED_NULL;
			m_pD3DTexture = ZED_NULL;
		}

		Texture::~Texture( )
		{
			if( m_pData != ZED_NULL )
			{
				XPhysicalFree( m_pData );
				m_pData = ZED_NULL;
			}

			if( m_pD3DTexture != ZED_NULL )
			{
				delete m_pD3DTexture;
				m_pD3DTexture = ZED_NULL;
			}

			if( m_pName != ZED_NULL )
			{
				XPhysicalFree( m_pName );
				m_pName = ZED_NULL;
			}
		}

		ZED_UINT32 Texture::Load( const char *p_pFilename )
		{
			// Get the file extension and check it against the known types
			// Known Types:
			// TGA
			// PNG - Implementing...
			// BMP
			// DDS
			// ZED - Designing... [Ver. 1.0 is good for now]

			HANDLE TextureFile = ZED_NULL;
			TextureFile = CreateFile( p_pFilename, GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

			if( TextureFile == ZED_NULL )
			{
				zedTrace( "Failed to load texture: %s\n", p_pFilename );
				return ZED_FAIL;
			}

			ZED_MEMSIZE FilenameLen = strlen( p_pFilename );
			char DotChar = '\0';
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
				zedTrace( "[ZED|Renderer|Texture::Load] Failed to get "
					"extension for %s", p_pFilename );
			}

			char *pExtStr = new char[ ExtChars ];
			// Copy the extension over
			for( ZED_MEMSIZE i = 0; i < ExtChars; i++ )
			{
				pExtStr[ i ] = p_pFilename[ ExtStart+i ];
			}

			// Check the file extension against known types
			pExtStr = strlwr( pExtStr );
			if( strncmp( pExtStr, "zedt", 4 ) == 0 )
			{
				// Valid ZED Texture?
				ZEDT_HEADER Header;
				DWORD ReadIn = 0;

				ReadFile( TextureFile, &Header, sizeof( ZEDT_HEADER ), &ReadIn,
					ZED_NULL );

				if( ReadIn != sizeof( ZEDT_HEADER ) )
				{
					zedTrace( "ZEDT Header for %s not valid.\n", p_pFilename );
					return ZED_FAIL;
				}

				// Get the raw texture data
				ZED_MEMSIZE BPP = GetBPP( Header.Format );

				if( BPP == 0 )
				{
					zedTrace( "[ERROR] Incorrect BPP for file: %s\n", p_pFilename );
					return ZED_FAIL;
				}

				m_Width = Header.Width;
				m_Height = Header.Height;
				m_Format = Header.Format;
				
				if( Header.Version[ 0 ] == 0 )
				{
					ZED_BYTE *pTextureData = ZED_NULL;

					pTextureData = new ZED_BYTE[ m_Width*m_Height*( BPP/8 ) ];
					ReadFile( TextureFile, pTextureData,
						m_Width*m_Height*( BPP/8 ), &ReadIn, ZED_NULL );

					if( ReadIn != m_Width*m_Height*( BPP/8 ) )
					{
						if( pTextureData )
						{
							delete pTextureData;
						}
						zedTrace( "[ERROR] Mis-matched data size for file: %s",
							p_pFilename );

						delete pExtStr;
						CloseHandle( TextureFile );

						return ZED_FAIL;
					}

					m_pD3DTexture = new IDirect3DTexture8( );

					// Set up the D3DTexture with 
					DWORD TextureSize = XGSetTextureHeader( m_Width, m_Height,
						1, 0, ToD3DFormat( m_Format ), 0, m_pD3DTexture, 0, 0 );

					m_pData = XPhysicalAlloc( TextureSize, MAXULONG_PTR,
						D3DTEXTURE_ALIGNMENT, PAGE_READWRITE | PAGE_WRITECOMBINE );
						
					m_pD3DTexture->Register( m_pData );

					// Copy and swizzle, if necessary
					// If swizzling is not necessary, just copy the data over
					if( XGIsSwizzledFormat( ToD3DFormat( m_Format ) ) )
					{
						XGSwizzleRect( pTextureData, 0, NULL, m_pData, m_Width,
							m_Height, NULL, sizeof( WORD ) );
					}
					else
					{
						memcpy( m_pData, pTextureData, TextureSize );
					}

					delete pTextureData;
				}

				m_pName = static_cast< char * >( XPhysicalAlloc(
					strlen( p_pFilename )+1, MAXULONG_PTR, 0, PAGE_READWRITE ) );
				ZeroMemory( m_pName, strlen( p_pFilename ) );
				strcpy( m_pName, p_pFilename );

				zedTrace( "[INFO] Loaded texture %s\n"
					"\tWidth: %d | Height: %d | BPP: %d | Format: %s\n",
					p_pFilename, m_Width, m_Height, BPP,
					FormatToString( m_Format ) );
			}
			else if( strncmp( pExtStr, "png", 3 ) == 0 )
			{
				// PNG Support is, well, not here =P
				// Determine if the PNG is a valid PNG
				PNGHEADER Header;
/*				fread( &Header.Transmission, sizeof( ZED_BYTE ), 1, pFile );
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

				ZED_UINT32 Dummy = 0;*/
			}
			else
			{
				// Uh-oh, file is not valid
				zedTrace( "[ZED|Renderer|Texture::Load] Unknown file type: %s",
					pExtStr );
				return ZED_FAIL;
			}

			CloseHandle( TextureFile );
			delete pExtStr;

			return ZED_OK;
		}
	}
}
