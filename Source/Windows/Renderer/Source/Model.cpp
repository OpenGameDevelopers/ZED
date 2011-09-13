#include <Model.hpp>
#include <GLShader.hpp>

namespace ZED
{
	namespace Renderer
	{
		Model::Model( )
		{
			m_pFile = ZED_NULL;
			m_pVertexData = ZED_NULL;
			m_pFaceData = ZED_NULL;
			m_SwapBytes = ZED_FALSE;
			m_pShader = ZED_NULL;

			m_WorldMatrix.Identity( );

#ifdef ZED_BUILD_DEBUG
			m_RenderWireframe = ZED_FALSE;
			m_pDebugShader = new GLShader( ZED_TRUE, ZED_TRUE, ZED_FALSE );
#endif
		}

		Model::~Model( )
		{
			if( m_pFile != ZED_NULL )
			{
				delete m_pFile;
				m_pFile = ZED_NULL;
			}

			if( m_pVertexData != ZED_NULL )
			{
				delete m_pVertexData;
				m_pVertexData = ZED_NULL;
			}

			if( m_pFaceData != ZED_NULL )
			{
				delete m_pFaceData;
				m_pFaceData = ZED_NULL;
			}

			/*for( ZED_MEMSIZE i = 0; i < m_FaceCount; i++ )
			{
				if( m_ppFaceIndices[ i ] != ZED_NULL )
				{
					delete m_ppFaceIndices[ i ];
					m_ppFaceIndices[ i ] = ZED_NULL;
				}
			}

			if( m_ppFaceIndices != ZED_NULL )
			{
				delete m_ppFaceIndices;
				m_ppFaceIndices = ZED_NULL;
			}*/

#ifdef ZED_BUILD_DEBUG
			if( m_pDebugShader != ZED_NULL )
			{
				delete m_pDebugShader;
			}
#endif
		}

		ZED_UINT32 Model::Load( const ZED_CHAR16 *p_pFileName )
		{
			m_pFile = _wfopen( p_pFileName, L"rb" );

			if( m_pFile == ZED_NULL )
			{
				zedTrace( "[ZED|Renderer|Model|Load] <ERROR> "
					"Failed to load model: %s\n.", p_pFileName );
				return ZED_FAIL;
			}

			// The first chunk MUST be the header
			LoadHeader( );

			ZED_UINT16 Type;
			ZED_UINT64 Size;

			ReadChunk( Type, Size );

			// Read all remaining chunks
			while( Type != ZED_V1_END ) 
			{
				if( Type == ZED_V1_VERTEX )
				{
					LoadVertices( Size );
				}
				else if( Type == ZED_V1_FACE )
				{
					LoadFaces( Size );
				}
				else if( Type == ZED_V1_MESH )
				{
					LoadMeshes( Size );
				}
				// More?
				ReadChunk( Type, Size );
			}

			fclose( m_pFile );
			
			zglGenVertexArrays( 1, m_pVertexArrayID );
			zglBindVertexArray( m_pVertexArrayID[ 0 ] );

			zglGenBuffers( 3, m_VertexBufferObjectID );
			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObjectID[ 0 ] );

			zglEnableVertexAttribArray( 0 );
			zglEnableVertexAttribArray( 1 );
			zglEnableVertexAttribArray( 2 );

			zglBufferData( GL_ARRAY_BUFFER, sizeof( float )*3*m_VertexCount, m_pVertices, GL_STATIC_DRAW );
			zglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObjectID[ 1 ] );
			zglBufferData( GL_ARRAY_BUFFER, sizeof( float )*2*m_VertexCount, m_pVertexUVs, GL_STATIC_DRAW );
			zglVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );

			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObjectID[ 2 ] );
			zglBufferData( GL_ARRAY_BUFFER, sizeof( float )*3*m_VertexCount, m_pVertexNormals, GL_STATIC_DRAW );
			zglVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, 0 );

			zglBindVertexArray( 0 );

