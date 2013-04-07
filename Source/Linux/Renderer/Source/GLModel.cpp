#include <OGL/GLModel.hpp>
#include <OGL/GLShader.hpp>
#include <OGL/GLExtender.hpp>
#include <Quaternion.hpp>
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

			m_pJointBindPosition = ZED_NULL;
			m_pJointBindOrientation = ZED_NULL;
			m_pJointParents = ZED_NULL;

			m_MeshCount = 0;
			m_JointCount = 0;

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
			if( m_pVertices != ZED_NULL )
			{
				delete [ ] m_pVertices;
				m_pVertices = ZED_NULL;
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

			if( m_pJointBindPosition != ZED_NULL )
			{
				delete [ ] m_pJointBindPosition;
				m_pJointBindPosition = ZED_NULL;
			}

			if( m_pJointBindOrientation != ZED_NULL )
			{
				delete [ ] m_pJointBindOrientation;
				m_pJointBindOrientation = ZED_NULL;
			}

			if( m_pJointParents != ZED_NULL )
			{
				delete [ ] m_pJointParents;
				m_pJointParents = ZED_NULL;
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
					/*case ZED_MODEL_VERTEX:
					{
						if( this->LoadVertices( Size ) != ZED_OK )
						{
							return ZED_FAIL;
						}
						ReadChunk( Type, Size );
						break;
					}*/
					case ZED_MODEL_MESH:
					{
						if( this->LoadMeshes( Size ) != ZED_OK )
						{
							return ZED_FAIL;
						}
						// Get more
						ReadChunk( Type, Size );
						break;
					}
/*					case ZED_MODEL_ANIMATION:
					{
						if( this->LoadAnimation( Size ) != ZED_OK )
						{
							return ZED_FAIL;
						}
						ReadChunk( Type, Size );
						break;
					}*/
					default:
					{
						zedTrace( "[ZED::Renderer::Model::Load] <ERROR> "
							"Unknown type chunk.\n" );

						fclose( m_pFile );
						m_pFile = ZED_NULL;

						return ZED_FAIL;
					}
				}
			}

			fclose( m_pFile );
			m_pFile = ZED_NULL;

			return ZED_OK;
		}
		
		void GLModel::Render( )
		{
			for( ZED_MEMSIZE i = 0; i < m_MeshCount; i++ )
			{
/*
#ifdef ZED_BUILD_DEBUG
				zedTrace( "Rendering... "
					"m_pStride[ %d ] = %d | m_pVertexCount[ %d ] = %d | "
					"m_pIndexCount[ %d ] = %d\n", i,
					m_pStride[ i ], i, m_pVertexCount[ i ],
					i, m_pIndexCount[ i ] );
#endif*/
				for( ZED_MEMSIZE j = 0; j < m_pMesh[ i ].StripCount( ); ++j )
				{
/*					m_pRenderer->Render( m_pVertexCount[ i ], m_ppVertices[ i ],
						m_pIndexCount[ i ], m_ppIndices[ i ], m_pAttributes[ i ],
						m_pMaterialID[ i ], ZED_TRIANGLE_LIST );*/
				}

				for( ZED_MEMSIZE j = 0; j < m_pMesh[ i ].ListCount( ); ++j )
				{
					m_pRenderer->Render( m_pMesh[ i ].VertexCount( ),
						m_pMesh[ i ].Vertices( ),
						m_pMesh[ i ].ListIndexCount( j ),
						m_pMesh[ i ].List( j ),
						m_pMesh[ i ].Attributes( ),
						m_pMesh[ i ].MaterialID( ),
						ZED_TRIANGLE_LIST );
				}

				for( ZED_MEMSIZE j = 0; j < m_pMesh[ i ].FanCount( ); ++j )
				{
				}
				// Temporarily render just bones
/*				ZED_FLOAT32 *pShowJoints = ZED_NULL;
				pShowJoints = new ZED_FLOAT32[ 9*m_JointCount ];
				ZED_UINT16 *pTmpInds = ZED_NULL;
				pTmpInds = new ZED_UINT16[ 3*m_JointCount ];
				ZED_MEMSIZE TmpVerts = 9*m_JointCount;
				ZED_MEMSIZE TmpInds = 3*m_JointCount;
				ZED_MEMSIZE JointRunning = 0;
				
				for( ZED_MEMSIZE i = 0; i < 3*m_JointCount; i++ )
				{
					pTmpInds[ i ] = i;
				}

				for( ZED_MEMSIZE i = 0; i < 9*m_JointCount; i += 9 )
				{
					pShowJoints[ i ] =
						m_pJointBindPosition[ JointRunning ][ 0 ]+1.0f;
					pShowJoints[ i+1 ] = m_pJointBindPosition[ JointRunning ][ 1 ];
					pShowJoints[ i+2 ] = m_pJointBindPosition[ JointRunning ][ 2 ];

					pShowJoints[ i+3 ] =
						m_pJointBindPosition[ JointRunning ][ 0 ]-1.0f;
					pShowJoints[ i+4 ] = m_pJointBindPosition[ JointRunning ][ 1 ];
					pShowJoints[ i+5 ] = m_pJointBindPosition[ JointRunning ][ 2 ];

					pShowJoints[ i+6 ] =
						m_pJointBindPosition[ JointRunning+1 ][ 0 ];
					pShowJoints[ i+7 ] =
						m_pJointBindPosition[ JointRunning+1 ][ 1 ];
					pShowJoints[ i+8 ] =
						m_pJointBindPosition[ JointRunning+1 ][ 2 ];

					JointRunning++;
				}

				m_pRenderer->Render( TmpVerts, ( ZED_BYTE* )pShowJoints, TmpInds,
					pTmpInds, 0x6, 0 );

				delete [ ] pShowJoints;
				delete [ ] pTmpInds;*/
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
			const Arithmetic::Quaternion &p_Orientation )
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
			FILEHEADER_V2 HeaderInfo;
			MODEL_V2 ModelInfo;

			// Assume the file is already open
			ZED_UINT16 ChunkType;
			ZED_UINT64 ChunkSize;

			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_MODEL_HEADER )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Chunk is not a header chunk.\n" );
				zedTrace( "Size: %X | Type: %X.\n", ChunkSize, ChunkType );
				return ZED_FAIL;
			}

			fread( &HeaderInfo, sizeof( FILEHEADER_V2 ), 1, m_pFile );
			fread( &ModelInfo, sizeof( MODEL_V2 ), 1, m_pFile );

			// Make sure that this is in the correct format
			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_MODEL_END )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			if( HeaderInfo.ID[ 0 ] != 'Z' &&
				HeaderInfo.ID[ 1 ] != 'E' &&
				HeaderInfo.ID[ 2 ] != 'D' )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Header is not from a valid ZED model\n" );
				return ZED_FAIL;
			}

			if( HeaderInfo.Type != 'M' )
			{
				zedTrace( "[ZED::Renderer::GLModel::LoadHeader] <ERROR> "
					"Header is not for a ZED model\n" );
				return ZED_FAIL;
			}

			m_Flags = HeaderInfo.Flags;
			
			// Check for endian swapping
			if( m_Flags && 0x00000001 )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <INFO> "
					"Swapping bytes.\n" );
			}

