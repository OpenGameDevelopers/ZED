#include <Model.hpp>
#include <GLShader.hpp>
#include <GLExtender.hpp>
#include <cstdio>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		Model::Model( )
		{
			m_pFile = ZED_NULL;
			m_pVertexData = ZED_NULL;
			m_pFaceData = ZED_NULL;
			m_pIndices = ZED_NULL;
			m_pVertexNormals = ZED_NULL;
			m_pShader = ZED_NULL;
			m_pVertices = ZED_NULL;
			m_pVertexUVs = ZED_NULL;

			m_ppFaceNormals = ZED_NULL;
			m_ppFaceIndices = ZED_NULL;

			m_WorldMatrix.Identity( );

#ifdef ZED_BUILD_DEBUG
			m_RenderWireframe = ZED_FALSE;
			m_RenderVertexNormals = ZED_FALSE;
			m_RenderFaceNormals = ZED_FALSE;
			m_RenderBones = ZED_FALSE;
			m_pDebugShader = new GLShader( ZED_TRUE, ZED_TRUE, ZED_FALSE );
			m_ppMeshNames = ZED_NULL;
#endif
		}

		Model::~Model( )
		{
			if( m_pFile != ZED_NULL )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
			}
		}

		ZED_UINT32 Model::Load( const ZED_CHAR8 *p_pFilename )
		{
			zedTrace( "[ZED::Renderer::Model::Load] <INFO> "
				"Loading model: %s\n", p_pFilename );
			m_pFile = fopen( p_pFilename, "rb" );

			if( m_pFile == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::Model::Load] <ERROR> "
					"Failed to open model: %s\n", p_pFilename );
				return ZED_FAIL;
			}

			// The first chunk has got to be the header
			if( LoadHeader( ) != ZED_OK )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
				return ZED_FAIL;
			}

			ZED_UINT16 Type;
			ZED_UINT64 Size;

			ReadChunk( Type, Size );

			// Read all remaining chunks in
			while( Type != ZED_V1_END )
			{
				switch( Type )
				{
					case ZED_V1_VERTEX:
					{
						LoadVertices( Size );
						// Get more
						ReadChunk( Type, Size );
						break;
					}
					case ZED_V1_FACE:
					{
						LoadFaces( Size );
						// Get more
						ReadChunk( Type, Size );
						break;
					}
					case ZED_V1_MESH:
					{
						LoadMeshes( Size );
						// Get more
						ReadChunk( Type, Size );
						break;
					}
					default:
					{
						zedTrace( "[ZED::Renderer::Model::Load <ERROR> "
							"Unknown type chunk.\n" );
						continue;
					}
				}
			}

			if( m_pFile != ZED_NULL )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
			}

			// TEMP!
			zglGenVertexArrays( 1, m_pVertexArrayID );
			zglBindVertexArray( m_pVertexArrayID[ 0 ] );

			zglGenBuffers( 3, m_VertexBufferObjectID );
			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObjectID[ 0 ] );

			zglEnableVertexAttribArray( 0 );
			zglEnableVertexAttribArray( 1 );
			zglEnableVertexAttribArray( 2 );

			zglBufferData( GL_ARRAY_BUFFER,
				sizeof( ZED_FLOAT32 )*3*m_VertexCount, m_pVertices,
				GL_STATIC_DRAW );
			zglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObjectID[ 1 ] );
			zglBufferData( GL_ARRAY_BUFFER,
				sizeof( ZED_FLOAT32 )*2*m_VertexCount, m_pVertexUVs,
				GL_STATIC_DRAW );
			zglVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );

			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObjectID[ 2 ] );
			zglBufferData( GL_ARRAY_BUFFER,
				sizeof( ZED_FLOAT32 )*3*m_VertexCount, m_pVertexNormals,
				GL_STATIC_DRAW );

			// Finished
			zglBindVertexArray( 0 );

			// !TEMP

			return ZED_OK;
		}
		
		void Model::Render( )
		{
			zglBindVertexArray( m_pVertexArrayID[ 0 ] );
			zglDrawElements( GL_TRIANGLES, 3*m_FaceCount, GL_UNSIGNED_SHORT,
				m_pIndices );
		}

		ZED_UINT32 Model::LoadHeader( )
		{
			LPFILEHEADER_V1 pHeaderInfo = new FILEHEADER_V1;

			// Assume the file is already open
			ZED_UINT16 ChunkType;
			ZED_UINT64 ChunkSize;

			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_V1_HEADER )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Chunk is not a header chunk.\n" );
				zedTrace( "Size: %X | Type: %X.\n", ChunkSize, ChunkType );
				delete pHeaderInfo;
				return ZED_FAIL;
			}

			fread( pHeaderInfo, sizeof( FILEHEADER_V1 ), 1, m_pFile );

			// Make sure that this is in the correct format
			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_V1_END )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				delete pHeaderInfo;
				return ZED_FAIL;
			}

			if( pHeaderInfo->ID[ 0 ] != 'Z' &&
				pHeaderInfo->ID[ 1 ] != 'E' &&
				pHeaderInfo->ID[ 2 ] != 'D' )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Header is not from a valid ZED model\n" );
				delete pHeaderInfo;
				return ZED_FAIL;
			}

			if( pHeaderInfo->Endianess == 'E' )
			{
				m_SwapBytes = ZED_TRUE;
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <INFO> "
					"Swapping bytes.\n" );
			}

			m_MeshCount = pHeaderInfo->Meshes;
			m_FaceCount = pHeaderInfo->Faces;
			m_IndexCount = pHeaderInfo->Indices;
			m_VertexCount = pHeaderInfo->Vertices;
			m_FaceCount = pHeaderInfo->Faces;
			m_pIndices = new ZED_UINT16[ m_FaceCount*3 ];