#ifdef ZED_BUILD_DEBUG
			const char *pColourVertSrc[ ] = 
			{
				"#version 120\n" // OGL 2.1
				//"#version 330\n" // OGL 3.3
				"uniform mat4 WVP;\n"
				"attribute vec3 Position;\n"
				"attribute vec3 Colour;\n"
				"varying vec3 extColour;\n"
				"void main( )\n"
				"{\n"
				"	gl_Position = WVP*vec4( Position, 1.0f );\n"
				"	extColour = Colour;\n"
				"}\n"
			};

			const char *pColourFragSrc[ ] =
			{
				"#version 120\n" // OGL 2.1
				//"#version 330\n" // OGL 3.3
				"precision highp float;\n"
				"in vec3 extColour;\n"
				//"out vec4 oColour;\n"
				"void main( )\n"
				"{\n"
				"	gl_FragColor = vec4( extColour, 1.0f );\n" // oColour
				"}\n"
			};

			if( m_pDebugShader->Compile( pColourVertSrc, ZED_VERTEX_SHADER, ZED_FALSE ) !=
				ZED_OK )
			{
				zedAssert( ZED_FALSE );
			}
			if( m_pDebugShader->Compile( pColourFragSrc, ZED_FRAGMENT_SHADER, ZED_FALSE ) !=
				ZED_OK )
			{
				zedAssert( ZED_FALSE );
			}

			// Set up the shader input types
			ZED_SHADER_INPUT_MAP DebugShaderInput[ 1 ];

			DebugShaderInput[ 0 ].Index = 0;
			DebugShaderInput[ 0 ].pName = "WVP";
			DebugShaderInput[ 0 ].Type = ZED_MAT4X4;

			m_pDebugShader->SetVariableTypes( DebugShaderInput, 1 );

			// Create vertices for debug rendering information
			zglGenVertexArrays( 4, m_DebugVertexArrayID );
			zglBindVertexArray( m_DebugVertexArrayID[ 0 ] );
			zglGenBuffers( 4, m_DebugVBO );
			zglBindBuffer( GL_ARRAY_BUFFER, m_DebugVBO[ 0 ] );

			zglEnableVertexAttribArray( 0 );
			zglEnableVertexAttribArray( 1 );

			zglBufferData( GL_ARRAY_BUFFER,
				sizeof( ZED_FLOAT32 )*3*m_VertexCount, m_pVertices,
				GL_STATIC_DRAW );
			zglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

			m_pDebugWireColour = new ZED_FLOAT32[ 3*m_VertexCount ];

			for( ZED_MEMSIZE i = 0; i < m_VertexCount; i++ )
			{
				m_pDebugWireColour[ i*3 ] = 1.0f;
				m_pDebugWireColour[ i*3 +1 ] = 0.0f;
				m_pDebugWireColour[ i*3 +2 ] = 0.0f;
			}
			zglBindBuffer( GL_ARRAY_BUFFER, m_DebugVBO[ 1 ] );
			zglBufferData( GL_ARRAY_BUFFER,
				sizeof( ZED_FLOAT32 )*3*m_VertexCount, m_pDebugWireColour,
				GL_STATIC_DRAW );
			zglVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

			zglBindVertexArray( 0 );
