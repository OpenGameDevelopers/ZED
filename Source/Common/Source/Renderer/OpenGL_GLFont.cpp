#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLFont.hpp>
#include <System/NativeFile.hpp>
#include <System/MemoryMappedFile.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLFont::GLFont( )
		{
			m_pGlyphSet = ZED_NULL;
		}

		GLFont::~GLFont( )
		{
		}

		ZED_UINT32 GLFont::Load( const ZED_CHAR8 *p_pFilePath )
		{
			ZED::System::NativeFile FontFile;

			if( FontFile.Open( p_pFilePath,
				ZED::System::FILE_ACCESS_READ |
					ZED::System::FILE_ACCESS_BINARY ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
					"Fatal error accessing file: \"%s\"\n", p_pFilePath );

				return ZED_FAIL;
			}

			if( FontFile.GetSize( ) < sizeof( FONTHEADER ) )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
					"File size is no longer than that of the header read "
					"in\n" );
				FontFile.Close( );

				return ZED_FAIL;
			}

			FONTHEADER FontHeader;
			ZED_MEMSIZE ReadSize;

			if( FontFile.ReadByte(
				reinterpret_cast< ZED_BYTE * >( &FontHeader ),
				sizeof( FontHeader ), &ReadSize ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
					"Failed to read the header for the font\n" );
				FontFile.Close( );

				return ZED_FAIL;
			}

			if( ReadSize != sizeof( FontHeader ) )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
					"Header read in was not the same as the size expected\n" );
				FontFile.Close( );

				return ZED_FAIL;
			}

			if( ( FontHeader.ID[ 0 ] != 'Z' ) &&
				( FontHeader.ID[ 1 ] != 'E' ) &&
				( FontHeader.ID[ 2 ] != 'D' ) &&
				( FontHeader.ID[ 3 ] != 'F' ) )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
					"Header ID was not 'ZEDF' as expected, got: %c%c%c%c\n",
					FontHeader.ID[ 0 ], FontHeader.ID[ 1 ], FontHeader.ID[ 2 ],
					FontHeader.ID[ 3 ] );
				FontFile.Close( );

				return ZED_FAIL;
			}

			ZED_UINT64 BytesLeft = FontFile.GetSize( ) - sizeof( FONTHEADER );

			ZED_UINT32 ChunkStatus;

			while( BytesLeft )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <DEBUG> "
					"Bytes left: %d\n", BytesLeft );

				if( BytesLeft < sizeof( ZED_FILE_CHUNK ) )
				{
					zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
						"Bytes remaining are less than that of a single "
						"chunk\n" );

					break;
				}

				ZED_FILE_CHUNK NextChunk;
				ChunkStatus = ReadChunk( &NextChunk, &FontFile );

				if( ChunkStatus == ZED_FAIL )
				{
					zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
						"Failed to read the chunk\n" );
					ChunkStatus = ZED_FAIL;

					break;
				}

				BytesLeft -= sizeof( NextChunk );
				BytesLeft -= NextChunk.Size;

				if( ChunkStatus != ZED_LASTCHUNKREAD )
				{
					BytesLeft -= sizeof( NextChunk );
				}
			}

			FontFile.Close( );

			// The final chunk should be ZED_FILE_CHUNK_END
			if( ChunkStatus != ZED_LASTCHUNKREAD )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::Load] <ERROR> "
					"The last chunk read was not the expected last chunk "
					"identifier\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLFont::ReadChunk( ZED_FILE_CHUNK *p_pFileChunk,
			ZED::System::File *p_pFile )
		{
			ZED_MEMSIZE ReadSize;

			p_pFile->ReadByte( reinterpret_cast< ZED_BYTE * >( p_pFileChunk ),
				sizeof( ZED_FILE_CHUNK ), &ReadSize );

			if( ReadSize != sizeof( ZED_FILE_CHUNK ) )
			{
				zedTrace( "[ZED::Renderer::OGL::GLFont::ReadChunk] <ERROR> "
					"Failed to correctly read the next chunk in the font "
					"file\n" );

				return ZED_FAIL;
			}

			zedTrace( "[ZED::Renderer::OGL::GLFont::ReadChunk] <DEBUG> "
				"Chunk Type: 0x%04X\n", p_pFileChunk->Type );

			switch( p_pFileChunk->Type )
			{
				case CHUNK_FONT_GLYPH:
				{
					return ReadGlyphs( p_pFile );
				}
				case CHUNK_FONT_TEXTURE:
				{
					return ReadTexture( p_pFile, p_pFileChunk->Size );
				}
				case ZED_FILE_CHUNK_END:
				{
					return ZED_LASTCHUNKREAD;
				}
				default:
				{
					return ZED_FAIL;
				}
			}
		}

		ZED_UINT32 GLFont::ReadGlyphs( ZED::System::File *p_pFile )
		{
			ZED_UINT32 GlyphCount = 0U;

			p_pFile->ReadUInt32( &GlyphCount, 1, ZED_NULL );
			
			m_pGlyphSet = new GLYPH[ GlyphCount ];

			zedTrace( "[ZED::Renderer::GLFont::ReadGlyphs] <INFO> Reading %d"
				" glyphs\n", GlyphCount );

			for( ZED_UINT32 i = 0; i < GlyphCount; ++i )
			{
				GLYPH Glyph;
				p_pFile->ReadByte( reinterpret_cast< ZED_BYTE * >( &Glyph ),
					sizeof( GLYPH ), ZED_NULL );
				m_pGlyphSet[ i ] = Glyph;
				zedTrace( "Found glyph [%d]\n"
					"\tCharacter: %c\n"
					"\tX Offset:  %d\n"
					"\tY Offset:  %d\n"
					"\tWidht:     %d\n"
					"\tHeight:    %d\n",
					i, Glyph.Character, Glyph.X, Glyph.Y, Glyph.Width,
					Glyph.Height );
			}

			ZED_FILE_CHUNK LastChunk;

			ReadChunk( &LastChunk, p_pFile );

			if( LastChunk.Type != ZED_FILE_CHUNK_END )
			{
				zedTrace( "[ZED::Renderer::GLFont::ReadGlyphs] <ERROR> "
					"Unexpected chunk (was expecting 0xFFFF, got 0x%04X)\n",
					LastChunk.Type );

				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLFont::ReadTexture( ZED::System::File *p_pFile,
			const ZED_UINT64 p_ChunkSize )
		{
			ZED::System::MemoryMappedFile TextureFile;
			zedTrace( "Mapping file...\n" );

			zedTrace( "Size of targa file: %lu\n", p_ChunkSize );

			// 0 should be replaced by the offset into the file...
			if( TextureFile.SetFileToMap32( ( *p_pFile ), 0, p_ChunkSize ) !=
				ZED_OK )
			{
				zedTrace( "Failed to map file\n" );
				return ZED_FAIL;
			}

			if( TextureFile.Open( "", 0 ) != ZED_OK )
			{
				zedTrace( "Failed to open mapped file\n" );
				return ZED_FAIL;
			}

			TextureFile.Close( );

			p_pFile->Seek( p_ChunkSize, ZED::System::FILE_SEEK_CURRENT );
		}
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