#ifdef ZED_BUILD_DEBUG
			//strcpy( m_Name, pHeaderInfo->Name );
			for( ZED_MEMSIZE i = 0; i < 32; i++ )
			{
				m_Name[ i ] = pHeaderInfo->Name[ i ];
			}
			ZED_CHAR8 *pEndianess = ZED_NULL;
			if( pHeaderInfo->Endianess == 'E' )
			{
				pEndianess = new ZED_CHAR8[ 4 ];
				strncpy( pEndianess, "Big\0", 4 );
			}
			else
			{
				pEndianess = new ZED_CHAR8[ 7 ];
				strncpy( pEndianess, "Little\0", 7 );
			}
			zedTrace( "[ZED::Renderer::Model::LoadHeader] <INFO> "
				"Header for: %s loaded successfully.\n", m_Name );
			zedTrace( "\tEndianess: %s | Version: %d.%d.%d | "
				"Vertices: %d | Indices: %d | Faces: %d | Meshes: %d |"
				" Type: %c\n", pEndianess,
				pHeaderInfo->Version[ 0 ], pHeaderInfo->Version[ 1 ],
				pHeaderInfo->Version[ 2 ], pHeaderInfo->Vertices,
				pHeaderInfo->Indices, pHeaderInfo->Faces,
				pHeaderInfo->Meshes, pHeaderInfo->Type );

			if( pEndianess != ZED_NULL )
			{
				delete pEndianess;
			}