#endif

			// The data is no longer required (though this will be different
			// when the Vertex Cache comes in
			if( m_pVertices )
			{
				delete m_pVertices;
				m_pVertices = ZED_NULL;
			}
			if( m_pVertexNormals )
			{
				delete m_pVertexNormals;
				m_pVertexNormals = ZED_NULL;
			}
			if( m_pVertexUVs )
			{
				delete m_pVertexUVs;
				m_pVertexUVs = ZED_NULL;
			}
			if( m_pFile != ZED_NULL )
			{
				m_pFile = ZED_NULL;
			}

			if( m_pVertexData != ZED_NULL )
			{
				delete m_pVertexData;
				m_pVertexData = ZED_NULL;
			}

			if( m_pFaceData != ZED_NULL )
			{
				delete m_pFaceData;
				m_pFaceData = ZED_NULL;
			}

			/*
			m_ppFaceIndices = new ZED_UINT16*[ m_FaceCount ];
			m_ppFaceNormals = new ZED_FLOAT32*[ m_FaceCount*3 ];

			for( ZED_MEMSIZE i = 0; i < ( m_FaceCount ); i++ )
			{
				m_ppFaceNormals[ i ] = new float[ 3 ];
				if( fread( m_ppFaceNormals, sizeof( float ), 3,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadFaces] <ERROR> "
						"Failed to load face data.\n" );
					return ZED_FAIL;
				}

				m_ppFaceIndices[ i ] = new ZED_UINT16[ 3 ];

				if( fread( m_ppFaceIndices[ i ], sizeof( ZED_UINT16 ), 3,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadFaces] <ERROR> "
						"Failed to load face data.\n" );
					return ZED_FAIL;
				}
			}*/

			for( ZED_MEMSIZE i = 0; i < m_FaceCount; i++ )
			{
				if( m_ppFaceIndices[ i ] != ZED_NULL )
				{
					delete m_ppFaceIndices[ i ];
					m_ppFaceIndices[ i ] = ZED_NULL;
				}
			}

			if( m_ppFaceIndices != ZED_NULL )
			{
				delete m_ppFaceIndices;
				m_ppFaceIndices = ZED_NULL;
			}
#ifdef ZED_BUILD_DEBUG
			m_pDebugWireframeIndices = new ZED_UINT16[ m_FaceCount*6 ];

			for( ZED_MEMSIZE i = 0; i < m_FaceCount; i++ )
			{
				m_pDebugWireframeIndices[ (i*6) ] = i;
				m_pDebugWireframeIndices[ (i*6)+1 ] = i+1;
				m_pDebugWireframeIndices[ (i*6)+2 ] = i+1;
				m_pDebugWireframeIndices[ (i*6)+3 ] = i+2;
				m_pDebugWireframeIndices[ (i*6)+4 ] = i+2;
				m_pDebugWireframeIndices[ (i*6)+5 ] = i;
			}
