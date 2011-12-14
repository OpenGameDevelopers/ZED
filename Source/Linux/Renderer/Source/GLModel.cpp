#include <GLModel.hpp>
#include <GLShader.hpp>
#include <GLExtender.hpp>
#include <cstdio>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		GLModel::GLModel( const Renderer *p_pRenderer )
		{
			m_pFile = ZED_NULL;
			m_ppVertices = ZED_NULL;
			m_ppIndices = ZED_NULL;
			//m_pVCacheMan = ZED_NULL;

			m_World.Identity( );

			m_pVertexCount = ZED_NULL;
			m_pIndexCount = ZED_NULL;
			m_pMaterialID = ZED_NULL;
			m_pAttributes = ZED_NULL;
			m_MeshCount = 0;

			m_pRenderer = const_cast< Renderer * >( p_pRenderer );

#ifdef ZED_BUILD_DEBUG
			m_RenderWireframe = ZED_FALSE;
			m_RenderVNormals = ZED_FALSE;
			m_RenderFNormals = ZED_FALSE;
			m_RenderBones = ZED_FALSE;
#endif
		}

		GLModel::~GLModel( )
		{
			if( m_ppVertices != ZED_NULL )
			{
				for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
				{
					if( m_ppVertices[ i ] != ZED_NULL )
					{
						delete m_ppVertices[ i ];
						m_ppVertices[ i ] = ZED_NULL;
					}
				}
				delete [ ] m_ppVertices;
				m_ppVertices = ZED_NULL;
			}
			
			if( m_ppIndices != ZED_NULL )
			{
				for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
				{
					if( m_ppIndices[ i ] != ZED_NULL )
					{
						delete m_ppIndices[ i ];
						m_ppIndices[ i ] = ZED_NULL;
					}
				}
				delete [ ] m_ppIndices;
				m_ppIndices = ZED_NULL;
			}

			if( m_pFile != ZED_NULL )
			{
				fclose( m_pFile );
				m_pFile = ZED_NULL;
			}
		}

		ZED_UINT32 GLModel::Load( const ZED_CHAR8 *p_pFilename )
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
			while( Type != ZED_MODEL_END )
			{
				switch( Type )
				{
					case ZED_MODEL_MESH:
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

						fclose( m_pFile );
						m_pFile = ZED_NULL;

						return ZED_FAIL;
					}
				}
			}

			return ZED_OK;
		}
		
		void GLModel::Render( )
		{
			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
#ifdef ZED_BUILD_DEBUG
				zedTrace( "Rendering... "
					"m_pStride[ %d ] = %d | m_pVertexCount[ %d ] = %d\n", i,
					m_pStride[ i ], i, m_pVertexCount[ i ] );
#endif
					
				m_pRenderer->Render( m_pVertexCount[ i ],	m_ppVertices[ i ],
					m_pIndexCount[ i ], m_ppIndices[ i ], m_pAttributes[ i ],
					m_pMaterialID[ i ] );
			}
		}

		void GLModel::Update( const ZED_FLOAT32 p_Delta,
			const Arithmetic::Matrix4x4 &p_ViewProjection )
		{
		}

		void GLModel::SetPosition( const Arithmetic::Vector3 &p_Position )
		{
		}

		void GLModel::SetScale( const Arithmetic::Vector3 &p_Scale )
		{
		}

		void GLModel::SetOrientation(
			const Arithmetic::Matrix3x3 &p_Orientation )
		{
		}

#ifdef ZED_BUILD_DEBUG
		void GLModel::SetWireframeColour( const ZED_COLOUR &p_Colour )
		{
		}

		void GLModel::ToggleWireframe( )
		{
		}

		void GLModel::ToggleVertexNormals( )
		{
		}

		void GLModel::ToggleFaceNormals( )
		{
		}

		void GLModel::ToggleBones( )
		{
		}
#endif

		ZED_UINT32 GLModel::LoadHeader( )
		{
			LPFILEHEADER_V2 pHeaderInfo = new FILEHEADER_V2;

			// Assume the file is already open
			ZED_UINT16 ChunkType;
			ZED_UINT64 ChunkSize;

			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_MODEL_HEADER )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Chunk is not a header chunk.\n" );
				zedTrace( "Size: %X | Type: %X.\n", ChunkSize, ChunkType );
				delete pHeaderInfo;
				return ZED_FAIL;
			}

			fread( pHeaderInfo, sizeof( FILEHEADER_V2 ), 1, m_pFile );

			// Make sure that this is in the correct format
			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_MODEL_END )
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

			m_Flags = pHeaderInfo->Flags;
			
			// Check for endian swapping
			if( m_Flags && 0x00000001 )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <INFO> "
					"Swapping bytes.\n" );
			}

			m_MeshCount = pHeaderInfo->MeshCount;
			m_JointCount = pHeaderInfo->JointCount;

#ifdef ZED_BUILD_DEBUG
			//strcpy( m_Name, pHeaderInfo->Name );