#endif
			if( pHeaderInfo != ZED_NULL )
			{
				delete pHeaderInfo;
			}

			return ZED_OK;
		}

		ZED_UINT32 Model::LoadVertices( ZED_UINT64 p_Size )
		{
			// Convert the raw size into the amount of vertices being loaded
			m_VertexCount = p_Size / sizeof( VERTEX_V1 );
#ifdef ZED_BUILD_DEBUG
			zedTrace( "[ZED::Renderer::Model::LoadVertices] <INFO> "
				"Vertex Count: %d.\n", m_VertexCount );
#endif

			// New up the vertices and read int the values
			m_pVertices = new ZED_FLOAT32[ m_VertexCount * 3 ];
			m_pVertexNormals = new ZED_FLOAT32[ m_VertexCount * 3 ];
			m_pVertexUVs = new ZED_FLOAT32[ m_VertexCount * 2 ];

			for( ZED_MEMSIZE i = 0; i < m_VertexCount; i++ )
			{
				if( fread( &m_pVertices[ i*3 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FALSE;
				}
				if( fread( &m_pVertices[ ( i*3 )+1 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FALSE;
				}
				if( fread( &m_pVertices[ ( i*3 )+2 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 ) 
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FALSE;
				}

				if( fread( &m_pVertexNormals[ ( i*3 ) ], sizeof( ZED_FLOAT32 ),
					1, m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex normal data.\n" );
					return ZED_FALSE;
				}
				if( fread( &m_pVertexNormals[ ( i*3 )+1 ],
					sizeof( ZED_FLOAT32 ), 1, m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex normal data.\n" );
					return ZED_FALSE;
				}
				if( fread( &m_pVertexNormals[ ( i*3 )+2 ],
					sizeof( ZED_FLOAT32 ), 1, m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex normal data.\n" );
					return ZED_FALSE;
				}
				
				if( fread( &m_pVertexUVs[ i*2 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex UV data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertexUVs[ ( i*2 )+1 ], sizeof( ZED_FLOAT32 ),
					1, m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
						"Failed to load vertex UV data.\n" );
					return ZED_FAIL;
				}
			}

			// In case of emergency... [Seriously. DON'T DO THIS, EVER, RICO!]
#ifdef ZED_BUILD_DEBUG
			for( ZED_MEMSIZE i = 0; i < m_VertexCount; i++ )
			{
				zedTrace( "Vertex %d: %f %f %f\n", i,
					m_pVertices[ i*3 ],
					m_pVertices[ ( i*3 )+1 ],
					m_pVertices[ ( i*3 )+2 ] );
			}
#endif
			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// Should be the end chunk
			if( Type != ZED_V1_END )
			{
				zedTrace( "[ZED::Renderer::Model::LoadVertices] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 Model::LoadFaces( ZED_UINT64 p_Size )
		{
			// Convert the raw size into the amount of faces being loaded
			m_FaceCount = p_Size / sizeof( FACE_V1 );

			// New up and initialise the faces with the face data
			m_ppFaceIndices = new ZED_UINT16*[ m_FaceCount ];
			m_ppFaceNormals = new ZED_FLOAT32*[ m_FaceCount*3 ];

			for( ZED_MEMSIZE i = 0; i < m_FaceCount; i++ )
			{
				m_ppFaceNormals[ i ] = new ZED_FLOAT32[ 3 ];
				if( fread( m_ppFaceNormals, sizeof( ZED_FLOAT32 ), 3,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadFaces] <ERROR> "
						"Failed to load face normal data.\n" );
					return ZED_FAIL;
				}

				m_ppFaceIndices[ i ] = new ZED_UINT16[ 3 ];

				if( fread( m_ppFaceIndices[ i ], sizeof( ZED_UINT16 ), 3,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED::Renderer::Model::LoadFaces] <ERROR> "
						"Failed to load face index data.\n" );
					return ZED_FAIL;
				}
			}

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// The next one should be the end chunk
			if( Type != ZED_V1_END )
			{
				zedTrace( "[ZED::Renderer::Model::LoadFaces] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 Model::LoadMeshes( ZED_UINT64 p_Size )
		{
			ZED_UINT64 TempSize = p_Size;

			// The total indices stores the offeet for the next set of indices
			ZED_UINT16 TotalIndices = 0;
			ZED_UINT16 TotalWireIndices = 0;

			// TEMP!
			// Should be changed to load from the header
			m_pShader = new GLShader[ m_MeshCount ];
			// !TEMP

#ifdef ZED_BUILD_DEBUG
			m_ppMeshNames = new ZED_UCHAR8*[ m_MeshCount ];

			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
				m_ppMeshNames[ i ] = new ZED_UCHAR8[ 32 ];
			}
#endif
			// Read in each mesh, which has the format:
			// CHUNK
			// 	> CHAR Name[ 32 ]
			// 	> ZED_UINT16 MeshFaceIndices
			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
				// Store the mesh name for debugging purposes
#ifdef ZED_BUILD_DEBUG
				fread( m_ppMeshNames[ i ], sizeof( ZED_UCHAR8 ), 32, m_pFile );
#else
				fseek( m_pFile, sizeof( ZED_UCHAR8 )*32, SEEK_CUR );
#endif
				ZED_UINT16 Indices;
				// No error checking!
				fread( &Indices, sizeof( ZED_UINT16 ), 1, m_pFile );

				// Now go ahead and feed the indices into m_pIndices so they
				// can be imortalised!
				for( ZED_MEMSIZE j = 0; j < Indices; j++ )
				{
					ZED_UINT16 Face;
					// No error checking!
					fread( &Face, sizeof( ZED_UINT16 ), 1, m_pFile );

					m_pIndices[ ( j+TotalIndices )*3 ] =
						m_ppFaceIndices[ Face ][ 0 ];
					m_pIndices[ ( ( j+TotalIndices )*3 )+1 ] =
						m_ppFaceIndices[ Face ][ 1 ];
					m_pIndices[ ( ( j+TotalIndices )*3 )+2 ] =
						m_ppFaceIndices[ Face ][ 2 ];
				}

				TotalIndices += Indices;
#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::Model::LoadMeshes] <INFO> "
					"Mesh: %s | Indices: %d.\n", m_ppMeshNames[ i ], Indices );
#endif
			}
#ifdef ZED_BUILD_DEBUG
			zedTrace( "[ZED::Renderer::Model::LoadMeshes] <INFO> "
				"Loaded a total of %d indices.\n", TotalIndices );
#endif

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// The next chunk should be the end chunk
			if( Type != ZED_V1_END )
			{
				zedTrace( "[ZED::Renderer::Model::LoadMeshes] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		void Model::ReadChunk( ZED_UINT16 &p_Type, ZED_UINT64 &p_Size )
		{
			fread( &p_Type, sizeof( ZED_UINT16 ), 1, m_pFile );
			fread( &p_Size, sizeof( ZED_UINT64 ), 1, m_pFile );
		}
	}
}