#endif
			
			return ZED_OK;
		}

		void Model::Render( )
		{
#ifdef ZED_BUILD_DEBUG
			// For debugging purposes, check if the model is to be rendered
			// as a wireframe mesh, the vertex normals, face normals, and bones
			if( m_RenderWireframe )
			{
				m_pDebugShader->Activate( );
				zglBindVertexArray( m_DebugVertexArrayID[ 0 ] );
				glDrawElements( GL_LINES, m_FaceCount*6, GL_UNSIGNED_SHORT,
					m_pDebugWireframeIndices );
			}
			if( !m_RenderWireframe )
			{
#endif
			zglBindVertexArray( m_pVertexArrayID[ 0 ] );
			glDrawElements( GL_TRIANGLES, 3*m_FaceCount, GL_UNSIGNED_SHORT,
				m_pIndices );

#ifdef	ZED_BUILD_DEBUG
			}
#endif
		}

		void Model::Update( const ZED_FLOAT32 p_Delta,
			const Arithmetic::Matrix4x4 &p_ViewProjection )
		{
			//m_WorldMatrix = m_Scale*m_Orientation*m_Position;
			ZED_FLOAT32 *pMatrix = new ZED_FLOAT32[ 16 ];
			pMatrix = ( p_ViewProjection*m_WorldMatrix ).GetMatrix( );
#ifdef ZED_BUILD_DEBUG
			if( m_RenderWireframe )
			{
				m_pDebugShader->Activate( );
				m_pDebugShader->SetVariable( 0, pMatrix );
			}
#endif
			delete pMatrix;
		}

		void Model::SetPosition( const Arithmetic::Vector3 &p_Position )
		{
			m_Position = p_Position;
			m_WorldMatrix.Translate( p_Position );
		}

		void Model::SetScale( const Arithmetic::Vector3 &p_Scale )
		{
			m_Scale = p_Scale;
		}

		void Model::SetOrientation( const Arithmetic::Matrix3x3 &p_Orientation )
		{
			m_Orientation.Copy( p_Orientation );
		}

		ZED_UINT32 Model::LoadHeader( )
		{
			LPFILEHEADER_V1 pHeaderInfo = new FILEHEADER_V1;
			// Assume file is already open
			ZED_UINT16 ChunkType;
			ZED_UINT64 ChunkSize;

			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_V1_HEADER )
			{
				zedTrace( "[ZED|Renderer|Model|LoadHeader] <ERROR> "
					"Chunk is not a header chunk.\n" );
				return ZED_FAIL;
			}

			fread( pHeaderInfo, sizeof( FILEHEADER_V1 ), 1, m_pFile );

			// Make sure that this is in the correct format
			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_V1_END )
			{
				zedTrace( "[ZED|Renderer|Model|LoadHeader] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			if( pHeaderInfo->ID[ 0 ] != 'Z' &&
				pHeaderInfo->ID[ 1 ] != 'E' &&
				pHeaderInfo->ID[ 2 ] != 'D' )
			{
				zedTrace( "[ZED|Renderer|Model|LoadHeader] <ERROR> "
					"Not a ZED Model.\n" );
				return ZED_FAIL;
			}

			if( pHeaderInfo->Endianess == 'E' )
			{
				m_SwapBytes = ZED_TRUE;
			}

			/*if( pHeaderInfo->Type != m_Type )
			{
				zedTrace( "[ZED|Renderer|Model|LoadHeader] <ERROR> "
					"Type difference.  Expecting %c, got %c.\n", m_Type,
					pHeaderInfo->Type );
				return ZED_FAIL;
			}*/

			m_MeshCount = pHeaderInfo->Meshes;
			m_FaceCount = pHeaderInfo->Faces;
			m_pIndices = new ZED_UINT16[ m_FaceCount*3 ];

#ifdef ZED_BUILD_DEBUG
			for( ZED_MEMSIZE i = 0; i < 32; i++ )
			{
				m_Name[ i ] = pHeaderInfo->Name[ i ];
			}

			// Need to add an extra triangle for the line looping
			m_pDebugWireframeIndices =
				new ZED_UINT16[ m_FaceCount*3+( m_MeshCount*3 ) ];
#endif

			m_IndexCount = pHeaderInfo->Indices;
			m_VertexCount = pHeaderInfo->Vertices;
			m_FaceCount = pHeaderInfo->Faces;

			return ZED_OK;
		}

		ZED_UINT32 Model::LoadVertices( ZED_UINT64 p_Size )
		{
			// Convert the raw size into the amount of vertices being loaded
			m_VertexCount = p_Size / sizeof( VERTEX_V1 );

			// New up the vertices and read in the values
			m_pVertices = new ZED_FLOAT32[ m_VertexCount * 3 ];
			m_pVertexNormals = new ZED_FLOAT32[ m_VertexCount * 3 ];
			m_pVertexUVs = new ZED_FLOAT32[ m_VertexCount * 2 ];

			for( ZED_MEMSIZE i = 0; i < m_VertexCount; i++ )
			{
				if( fread( &m_pVertices[ i*3 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertices[ ( i*3 )+1 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertices[ ( i*3 )+2 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}

				if( fread( &m_pVertexNormals[ ( i*3 ) ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertexNormals[ ( i*3 )+1 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertexNormals[ ( i*3 )+2 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertexUVs[ ( i*2 ) ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
				if( fread( &m_pVertexUVs[ ( i*2 )+1 ], sizeof( ZED_FLOAT32 ), 1,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
						"Failed to load vertex data.\n" );
					return ZED_FAIL;
				}
			}

			// Well, something's fucked up in the code...
			for( ZED_MEMSIZE i = 0; i < m_VertexCount; i++ )
			{
				zedTrace( "Vertex %d: %f %f %f\n", i, m_pVertices[ i*3 ],
					m_pVertices[ ( i*3 ) + 1 ],
					m_pVertices[ ( i*3 ) + 2 ] );
			}

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// It should end with the end chunk
			if( Type != ZED_V1_END )
			{
				zedTrace( "[ZED|Renderer|Model|LoadVertices] <ERROR> "
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

			for( ZED_MEMSIZE i = 0; i < ( m_FaceCount ); i++ )
			{
				m_ppFaceNormals[ i ] = new float[ 3 ];
				if( fread( m_ppFaceNormals, sizeof( float ), 3,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadFaces] <ERROR> "
						"Failed to load face data.\n" );
					return ZED_FAIL;
				}

				m_ppFaceIndices[ i ] = new ZED_UINT16[ 3 ];

				if( fread( m_ppFaceIndices[ i ], sizeof( ZED_UINT16 ), 3,
					m_pFile ) <= 0 )
				{
					zedTrace( "[ZED|Renderer|Model|LoadFaces] <ERROR> "
						"Failed to load face data.\n" );
					return ZED_FAIL;
				}
			}

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// It should end with the end chunk
			if( Type != ZED_V1_END )
			{
				zedTrace( "[ZED|Renderer|Model|LoadFaces] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 Model::LoadMeshes( ZED_UINT64 p_Size )
		{
			ZED_UINT32 TempSize = p_Size;

			// The total indices stores the offset for the next set of indices
			ZED_UINT16 TotalIndices = 0;
			ZED_UINT16 TotalWireIndices = 0;

			m_pShader = new GLShader[ m_MeshCount ];

#ifdef ZED_BUILD_DEBUG
			m_ppMeshNames = new ZED_UCHAR8*[ m_MeshCount ];

			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
				m_ppMeshNames[ i ] = new ZED_UCHAR8[ 32 ];
			}
#endif
			// Read in each mesh, which has the format:
			//	CHUNK
			//		> CHAR Name[ 32 ]
			//		> ZED_UINT16 MeshFaceIndices
			//	ZED_UINT16[ MeshFaceIndices ] FaceIndicesForTheMesh
			for( ZED_UINT32 i = 0; i < m_MeshCount; i++ )
			{
				// Store the mesh name for debugging purposes
#ifdef ZED_BUILD_DEBUG
				fread( m_ppMeshNames[ i ], sizeof( ZED_UCHAR8 ), 32, m_pFile );
#else
				fseek( m_pFile, sizeof( ZED_UCHAR8 )*32, SEEK_CUR );
#endif
				ZED_UINT16 Indices;
				fread( &Indices, sizeof( ZED_UINT16 ), 1, m_pFile );

				// Now go ahead and feed the indices into m_pIndices
				// so thay can be imortalised!
				for( ZED_MEMSIZE j = 0; j < Indices; j++ )
				{
					ZED_UINT16 Face;
					fread( &Face, sizeof( ZED_UINT16 ),
						1, m_pFile );

					m_pIndices[ ( j+TotalIndices )*3 ] = m_ppFaceIndices[ Face ][ 0 ];
					m_pIndices[ ( ( j+TotalIndices )*3 )+1 ] = m_ppFaceIndices[ Face ][ 1 ];
					m_pIndices[ ( ( j+TotalIndices )*3 )+2 ] = m_ppFaceIndices[ Face ][ 2 ];
				}

				TotalIndices += Indices;
			}

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// It should end with the end chunk
			if( Type != ZED_V1_END )
			{
				zedTrace( "[ZED|Renderer|Model|LoadMeshes] <ERROR> "
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

#ifdef ZED_BUILD_DEBUG
		void Model::ToggleWireframe( )
		{
			m_RenderWireframe = !m_RenderWireframe;
		}

		void Model::SetWireframeColour( const ZED_COLOUR &p_Colour )
		{
			m_WireColour = p_Colour;
		}
#endif
	}
}
