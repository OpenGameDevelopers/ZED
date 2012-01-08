#include <GLVertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLVertexCacheManager::GLVertexCacheManager( )
		{
			m_pCache = ZED_NULL;
			
			m_NumCaches = 0;

			m_DefaultVertexMaximum = 10000;
			m_DefaultIndexMaximum = 10000;
			m_DefaultLineCount = 10;

			m_pCacheAttributes = new ZED_UINT64[ m_DefaultLineCount ];

		}

		GLVertexCacheManager::GLVertexCacheManager(
			const ZED_MEMSIZE p_InitialCacheCount )
		{
			m_pCache = ZED_NULL;
			m_DefaultVertexMaximum = 10000;
			m_DefaultIndexMaximum = 10000;
			m_DefaultLineCount = p_InitialCacheCount;

			if( p_InitialCacheCount > 0 )
			{
				m_pCache = new GLVertexCache[ p_InitialCacheCount ];
				m_NumCaches = p_InitialCacheCount;
				m_pCacheAttributes = new ZED_UINT64[ m_DefaultLineCount ];
			}
		}

		GLVertexCacheManager::~GLVertexCacheManager( )
		{
			if( m_pCacheAttributes != ZED_NULL )
			{
				delete [ ] m_pCacheAttributes;
			}

			if( m_pCache != ZED_NULL )
			{
				delete [ ] m_pCache;
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

			// New-up the caches
			m_pCache = new GLVertexCache[ m_NumCaches+1 ];

			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				m_pCache[ i ] = GLVertexCache( MaxVertices[ i ],
					MaxIndices[ i ], AttributeCount[ i ], Attributes[ i ],
					CacheLines[ i ] );
				m_pCache[ i ].Initialise( );
			}

			m_pCache[ m_NumCaches ] = GLVertexCache( p_VertexCount,
				p_IndexCount, p_AttributeCount, p_Attributes, p_LineCount );
			m_pCache[ m_NumCaches ].Initialise( );
				

			// As a cache has been added. increment the counter
			m_NumCaches++;
			
			return ZED_OK;
		}

		ZED_UINT32 GLVertexCacheManager::Render(
			const ZED_MEMSIZE p_VertexCount, const ZED_BYTE *p_pVertices,
			const ZED_MEMSIZE p_IndexCount, const ZED_UINT16 *p_pIndices,
			const ZED_UINT64 p_Attributes, const ZED_UINT32 p_MaterialID )
		{
			// Just add the vertices to the cache, which will automatically
			// flush itself
			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				if( m_pCacheAttributes[ i ] == p_Attributes )
				{
					// Render
					return m_pCache[ i ].Add( p_VertexCount, p_pVertices,
						p_IndexCount, p_pIndices, p_MaterialID );
				}
			}

			// Okay, a new cache is needed.  Use the default cache sizes.
			// Before that; determine how many attributes are involved.
			ZED_MEMSIZE AttributeCount = 0;
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				// If the value is masked correctly, then that must be the size
				// otherwise it's the incremented count
				if( ( p_Attributes >> ( i*4 ) ) == 0)
				{
					break;
				}
				AttributeCount++;
			}

			if( this->CreateCache( m_DefaultVertexMaximum,
				m_DefaultIndexMaximum, AttributeCount, p_Attributes,
				m_DefaultLineCount ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			// Now that the cache is added, push the vertices into the new
			// cache
			m_pCache[ m_NumCaches-1 ].Add( p_VertexCount, p_pVertices,
				p_IndexCount, p_pIndices, p_MaterialID );

			return ZED_OK;
		}

		void GLVertexCacheManager::ForceFlushAll( )
		{
			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				m_pCache[ i ].Flush( );
			}
		}

		void GLVertexCacheManager::SetDefaultMaximumVertices(
			const ZED_MEMSIZE p_MaximumVertices )
		{
			m_DefaultVertexMaximum = p_MaximumVertices;
		}

		void GLVertexCacheManager::SetDefaultMaximumIndices(
			const ZED_MEMSIZE p_MaximumIndices )
		{
			m_DefaultIndexMaximum = p_MaximumIndices;
		}

		void GLVertexCacheManager::SetDefaultCacheLineCount(
			const ZED_MEMSIZE p_DefaultLineCount )
		{
			m_DefaultLineCount = p_DefaultLineCount;
		}
	}
}


