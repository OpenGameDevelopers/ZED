#include <Renderer/Mesh.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		Mesh::Mesh( ) :
			m_StripCount( 0 ),
			m_ListCount( 0 ),
			m_FanCount( 0 ),
			m_ppStripIndices( ZED_NULL ),
			m_ppListIndices( ZED_NULL ),
			m_ppFanIndices( ZED_NULL ),
			m_pStripIndexCount( ZED_NULL ),
			m_pListIndexCount( ZED_NULL ),
			m_pFanIndexCount( ZED_NULL ),
			m_pVertices( ZED_NULL )
		{
		}

		Mesh::Mesh( const ZED_UINT32 p_StripCount,
			const ZED_UINT32 p_ListCount, const ZED_UINT32 p_FanCount ) :
				m_StripCount( p_StripCount ),
				m_ListCount( p_ListCount ),
				m_FanCount( p_FanCount ),
				m_pVertices( ZED_NULL )
		{
			if( p_StripCount > 0 )
			{
				m_ppStripIndices = new ZED_UINT16*[ p_StripCount ];
				m_pStripIndexCount = new ZED_UINT32[ p_StripCount ];
			}
			if( p_ListCount > 0 )
			{
				m_ppListIndices = new ZED_UINT16*[ p_ListCount ];
				m_pListIndexCount = new ZED_UINT32[ p_ListCount ];
			}
			if( p_FanCount > 0 )
			{
				m_ppFanIndices = new ZED_UINT16*[ p_FanCount ];
				m_pFanIndexCount = new ZED_UINT32[ p_FanCount ];
			}
		}

		Mesh::~Mesh( )
		{
			zedSafeDeleteArray( m_pVertices );
			zedSafeDeleteArray( m_pStripIndexCount );
			zedSafeDeleteArray( m_pListIndexCount );
			zedSafeDeleteArray( m_pFanIndexCount );
			zedSafeDelete2DArray( m_ppStripIndices, m_StripCount );
			zedSafeDelete2DArray( m_ppListIndices, m_ListCount );
			zedSafeDelete2DArray( m_ppFanIndices, m_FanCount );
		}

		ZED_UINT32 Mesh::SetVertices( const ZED_BYTE *p_pVertices,
			const ZED_UINT32 p_VertexCount )
		{
			if( p_pVertices )
			{
				zedSafeDeleteArray( m_pVertices );
			}
			else
			{
				return ZED_FAIL;
			}

			ZED_MEMSIZE VertexSize = sizeof( VERTEX_V2 ) * p_VertexCount;

			m_pVertices = new ZED_BYTE[ VertexSize ];
			memcpy( m_pVertices, p_pVertices, VertexSize );
			m_VertexCount = p_VertexCount;

			return ZED_OK;
		}

		ZED_BYTE *Mesh::GetVertices( ) const
		{
			return m_pVertices;
		}

		ZED_UINT32 Mesh::SetStripIndices( const ZED_UINT16 *p_pStripIndices,
			const ZED_MEMSIZE p_Index, const ZED_UINT32 p_Count )
		{
			if( p_Index > ( m_ListCount + 1 ) )
			{
				return ZED_FAIL;
			}

			if( m_ppStripIndices )
			{
				m_ppStripIndices[ p_Index ] = new ZED_UINT16[ p_Count ];

				memcpy( m_ppStripIndices[ p_Index ], p_pStripIndices,
					p_Count );
				m_pStripIndexCount[ p_Index ] = p_Count;

				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 Mesh::SetListIndices( const ZED_UINT16 *p_pListIndices,
			const ZED_MEMSIZE p_Index, const ZED_UINT32 p_Count )
		{
			if( p_Index > ( m_ListCount + 1 ) )
			{
				return ZED_FAIL;
			}

			if( m_ppListIndices )
			{
				m_ppListIndices[ p_Index ] = new ZED_UINT16[ p_Count ];

				memcpy( m_ppListIndices[ p_Index ], p_pListIndices, p_Count );
				m_pListIndexCount[ p_Index ] = p_Count;

				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 Mesh::SetFanIndices( const ZED_UINT16 *p_pFanIndices,
			const ZED_MEMSIZE p_Index, const ZED_UINT32 p_Count )
		{
			if( p_Index > ( m_FanCount + 1 ) )
			{
				return ZED_FAIL;
			}

			if( m_ppFanIndices )
			{
				m_ppFanIndices[ p_Index ] = new ZED_UINT16[ p_Count ];

				memcpy( m_ppFanIndices[ p_Index ], p_pFanIndices, p_Count );
				m_pFanIndexCount[ p_Index ] = p_Count;

				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 Mesh::GetStripIndices( const ZED_MEMSIZE p_Index, 
			ZED_UINT16 **p_ppStripIndices ) const
		{
			( *p_ppStripIndices ) = m_ppStripIndices[ p_Index ];
			return ZED_OK;
		}

		ZED_UINT32 Mesh::GetListIndices( const ZED_MEMSIZE p_Index,
			ZED_UINT16 **p_ppListIndices ) const
		{
			( *p_ppListIndices ) = m_ppListIndices[ p_Index ];
			return ZED_OK;
		}

		ZED_UINT32 Mesh::GetFanIndices( const ZED_MEMSIZE p_Index,
			ZED_UINT16 **p_ppFanIndices ) const
		{
			( *p_ppFanIndices ) = m_ppFanIndices[ p_Index ];
			return ZED_OK;
		}

		ZED_UINT32 Mesh::CopyStripIndices( const ZED_MEMSIZE p_Index,
			ZED_UINT16 **p_ppStripIndices ) const
		{
			memcpy( ( *p_ppStripIndices ), m_ppStripIndices[ p_Index ],
				m_pStripIndexCount[ p_Index ] );
			return ZED_OK;
		}

		ZED_UINT32 Mesh::CopyListIndices( const ZED_MEMSIZE p_Index,
			ZED_UINT16 **p_ppListIndices ) const
		{
			memcpy( ( *p_ppListIndices ), m_ppListIndices[ p_Index ],
				m_pListIndexCount[ p_Index ] );
			return ZED_OK;
		}

		ZED_UINT32 Mesh::CopyFanIndices( const ZED_MEMSIZE p_Index,
			ZED_UINT16 **p_ppFanIndices ) const
		{
			memcpy( ( *p_ppFanIndices ), m_ppFanIndices[ p_Index ],
				m_pFanIndexCount[ p_Index ] );
			return ZED_OK;
		}

		void Mesh::SetStripCount( const ZED_UINT32 p_StripCount )
		{
			zedSafeDelete2DArray( m_ppStripIndices, m_StripCount );
			zedSafeDeleteArray( m_pStripIndexCount );

			m_ppStripIndices = new ZED_UINT16*[ p_StripCount ];
			m_pStripIndexCount = new ZED_UINT32[ p_StripCount ];
			m_StripCount = p_StripCount;
		}

		void Mesh::SetListCount( const ZED_UINT32 p_ListCount )
		{
			zedSafeDelete2DArray( m_ppListIndices, m_ListCount );
			zedSafeDeleteArray( m_pListIndexCount );

			m_ppListIndices = new ZED_UINT16*[ p_ListCount ];
			m_pListIndexCount = new ZED_UINT32[ p_ListCount ];
			m_ListCount = p_ListCount;
		}

		void Mesh::SetFanCount( const ZED_UINT32 p_FanCount )
		{
			zedSafeDelete2DArray( m_ppFanIndices, m_FanCount );
			zedSafeDeleteArray( m_pFanIndexCount );

			m_ppFanIndices = new ZED_UINT16*[ p_FanCount ];
			m_pFanIndexCount = new ZED_UINT32[ p_FanCount ];
			m_FanCount = p_FanCount;
		}

		void Mesh::CalculateBoundingBox( )
		{
			if( m_VertexCount < 4 )
			{
				return;
			}

			VERTEX_V2 Vertices[ m_VertexCount ];

			memcpy( Vertices, m_pVertices, sizeof( VERTEX_V2 )*m_VertexCount );

			ZED::Arithmetic::Vector3 Min(
				Vertices[ 0 ].Position[ 0 ],
				Vertices[ 0 ].Position[ 1 ],
				Vertices[ 0 ].Position[ 2 ] );
			ZED::Arithmetic::Vector3 Max(
				Vertices[ 0 ].Position[ 0 ],
				Vertices[ 0 ].Position[ 1 ],
				Vertices[ 0 ].Position[ 2 ] );

			for( ZED_UINT32 i = 1; i < m_VertexCount; ++i )
			{
				if( Vertices[ i ].Position[ 0 ] > Max[ 0 ] )
				{
					Max[ 0 ] = Vertices[ i ].Position[ 0 ];
				}
				if( Vertices[ i ].Position[ 1 ] > Max[ 1 ] )
				{
					Max[ 1 ] = Vertices[ i ].Position[ 1 ];
				}
				if( Vertices[ i ].Position[ 2 ] > Max[ 2 ] )
				{
					Max[ 2 ] = Vertices[ i ].Position[ 2 ];
				}

				if( Vertices[ i ].Position[ 0 ] < Min[ 0 ] )
				{
					Min[ 0 ] = Vertices[ i ].Position[ 0 ];
				}
				if( Vertices[ i ].Position[ 1 ] < Min[ 1 ] )
				{
					Min[ 1 ] = Vertices[ i ].Position[ 1 ];
				}
				if( Vertices[ i ].Position[ 2 ] < Min[ 2 ] )
				{
					Min[ 2 ] = Vertices[ i ].Position[ 2 ];
				}
			}

			m_BoundingBox.Min( Min );
			m_BoundingBox.Max( Max );
		}

		ZED_UINT32 Mesh::GetBoundingBox(
			ZED::Arithmetic::AABB *p_pBoundingBox ) const
		{
			if( p_pBoundingBox == ZED_NULL )
			{
				return ZED_FAIL;
			}

			( *p_pBoundingBox ) = m_BoundingBox;

			return ZED_OK;
		}

		ZED_UINT32 Mesh::GetStripCount( ) const
		{
			return m_StripCount;
		}

		ZED_UINT32 Mesh::GetListCount( ) const
		{
			return m_ListCount;
		}

		ZED_UINT32 Mesh::GetFanCount( ) const
		{
			return m_FanCount;
		}

		ZED_UINT32 Mesh::GetStripIndexCount( const ZED_MEMSIZE p_Index ) const
		{
			return m_pStripIndexCount[ p_Index ];
		}

		ZED_UINT32 Mesh::GetListIndexCount( const ZED_MEMSIZE p_Index ) const
		{
			return m_pListIndexCount[ p_Index ];
		}

		ZED_UINT32 Mesh::GetFanIndexCount( const ZED_MEMSIZE p_Index ) const
		{
			return m_pFanIndexCount[ p_Index ];
		}

		ZED_UINT32 Mesh::GetVertexCount( ) const
		{
			return m_VertexCount;
		}

		void Mesh::SetAttributes( const ZED_UINT64 p_Attributes )
		{
			m_Attributes = p_Attributes;
		}

		ZED_UINT64 Mesh::GetAttributes( ) const
		{
			return m_Attributes;
		}

		void Mesh::SetMaterialID( const ZED_UINT32 p_MaterialID )
		{
			m_MaterialID = p_MaterialID;
		}

		ZED_UINT32 Mesh::GetMaterialID( ) const
		{
			return m_MaterialID;
		}
	}
}

