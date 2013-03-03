#include <OGL/GLVertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLVertexCacheManager::GLVertexCacheManager( )
		{
			// Null pointers and set variables to sane values
			m_pCache = ZED_NULL;
			m_pCacheAttributes = ZED_NULL;

			m_NumCaches = 0;
			m_DefaultVertexMaximum = GLVC_MAXVERTS;
			m_DefaultIndexMaximum = GLVC_MAXINDICES;
			m_DefaultLineCount = GLVC_CACHELINES;
		}

		GLVertexCacheManager::GLVertexCacheManager(
			const ZED_MEMSIZE p_InitialCacheCount )
		{
			// Null pointers and set variables to sane values
			m_pCache = ZED_NULL;
			m_pCacheAttributes = ZED_NULL;

			m_NumCaches = p_InitialCacheCount;

			m_DefaultVertexMaximum = GLVC_MAXVERTS;
			m_DefaultIndexMaximum = GLVC_MAXINDICES;
			m_DefaultLineCount = GLVC_CACHELINES;
		}

		GLVertexCacheManager::~GLVertexCacheManager( )
		{
			if( m_pCache != ZED_NULL )
			{
				delete [ ] m_pCache;
				m_pCache = ZED_NULL;
			}

			if( m_pCacheAttributes != ZED_NULL )
			{
				delete [ ] m_pCacheAttributes;
				m_pCacheAttributes = ZED_NULL;
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
			ZED_UINT64 *pAttributes = new ZED_UINT64[ m_NumCaches ];
			ZED_MEMSIZE *pAttributeCount = new ZED_MEMSIZE[ m_NumCaches ];
			ZED_MEMSIZE *pMaxVertices = new ZED_MEMSIZE[ m_NumCaches ];
			ZED_MEMSIZE *pMaxIndices = new ZED_MEMSIZE[ m_NumCaches ];
			ZED_MEMSIZE *pCacheLines = new ZED_MEMSIZE[ m_NumCaches ];

			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				pAttributes[ i ] = m_pCache[ i ].GetAttributes( );
				pAttributeCount[ i ] = m_pCache[ i ].GetAttributeCount( );
				pMaxVertices[ i ] = m_pCache[ i ].GetVertexCapacity( );
				pMaxIndices[ i ] = m_pCache[ i ].GetIndexCapacity( );
				pCacheLines[ i ] = m_pCache[ i ].GetLineCount( );

				// Flush the caches so they can be deleted
				m_pCache[ i ].Flush( );
			}

			// Delete the caches
			delete [ ] m_pCache;

			// New-up the caches
			m_pCache = new GLVertexCache[ m_NumCaches+1 ];

			for( ZED_MEMSIZE i = 0; i < m_NumCaches; i++ )
			{
				m_pCache[ i ] = GLVertexCache( pMaxVertices[ i ],
					pMaxIndices[ i ], pAttributeCount[ i ], pAttributes[ i ],
					pCacheLines[ i ] );
				m_pCache[ i ].Initialise( );
			}

			m_pCache[ m_NumCaches ] = GLVertexCache( p_VertexCount,
				p_IndexCount, p_AttributeCount, p_Attributes, p_LineCount );
			m_pCache[ m_NumCaches ].Initialise( );

			// Increment the cache count
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
				if( m_pCache[ i ].GetAttributes( ) == p_Attributes )
				{
					return m_pCache[ i ].Add( p_VertexCount, p_pVertices,
						p_IndexCount, p_pIndices, p_MaterialID );
				}
			}

			// A new cache is needed.  Use the default cache sizes
			ZED_MEMSIZE AttributeCount = 0;
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				// If the value is masked correctly, then that must be the
				// size, otherwise it's the incremented count
				if( ( p_Attributes >> ( i*4 ) ) == 0 )
				{
					break;
				}
				AttributeCount++;
			}

			// If the vertices or indices wanted is greater than the default,
			// override the default to the desired count rounded up to the
			// nearest 1,000
			ZED_MEMSIZE VertexCount = m_DefaultVertexMaximum;
			ZED_MEMSIZE IndexCount = m_DefaultIndexMaximum;

			if( m_DefaultVertexMaximum < p_VertexCount )
			{
				// Round up the p_VertexCount to the nearest 1,000
				VertexCount = p_VertexCount + ( p_VertexCount % 1000 );
			}

			if( m_DefaultIndexMaximum < p_IndexCount )
			{
				// Round up the p_IndexCount to the nearest 1,000
				IndexCount = p_IndexCount + ( p_IndexCount % 1000 );
			}

			if( this->CreateCache( VertexCount, IndexCount, AttributeCount,
				p_Attributes, m_DefaultLineCount ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			// Now that the cache is added, push the vertices into the new
			// cache
			return m_pCache[ m_NumCaches-1 ].Add( p_VertexCount, p_pVertices,
				p_IndexCount, p_pIndices, p_MaterialID );
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