//			m_MeshCount = HeaderInfo->MeshCount;
//			m_JointCount = HeaderInfo->JointCount;

#ifdef ZED_BUILD_DEBUG
			//strcpy( m_Name, HeaderInfo->Name );
/*			for( ZED_MEMSIZE i = 0; i < 32; i++ )
			{
				m_Name[ i ] = HeaderInfo->Name[ i ];
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
				"Header for: %s loaded successfully.\n", ModelInfo.Name );
			zedTrace( "\tEndianess: %s | Version: %d.%d.%d | ",
				//"Meshes: %d | Joints: %d\n",
				pEndianess,
				HeaderInfo.Version[ 0 ], HeaderInfo.Version[ 1 ],
				HeaderInfo.Version[ 2 ] );//,
//				HeaderInfo->MeshCount, HeaderInfo->JointCount );

			if( pEndianess != ZED_NULL )
			{
				delete [ ] pEndianess;
			}
#endif
			m_VertexCount = ModelInfo.VertexCount;

			return ZED_OK;
		}

		ZED_UINT32 GLModel::LoadVertices( const ZED_UINT64 p_Size )
		{
			m_pVertices = new ZED_BYTE[ m_VertexCount*sizeof( VERTEX_V2 ) ];

			fread( m_pVertices, sizeof( VERTEX_V2 ), m_VertexCount, m_pFile );

			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			if( Type != ZED_MODEL_END )
			{
				zedTrace( "Type: 0x%08X | Size: 0x%016X\n", Type, Size );
				zedTrace( "[ZED::Renderer::GLModel::LoadVertices] <ERROR> "
					"Unexpected data: Non-end chunk.\n" );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLModel::LoadMeshes( const ZED_UINT64 p_Size )
		{
			m_pMesh = new Mesh[ 1 ];

			MESH_V2 TmpMesh;
			memset( &TmpMesh, sizeof( MESH_V2 ), 0 );
			fread( &TmpMesh, sizeof( MESH_V2 ), 1, m_pFile );
			VERTEX_V2 *pTmpVerts = new VERTEX_V2[ TmpMesh.VertexCount ];
			ZED_UINT32 VertOffset = 0;

			for( ZED_UINT32 i = 0; i < TmpMesh.VertexCount; ++i )
			{
				fread( &pTmpVerts[ VertOffset*sizeof( VERTEX_V2 ) ],
					sizeof( VERTEX_V2 ), 1, m_pFile );
				++VertOffset;
			}

			m_pMesh[ 0 ].Vertices( ( ZED_BYTE * )pTmpVerts,
				TmpMesh.VertexCount );

			delete [ ] pTmpVerts;
			pTmpVerts = ZED_NULL;

			m_pMesh[ 0 ].MaterialID( TmpMesh.MaterialID );

			if( TmpMesh.Strips > 0 )
			{
			}

			if( TmpMesh.Lists > 0 )
			{
				m_pMesh[ 0 ].StripCount( TmpMesh.Lists );

				for( ZED_UINT32 i = 0; i < TmpMesh.Lists; ++i )
				{
					ZED_UINT16 IndexCount = 0;
					fread( &IndexCount, sizeof( ZED_UINT16 ), 1, m_pFile );
					ZED_UINT16 *pList = new ZED_UINT16[ IndexCount ];
					fread( pList, sizeof( ZED_UINT16 ), IndexCount, m_pFile );

					m_pMesh[ 0 ].List( pList, i, IndexCount );

					delete [ ] pList;
					pList = ZED_NULL;
				}
			}

			if( TmpMesh.Fans > 0 )
			{
			}

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

		ZED_UINT32 GLModel::LoadAnimation( const ZED_UINT64 p_Size )
		{
			// Divide the size by the size of a joint and
			// let's get crack-a-lackin'!
			m_JointCount = p_Size / sizeof( JOINT_V2 );

			m_pJointBindPosition = new Arithmetic::Vector3[ m_JointCount ];
			m_pJointBindOrientation =
				new Arithmetic::Quaternion[ m_JointCount ];
			m_pJointParents = new ZED_BYTE[ m_JointCount ];

			// Read in all joints
			for( ZED_MEMSIZE i = 0; i < m_JointCount; i++ )
			{
				fread( &m_pJointBindOrientation[ i ], sizeof( ZED_FLOAT32 ), 4,
					m_pFile );
				fread( &m_pJointBindPosition[ i ], sizeof( ZED_FLOAT32 ), 3,
					m_pFile );
				fread( &m_pJointParents[ i ], sizeof( ZED_BYTE ), 1, m_pFile );
			}
			
			ZED_UINT16 Type;
			ZED_UINT64 Size;
			ReadChunk( Type, Size );

			if( Type != ZED_MODEL_END )
			{
				zedTrace( "Type: 0x%08X | Size: 0x%016X\n", Type, Size );//m_JointCount );
				zedTrace( "[ZED::Renderer::Model::LoadAnimation] <ERROR> "
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

