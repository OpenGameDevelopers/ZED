#include <GLVertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLVertexCacheManager::GLVertexCacheManager( )
		{
		}

		GLVertexCacheManager::GLVertexCacheManager(
			const ZED_MEMSIZE p_InitialCacheCount )
		{
			if( p_InitialCacheCount > 0 )
			{
				m_pCache = new GLVertexCache[ p_InitialCacheCount ];
			}
		}

		ZED_UINT32 GLVertexCacheManager::CreateCache(
			const ZED_MEMSIZE p_VertexCount, const ZED_MEMSIZE p_IndexCount,
			const ZED_MEMSIZE p_AttributeCount, const ZED_UINT64 p_Attributes,
			const ZED_MEMSIZE p_LineCount )
		{
			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				if( m_pCache[ i ].GetAttributes( ) == p_Attributes )
				{
					// No need to create a new cache
					return ZED_OK;
				}
			}
			
			// Copy the caches over
			ZED_UINT64 Attributes[ m_NumCaches ];
			ZED_MEMSIZE AttributeCount[ m_NumCaches ];
			ZED_MEMSIZE MaxVertices[ m_NumCaches ];
			ZED_MEMSIZE MaxIndices[ m_NumCaches ];
			ZED_MEMSIZE CacheLines[ m_NumCaches ];

			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				Attributes[ i ] = m_pCache[ i ].GetAttributes( );
				AttributeCount[ i ] = m_pCache[ i ].GetAttributeCount( );
				MaxVertices[ i ] = m_pCache[ i ].GetVertexCapacity( );
				MaxIndices[ i ] = m_pCache[ i ].GetIndexCapacity( );
				CacheLines[ i ] = m_pCache[ i ].GetLineCount( );

				// Flush the caches so they can be deleted
				m_pCache[ i ].Flush( );
			}

			// Delete the caches
			delete [ ] m_pCache;

			// New-up the cache
			m_pCache = new GLVertexCache[ m_NumCaches+1 ];

			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				m_pCache[ i ] = GLVertexCache( MaxVertices[ i ],
					MaxIndices[ i ], AttributeCount[ i ], Attributes[ i ],
					CacheLines[ i ] );
			}

			m_pCache[ m_NumCaches ] = GLVertexCache( p_VertexCount,
				p_IndexCount, p_AttributeCount, p_Attributes, p_LineCount );
				

			// As a cache has been added. increment the counter
			m_NumCaches++;
			
			return ZED_OK;
		}
	}
}

