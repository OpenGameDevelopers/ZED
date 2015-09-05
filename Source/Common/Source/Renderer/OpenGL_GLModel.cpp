#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLModel.hpp>
#include <Renderer/OGL/GLShader.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <System/Memory.hpp>
#include <Arithmetic/Quaternion.hpp>
#include <cstdio>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		GLModel::GLModel( const Renderer *p_pRenderer )
		{
			m_pFile = ZED_NULL;
			m_pMesh = ZED_NULL;
			m_World.Identity( );

			m_pJointBindPosition = ZED_NULL;
			m_pJointBindOrientation = ZED_NULL;
			m_pJointParents = ZED_NULL;

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
			zedSafeDeleteArray( m_pJointBindPosition );
			zedSafeDeleteArray( m_pJointBindOrientation );
			zedSafeDeleteArray( m_pJointParents );
			zedSafeDeleteArray( m_pMesh );
			zedSafeDeleteArray( m_pFile );
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
						fseek( m_pFile, -( sizeof( Type )+sizeof( Size ) ),
							SEEK_CUR );
						zedTrace( "[ZED::Renderer::Model::Load] <ERROR> "
							"Unknown type chunk: 0x%02X | Offset: 0x%08X\n",
							Type, ftell( m_pFile ) );

						fclose( m_pFile );
						m_pFile = ZED_NULL;

						return ZED_FAIL;
					}
				}
			}

			this->CalculateBoundingBox( );

			fclose( m_pFile );
			m_pFile = ZED_NULL;

			return ZED_OK;
		}
		
		void GLModel::Render( )
		{
			for( ZED_MEMSIZE i = 0; i < m_MeshCount; ++i )//m_MeshCount; i++ )
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
					m_pRenderer->CreateMesh( m_pMesh[ i ].VertexCount( ),
						m_pMesh[ i ].Vertices( ),
						m_pMesh[ i ].ListIndexCount( j ),
						m_pMesh[ i ].List( j ),
						0x66,//m_pMesh[ i ].Attributes( ),
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

		void GLModel::CalculateBoundingBox( )
		{
			Arithmetic::Vector3 Minimum, Maximum;
			ZED_FLOAT32 MinX, MinY, MinZ, MaxX, MaxY, MaxZ;

			zedTrace( "Bounding box for mesh 0:\n" );

			m_pMesh[ 0 ].CalculateBoundingBox( );
			m_pMesh[ 0 ].BoundingBox( &m_BoundingBox );

			m_BoundingBox.Min( &Minimum );
			m_BoundingBox.Max( &Maximum );

			if( m_MeshCount > 0 )
			{
				for( ZED_UINT32 i = 1; i < m_MeshCount; ++i )
				{
					zedTrace( "Bounding box for mesh %d\n", i );
					m_pMesh[ i ].CalculateBoundingBox( );
					ZED::Arithmetic::AABB Box;
					m_pMesh[ i ].BoundingBox( &Box );
					ZED::Arithmetic::Vector3 Min, Max;
					Box.Min( &Min );
					Box.Max( &Max );
					if( Min[ 0 ] < Minimum[ 0 ] )
					{
						Minimum[ 0 ] = Min[ 0 ];
					}
					if( Max[ 0 ] > Maximum[ 0 ] )
					{
						Maximum[ 0 ] = Max[ 0 ];
					}
					if( Min[ 1 ] < Minimum[ 1 ] )
					{
						Minimum[ 1 ] = Min[ 1 ];
					}
					if( Max[ 1 ] > Maximum[ 1 ] )
					{
						Maximum[ 1 ] = Max[ 1 ];
					}
					if( Min[ 2 ] < Minimum[ 2 ] )
					{
						Minimum[ 2 ] = Min[ 2 ];
					}
					if( Max[ 2 ] > Maximum[ 2 ] )
					{
						Maximum[ 2 ] = Max[ 2 ];
					}
				}
			}
			m_BoundingBox.Min( Minimum );
			m_BoundingBox.Max( Maximum );
			zedTrace( "Overall bounding box:\n" );
			zedTrace( "\tMinimum: %f | %f | %f\n", Minimum[ 0 ], Minimum[ 1 ],
				Minimum[ 2 ] );
			zedTrace( "\tMaximum: %f | %f | %f\n", Maximum[ 0 ], Maximum[ 1 ],
				Maximum[ 2 ] );
		}

		void GLModel::BoundingBox( Arithmetic::AABB *p_pBoundingBox )
		{
			( *p_pBoundingBox ) = m_BoundingBox;
		}

		void GLModel::BoundingBox( Arithmetic::AABB *p_pBoundingBox,
			const ZED_UINT32 p_MeshIndex )
		{
			if( p_MeshIndex < m_MeshCount && p_MeshIndex >= 0 )
			{
				m_pMesh[ p_MeshIndex ].BoundingBox( p_pBoundingBox );
			}
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

		void GLModel::ToggleBoundingBox( )
		{
			if( m_RenderBoundingBox )
			{
				m_RenderBoundingBox = ZED_FALSE;
			}
			else
			{
				m_RenderBoundingBox = ZED_TRUE;
			}
		}
#endif

		ZED_UINT32 GLModel::LoadHeader( )
		{
			FILEHEADER_V2 HeaderInfo;
			MODEL_V2 ModelInfo;
			ZED_UINT16 ChunkType;
			ZED_UINT64 ChunkSize;

			fread( &HeaderInfo, sizeof( FILEHEADER_V2 ), 1, m_pFile );

			if( HeaderInfo.Type != 'M' )
			{
				zedTrace( "[ZED::Renderer::GLModel::LoadHeader] <ERROR> "
					"Unexpected file type: %c\n", HeaderInfo.Type );
				return ZED_FAIL;
			}

			zedTrace( "Reading model meta data from: %d\n", ftell( m_pFile ) );

			// Make sure that this is in the correct format
			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_MODEL_METADATA )
			{
				zedTrace( "[ZED::Renderer::Model::LoadHeader] <ERROR> "
					"Unexpected data: 0x%02X %d bytes in the file.  "
					"Expected model metadata.\n",
					ChunkType,
					ftell( m_pFile ) );
				return ZED_FAIL;
			}

			fread( &ModelInfo, sizeof( MODEL_V2 ), 1, m_pFile );

			zedTrace( "\t[MODEL_V2]: %d bytes | [File]: %d bytes\n",
				sizeof( MODEL_V2 ), ChunkSize );

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

#ifdef ZED_BUILD_DEBUG
			zedTrace( "[ZED::Renderer::Model::LoadHeader] <INFO> "
				"Header for: %s loaded successfully.\n", ModelInfo.Name );
			zedTrace( "\t[ Version: %d.%d.%d ]\n"
				"\tMeshes: %d\n",
				//pEndianess,
				ModelInfo.Version[ 0 ], ModelInfo.Version[ 1 ],
				ModelInfo.Version[ 2 ],
				ModelInfo.MeshCount );
#endif
			m_MeshCount = ModelInfo.MeshCount;

			ReadChunk( ChunkType, ChunkSize );

			if( ChunkType != ZED_MODEL_END )
			{
				zedTrace( "[ZED::Renderer::GLModel::LoadHeader] <ERROR> "
					"Unexpected data: 0x%02X at location 0x%08X in the file.  "
					"Expected model end chunk\n",
					ChunkType,
					ftell( m_pFile )-sizeof( CHUNK ) );
				return ZED_FAIL;
			}

			m_pMesh = new Mesh[ m_MeshCount ];
			m_CurrentMesh = 0;

			return ZED_OK;
		}

		ZED_UINT32 GLModel::LoadMeshes( const ZED_UINT64 p_Size )
		{
			MESH_V2 TmpMesh;
			memset( &TmpMesh, sizeof( MESH_V2 ), 0 );
			fread( &TmpMesh, sizeof( MESH_V2 ), 1, m_pFile );
			ZED_BYTE *pTmpVerts =
				new ZED_BYTE[ sizeof( VERTEX_V2 )*TmpMesh.VertexCount ];
			ZED_UINT32 VertOffset = 0;

			zedTrace( "[ZED::Renderer::GLModel::LoadMeshes] <INFO> "
				"Mesh %d information:\n", m_CurrentMesh );
			zedTrace( "\tVertex Count: %d | MaterialID: %d | Strips: %d | "
				"Lists: %d | Fans: %d\n", TmpMesh.VertexCount,
				TmpMesh.MaterialID, TmpMesh.Strips, TmpMesh.Lists,
				TmpMesh.Fans );
			fread( pTmpVerts,//[ VertOffset*sizeof( VERTEX_V2 ) ],
					sizeof( VERTEX_V2 ), TmpMesh.VertexCount, m_pFile );
/*			for( ZED_UINT32 i = 0; i < TmpMesh.VertexCount; ++i )
			{
				fread( pTmpVerts,//[ VertOffset*sizeof( VERTEX_V2 ) ],
					sizeof( VERTEX_V2 ), TmpMesh.VertexCount, m_pFile );
//				++VertOffset;
			}*/

			m_pMesh[ m_CurrentMesh ].Vertices( pTmpVerts,
				TmpMesh.VertexCount );

			delete [ ] pTmpVerts;
			pTmpVerts = ZED_NULL;

			m_pMesh[ m_CurrentMesh ].MaterialID( TmpMesh.MaterialID );

			if( TmpMesh.Strips > 0 )
			{
			}

			if( TmpMesh.Lists > 0 )
			{
				if( TmpMesh.Lists == 1 )
				{
					zedTrace( "\tFound %d triangle list:\n", TmpMesh.Lists );
				}
				else
				{
					zedTrace( "\tFound %d triangle lists:\n", TmpMesh.Lists );
				}

				m_pMesh[ m_CurrentMesh ].ListCount( TmpMesh.Lists );

				for( ZED_UINT32 i = 0; i < TmpMesh.Lists; ++i )
				{
					ZED_UINT16 IndexCount = 0;
					fread( &IndexCount, sizeof( ZED_UINT16 ), 1, m_pFile );
					zedTrace( "\t\t[%d]: %d indices\n", i, IndexCount );
					ZED_UINT16 *pList = new ZED_UINT16[ IndexCount ];
					fread( pList, sizeof( ZED_UINT16 ), IndexCount, m_pFile );

					m_pMesh[ m_CurrentMesh ].List( pList, i, IndexCount );

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
					"Unexpected data: 0x%02X at location 0x%08X in the file.  "
					"Expected model end-chunk.\n",
					Type, ftell( m_pFile )-sizeof( CHUNK ) );
				return ZED_FAIL;
			}

			++m_CurrentMesh;

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
			fread( &p_Size, sizeof( ZED_UINT32 ), 1, m_pFile );
		}
	}
}
#endif // ZED_PLATFORM_SUPPORTS_OPENGL

