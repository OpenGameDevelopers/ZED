#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLFont.hpp>
#include <System/NativeFile.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLFont::GLFont( )
		{
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
				return ZED_FAIL;
			}

			if( FontFile.GetSize( ) < sizeof( FONTHEADER ) )
			{
				FontFile.Close( );
				return ZED_FAIL;
			}

			FONTHEADER FontHeader;
			ZED_MEMSIZE ReadSize;

			if( FontFile.ReadByte(
				reinterpret_cast< ZED_BYTE * >( &FontHeader ),
				sizeof( FontHeader ), &ReadSize ) != ZED_OK )
			{
				FontFile.Close( );
				return ZED_FAIL;
			}

			if( ReadSize != sizeof( FontHeader ) )
			{
				FontFile.Close( );
				return ZED_FAIL;
			}

			ZED_MEMSIZE BytesLeft = FontFile.GetSize( ) - sizeof( FONTHEADER );
			ZED_UINT32 ChunkStatus;

			while( BytesLeft )
			{
				if( BytesLeft < sizeof( ZED_FILE_CHUNK ) )
				{
					break;
				}

				ZED_FILE_CHUNK NextChunk;
				FontFile.ReadByte(
					reinterpret_cast< ZED_BYTE * >( &NextChunk ),
					sizeof( NextChunk ), &ReadSize );

				ChunkStatus = ReadChunk( NextChunk );

				if( ChunkStatus == ZED_FAIL )
				{
					break;
				}
			}

			// The final chunk should be ZED_FILE_CHUNK_END
			if( ChunkStatus != ZED_LASTCHUNKREAD )
			{
				FontFile.Close( );
				return ZED_OK;
			}

			FontFile.Close( );

			return ZED_OK;
		}

		ZED_UINT32 GLFont::ReadChunk( const ZED_FILE_CHUNK &p_FileChunk )
		{
			switch( p_FileChunk.Type )
			{
				case CHUNK_FONT_GLYPH:
				{
					return ZED_OK;
				}
				case CHUNK_FONT_TEXTURE:
				{
					return ZED_OK;
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
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

