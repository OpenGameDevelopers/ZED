#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLFont.hpp>
#include <System/NativeFile.hpp>
#include <System/MemoryMappedFile.hpp>
#include <Renderer/OGL/GLTexture.hpp>
#include <Renderer/OGL/GLShader.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <System/Memory.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		GLFont::GLFont( ZED::Renderer::Renderer * const &p_pRenderer ) :
			Font( p_pRenderer ),
			m_pShader( ZED_NULL ),
			m_pVertices( ZED_NULL ),
			m_pIndices( ZED_NULL ),
			m_TextureWidth( 0.0f ),
			m_TextureHeight( 0.0f )
		{
		}

		GLFont::~GLFont( )
		{
			zedSafeDelete( m_pShader );
			zedSafeDeleteArray( m_pVertices );
			zedSafeDeleteArray( m_pIndices );
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

			const char *VShader =
			{
				"#version 130\n"
				"uniform mat4 uPVW;\n"
				"in vec3 iPosition;\n"
				"in vec2 iUV;\n"
				"out vec2 oUV;\n"
				"void main( )\n"
				"{\n"
				"	oUV = iUV;\n"
				"	gl_Position = uPVW * vec4( iPosition, 1.0 );\n"
				"}\n"
			};

			const char *FShader =
			{
				"#version 130\n"
				"uniform sampler2D uTexture;\n"
				"in vec2 oUV;\n"
				"out vec4 fColour;\n"
				"void main( )\n"
				"{\n"
				"	fColour = texture( uTexture, oUV );\n"
				"}\n"
			};

			m_pShader = new GLShader( ZED_TRUE, ZED_TRUE, ZED_FALSE );
			
			if( m_pShader->Compile( &VShader, ZED_VERTEX_SHADER, ZED_FALSE ) !=
				ZED_OK )
			{
				return ZED_FAIL;
			}

			if( m_pShader->Compile( &FShader, ZED_FRAGMENT_SHADER,
				ZED_FALSE ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			ZED_SHADER_CONSTANT_MAP Constants[ 2 ];
			zedSetConstant( Constants, 0, ZED_MAT4X4, "uPVW" );
			zedSetConstant( Constants, 1, ZED_FLOAT1, "uTexture" );

			m_pShader->SetConstantTypes( Constants, 2 );

			return ZED_OK;
		}

		void GLFont::RenderGlyph( const ZED_CHAR8 p_Character,
			const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
			const ZED_FLOAT32 p_Scale )
		{
			if( m_pRenderer )
			{
				// Here's some slow code...
				glEnable( GL_BLEND );
				
				zglBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
				zglBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
					GL_ONE, GL_ZERO );

				ZED::Arithmetic::Matrix4x4 WorldMatrix, TranslateMatrix,
					ScaleMatrix, ProjectionWorldMatrix;
				ZED::Arithmetic::Vector3 Position;
				m_pTexture->Activate( );
				m_pShader->Activate( );

				GLYPHPOLY Glyph;
				std::map< ZED_CHAR8, GLYPHPOLY >::const_iterator GlyphItr;
				GlyphItr = m_GlyphPolys.find( p_Character );

				Glyph = GlyphItr->second;

				ZED_BYTE Vertices[ sizeof( GLYPHVERTEX ) * 4 ];
				memcpy( Vertices, Glyph.Vertex, sizeof( GLYPHVERTEX ) * 4 );

				ScaleMatrix.Scale( p_Scale );
				Position.Set( p_X, p_Y - ( Glyph.Height * p_Scale ) +
					( Glyph.BearingY * p_Scale ), 0.0f );

				TranslateMatrix.Translate( Position );
				WorldMatrix = TranslateMatrix * ScaleMatrix;

				ProjectionWorldMatrix = m_ProjectionMatrix * WorldMatrix;
				ZED_FLOAT32 ProjectionWorld[ 16 ];
				ProjectionWorldMatrix.AsFloat( ProjectionWorld );

				m_pShader->SetConstantData( 0, ProjectionWorld );
				m_pShader->SetConstantData( 1, 0 );

				m_pRenderer->Render( 4, Vertices, 6, m_pIndices, 0x56, 0,
					ZED_TRIANGLE_LIST );

				glDisable( GL_BLEND );
			}
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
			ZED_UINT32 TextureWidth;
			p_pFile->ReadUInt32( &TextureWidth, 1, ZED_NULL );
			ZED_UINT32 TextureHeight;
			p_pFile->ReadUInt32( &TextureHeight, 1, ZED_NULL );

			m_TextureWidth = static_cast< ZED_FLOAT32 >( TextureWidth );
			m_TextureHeight = static_cast< ZED_FLOAT32 >( TextureHeight );

			if( ZED::Arithmetic::IsZero( m_TextureWidth ) ||
				ZED::Arithmetic::IsZero( m_TextureHeight ) )
			{
				zedTrace( "[ZED::Renderer::GLFont::ReadGlyphs] <ERROR> "
					"The texture width and height are not valid!\n" );

				return ZED_FAIL;
			}
			
			m_pGlyphSet = new GLYPH[ GlyphCount ];

			zedTrace( "[ZED::Renderer::GLFont::ReadGlyphs] <INFO> Reading %d"
				" glyphs\n", GlyphCount );

			ZED_FLOAT32 OnePixelH = 1.0f / m_TextureWidth;
			ZED_FLOAT32 OnePixelV = 1.0f / m_TextureHeight;

			ZED_UINT16 Indices[ 6 ]  = { 0, 1, 2, 0, 2, 3 };
			m_pIndices = new ZED_UINT16[ 6 ];
			memcpy( m_pIndices, Indices, 6 * sizeof( ZED_UINT16 ) );

			for( ZED_UINT32 i = 0; i < GlyphCount; ++i )
			{
				GLYPH Glyph;
				p_pFile->ReadByte( reinterpret_cast< ZED_BYTE * >( &Glyph ),
					sizeof( GLYPH ), ZED_NULL );
				m_pGlyphSet[ i ] = Glyph;

				GLYPHPOLY GlyphPolygon;
				GlyphPolygon.Vertex[ 0 ].Position[ 0 ] = 0.0f;
				GlyphPolygon.Vertex[ 0 ].Position[ 1 ] =
					static_cast< ZED_FLOAT32 >( Glyph.Height );
				GlyphPolygon.Vertex[ 0 ].Position[ 2 ] = 0.0f;
				GlyphPolygon.Vertex[ 0 ].UV[ 0 ] =
					static_cast< ZED_FLOAT32 >( Glyph.X ) * OnePixelH;
				GlyphPolygon.Vertex[ 0 ].UV[ 1 ] =
					( m_TextureHeight -
						( static_cast< ZED_FLOAT32 >( Glyph.Y ) +
						static_cast< ZED_FLOAT32 >( Glyph.Height ) ) ) *
					OnePixelV;

				GlyphPolygon.Vertex[ 1 ].Position[ 0 ] = 0.0f;
				GlyphPolygon.Vertex[ 1 ].Position[ 1 ] = 0.0f;
				GlyphPolygon.Vertex[ 1 ].Position[ 2 ] = 0.0f;
				GlyphPolygon.Vertex[ 1 ].UV[ 0 ] =
					static_cast< ZED_FLOAT32 >( Glyph.X ) * OnePixelH;
				GlyphPolygon.Vertex[ 1 ].UV[ 1 ] =
					( m_TextureHeight -
						static_cast< ZED_FLOAT32 >( Glyph.Y ) ) *
					OnePixelV;

				GlyphPolygon.Vertex[ 2 ].Position[ 0 ] =
					static_cast< ZED_FLOAT32 >( Glyph.Width );
				GlyphPolygon.Vertex[ 2 ].Position[ 1 ] = 0.0f;
				GlyphPolygon.Vertex[ 2 ].Position[ 2 ] = 0.0f;
				GlyphPolygon.Vertex[ 2 ].UV[ 0 ] =
					( static_cast< ZED_FLOAT32 >( Glyph.X ) +
						static_cast< ZED_FLOAT32 >( Glyph.Width ) ) *
					OnePixelH;
				GlyphPolygon.Vertex[ 2 ].UV[ 1 ] =
					( m_TextureHeight -
						static_cast< ZED_FLOAT32 >( Glyph.Y ) ) *
					OnePixelV;

				GlyphPolygon.Vertex[ 3 ].Position[ 0 ] =
					static_cast< ZED_FLOAT32 >( Glyph.Width );
				GlyphPolygon.Vertex[ 3 ].Position[ 1 ] =
					static_cast< ZED_FLOAT32 >( Glyph.Height );
				GlyphPolygon.Vertex[ 3 ].Position[ 2 ] = 0.0f;
				GlyphPolygon.Vertex[ 3 ].UV[ 0 ] =
					( static_cast< ZED_FLOAT32 >( Glyph.X ) +
						static_cast< ZED_FLOAT32 >( Glyph.Width ) ) *
						OnePixelH;
				GlyphPolygon.Vertex[ 3 ].UV[ 1 ] =
					( m_TextureHeight -
						( static_cast< ZED_FLOAT32 >( Glyph.Y ) +
							static_cast< ZED_FLOAT32 >( Glyph.Height ) ) ) *
					OnePixelV;

				GlyphPolygon.Width = static_cast< ZED_FLOAT32 >( Glyph.Width );
				GlyphPolygon.Height =
					static_cast< ZED_FLOAT32 >( Glyph.Height );
				GlyphPolygon.BearingY =
					static_cast< ZED_FLOAT32 >( Glyph.BearingY );

				m_GlyphPolys.insert( std::pair< ZED_CHAR8, GLYPHPOLY >(
					Glyph.Character, GlyphPolygon ) );
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

			m_GlyphCount = GlyphCount;

			return ZED_OK;
		}

		ZED_UINT32 GLFont::ReadTexture( ZED::System::File *p_pFile,
			const ZED_UINT64 p_ChunkSize )
		{
			ZED::System::MemoryMappedFile TextureFile;

			// 0 should be replaced by the offset into the file...
			// As it should lie on the page boundary, the page size should be
			// retrieved and used to determine the offset (maybe divide + mod?)
			if( TextureFile.SetFileToMap32( ( *p_pFile ), 0,
				// Have to use the current pointer in the file, as we're
				// starting from zero
				p_ChunkSize + p_pFile->GetPosition( ) ) !=
				ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLFont::ReadTexture] <ERROR> "
					"Failed to map file\n" );

				return ZED_FAIL;
			}

			if( TextureFile.Open( "", 0 ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLFont::ReadTexture] <ERROR> "
					"Failed to open mapped file\n" );

				return ZED_FAIL;
			}

			TextureFile.Seek( p_pFile->GetPosition( ),
				ZED::System::FILE_SEEK_CURRENT );

			m_pTexture = new ZED::Renderer::GLTexture( );

			if( m_pTexture->Load( &TextureFile ) != ZED_OK )
			{
				TextureFile.Close( );
				
				zedTrace( "[ZED::Renderer::GLFont::ReadTexture] <ERROR> "
					"Failed to load texture file\n" );

				return ZED_FAIL;
			}

			TextureFile.Close( );

			m_pTexture->SetTextureUnit( 0 );

			// Jump over to what should be the end chunk ID
			p_pFile->Seek( p_ChunkSize, ZED::System::FILE_SEEK_CURRENT );

			ZED_FILE_CHUNK LastChunk;
			ReadChunk( &LastChunk, p_pFile );

			if( LastChunk.Type != ZED_FILE_CHUNK_END )
			{
				zedTrace( "[ZED::Renderer::GLFont::ReadTexture] <ERROR> "
					"Unexpected chunk (was expecting 0xFFFF, got 0x%04X)\n",
					LastChunk.Type );

				return ZED_FAIL;
			}

			m_TextureWidth =
				static_cast< ZED_FLOAT32 >( m_pTexture->GetWidth( ) );
			m_TextureHeight =
				static_cast< ZED_FLOAT32 >( m_pTexture->GetHeight( ) );
		}
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