/*			for( ZED_MEMSIZE i = 0; i < 32; i++ )
			{
				m_Name[ i ] = pHeaderInfo->Name[ i ];
			}*/
			ZED_CHAR8 *pEndianess = ZED_NULL;
			if( ( m_Flags & 0x00000001 ) )
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
				"Header"/* for: %s*/" loaded successfully.\n"/*, m_Name*/ );
			zedTrace( "\tEndianess: %s | Version: %d.%d.%d | "
				"Meshes: %d | Joints: %d\n", pEndianess,
				pHeaderInfo->Version[ 0 ], pHeaderInfo->Version[ 1 ],
				pHeaderInfo->Version[ 2 ],
				pHeaderInfo->MeshCount, pHeaderInfo->JointCount );

			if( pEndianess != ZED_NULL )
			{
				delete [ ] pEndianess;
			}
#endif
			if( pHeaderInfo != ZED_NULL )
			{
				delete pHeaderInfo;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLModel::LoadMeshes( ZED_UINT64 p_Size )
		{
			ZED_UINT64 TempSize = p_Size;

			// The total indices stores the offeet for the next set of indices
			ZED_UINT16 TotalIndices = 0;
			ZED_UINT16 TotalWireIndices = 0;

			// TEMP!
			// Should be changed to load from the header
			//m_pShader = new GLShader[ m_MeshCount ];
			// !TEMP
/*
#ifdef ZED_BUILD_DEBUG
			m_ppMeshNames = new ZED_UCHAR8*[ m_MeshCount ];
*/
			delete [ ] m_pAttributes;
			m_pAttributes = new ZED_UINT64[ m_MeshCount ];
			m_pMaterialID = new ZED_UINT32[ m_MeshCount ];
			m_pStride = new ZED_BYTE[ m_MeshCount ];
			m_pVertexCount = new ZED_UINT16[ m_MeshCount ];
			m_pIndexCount = new ZED_UINT16[ m_MeshCount ];
			m_ppVertices = new ZED_BYTE*[ m_MeshCount ];
			m_ppIndices = new ZED_UINT16*[ m_MeshCount ];
/*
			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
				m_ppMeshNames[ i ] = new ZED_UCHAR8[ 32 ];
			}
#endif*/
			// Read in each mesh, which has the format:
			// CHUNK
			// 	> CHAR Name[ 32 ]
			// 	> ZED_UINT16 MeshFaceIndices
			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
				// Store the mesh name for debugging purposes
#ifdef ZED_BUILD_DEBUG
				//fread( m_ppMeshNames[ i ], sizeof( ZED_UCHAR8 ), 32, m_pFile );
#else
				//fseek( m_pFile, sizeof( ZED_UCHAR8 )*32, SEEK_CUR );
#endif
				ZED_UINT16 Indices;
				fread( &m_pAttributes[ i ], sizeof( ZED_UINT64 ), 1, m_pFile );
				fread( &m_pMaterialID[ i ], sizeof( ZED_UINT32 ), 1, m_pFile );
				fread( &m_pVertexCount[ i ], sizeof( ZED_UINT16 ), 1,
					m_pFile );
				fread( &m_pIndexCount[ i ], sizeof( ZED_UINT16 ), 1, m_pFile );
				fread( &m_pStride[ i ], sizeof( ZED_BYTE ), 1, m_pFile );
#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLModel::LoadMeshes] <INFO> "
					"Loaded mesh %d\n\tAttributes: 0x%016X | Material ID: %d | "
					"Vertices: %d | Indices: %d\n", i, m_pAttributes[ i ],
					m_pMaterialID[ i ], m_pVertexCount[ i ],
					m_pIndexCount[ i ]);
#endif

				m_ppVertices[ i ] =
					new ZED_BYTE[ m_pVertexCount[ i ]*m_pStride[ i ] ];
				m_ppIndices[ i ] = new ZED_UINT16[ m_pIndexCount[ i ]*3 ];
				// No error checking!
				fread( m_ppVertices[ i ], sizeof( ZED_BYTE )*m_pStride[ i ],
					m_pVertexCount[ i ], m_pFile );
				fread( m_ppIndices[ i ], sizeof( ZED_UINT16 )*3, m_pIndexCount[ i ],
					m_pFile );	

				// Now go ahead and feed the indices into m_pIndices so they
				// can be imortalised!
				/*for( ZED_MEMSIZE j = 0; j < Indices; j++ )
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
				}*/

				TotalIndices += m_pIndexCount[ i ];
				/*
#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::Model::LoadMeshes] <INFO> "
					"Mesh: %s | Indices: %d.\n", m_ppMeshNames[ i ], Indices );
#endif*/
			}
#ifdef ZED_BUILD_DEBUG
			zedTrace( "[ZED::Renderer::Model::LoadMeshes] <INFO> "
				"Loaded a total of %d indices.\n", TotalIndices );
#endif

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			// The next chunk should be the end chunk
			if( Type != ZED_MODEL_END )
			{
				zedTrace( "[ZED::Renderer::Model::LoadMeshes] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		void GLModel::ReadChunk( ZED_UINT16 &p_Type, ZED_UINT64 &p_Size )
		{
			fread( &p_Type, sizeof( ZED_UINT16 ), 1, m_pFile );
			fread( &p_Size, sizeof( ZED_UINT64 ), 1, m_pFile );
		}
	}
}

