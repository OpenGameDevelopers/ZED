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
			m_ppStrips( ZED_NULL ),
			m_ppLists( ZED_NULL ),
			m_ppFans( ZED_NULL ),
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
				m_ppStrips = new ZED_UINT16*[ p_StripCount ];
				m_pStripIndexCount = new ZED_UINT32[ p_StripCount ];
			}
			if( p_ListCount > 0 )
			{
				m_ppLists = new ZED_UINT16*[ p_ListCount ];
				m_pListIndexCount = new ZED_UINT32[ p_ListCount ];
			}
			if( p_FanCount > 0 )
			{
				m_ppFans = new ZED_UINT16*[ p_FanCount ];
				m_pFanIndexCount = new ZED_UINT32[ p_FanCount ];
			}
		}

		Mesh::~Mesh( )
		{
			zedSafeDeleteArray( m_pVertices );
			zedSafeDeleteArray( m_pStripIndexCount );
			zedSafeDeleteArray( m_pListIndexCount );
			zedSafeDeleteArray( m_pFanIndexCount );
			zedSafeDelete2DArray( m_ppStrips, m_StripCount );
			zedSafeDelete2DArray( m_ppLists, m_ListCount );
			zedSafeDelete2DArray( m_ppFans, m_FanCount );
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
	}
}

