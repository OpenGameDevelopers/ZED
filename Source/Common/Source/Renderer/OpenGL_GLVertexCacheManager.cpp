#ifdef ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLVertexCacheManager.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLVertexCacheManager::GLVertexCacheManager( )
		{
			m_DefaultVertexMaximum = 100000;
			m_DefaultIndexMaximum = 100000;
			m_DefaultLineCount = 10;

			m_pCacheList = ZED_NULL;
		}

		GLVertexCacheManager::~GLVertexCacheManager( )
		{
		}

		VertexCache *GLVertexCacheManager::CreateCache(
			const ZED_MEMSIZE p_VertexCount, const ZED_MEMSIZE p_IndexCount,
			const ZED_MEMSIZE p_AttributeCount, const ZED_UINT64 p_Attributes,
			const ZED_MEMSIZE p_LineCount )
		{
			if( m_pCacheList )
			{
				CACHELIST *pLast = m_pCacheList;
				
				// Keep trying to get the next cache until there are no more
				// left
				while( pLast )
				{
					if( pLast->pNext )
					{
						pLast = pLast->pNext;
					}
					else
					{
						break;
					}
				}

				// Create a new cache and set the last cache's next pointer to
				// the newly-created cache
				CACHELIST *pNext = new CACHELIST;
				pNext->pCache = new GLVertexCache( p_VertexCount,
					p_IndexCount, p_AttributeCount, p_Attributes,
					p_LineCount );
				
				if( pNext->pCache->Initialise( ) != ZED_OK )
				{
					zedSafeDelete( pNext );

					zedTrace( "[ZED::Renderer::GLVertexCacheManager::"
						"CreateCache <ERROR> Failed to initialise cache\n" );

					return ZED_NULL;
				}

				pNext->pNext = ZED_NULL;

				pLast->pNext = pNext;

				return pNext->pCache;
			}

			// There was no cache list, create a new one
			m_pCacheList = new CACHELIST;

			m_pCacheList->pCache = new GLVertexCache( p_VertexCount,
				p_IndexCount, p_AttributeCount, p_Attributes,
				p_LineCount );
			m_pCacheList->pNext = ZED_NULL;

			if( m_pCacheList->pCache->Initialise( ) != ZED_OK )
			{
				zedSafeDelete( m_pCacheList );

				zedTrace( "[ZED::Renderer::GLVertexCacheManager::"
					"CreateCache <ERROR> Failed to initialise cache\n" );

				return ZED_NULL;
			}

			return m_pCacheList->pCache;
		}

		ZED_UINT32 GLVertexCacheManager::Render(
			const ZED_MEMSIZE p_VertexCount, const ZED_BYTE *p_pVertices,
			const ZED_MEMSIZE p_IndexCount, const ZED_UINT16 *p_pIndices,
			const ZED_UINT64 p_Attributes, const ZED_UINT32 p_MaterialID,
			const ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			// Just add the vertices to the cache, which will automatically
			// flush itself
			CACHELIST *pCache = m_pCacheList;
			while( pCache )
			{
				if( pCache->pCache->GetAttributes( ) == p_Attributes )
				{
					return pCache->pCache->Add( p_VertexCount, p_pVertices,
						p_IndexCount, p_pIndices, p_MaterialID,
						p_PrimitiveType );
				}
				pCache = m_pCacheList->pNext;
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

			// If the vertices or indices wanted is greater than the default,
			// override the  default to the desired count rounded up to the
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

			VertexCache *pGLCache = this->CreateCache( VertexCount,
				IndexCount, AttributeCount,	p_Attributes, m_DefaultLineCount );

			if( pGLCache == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GLVertexCacheManager::Render] "
					"<ERROR> Failed to create cache\n" );

				return ZED_FAIL;
			}

			// Now that the cache is added, push the vertices into the new
			// cache
			pGLCache->Add( p_VertexCount, p_pVertices, p_IndexCount,
				p_pIndices, p_MaterialID, p_PrimitiveType );

			return ZED_OK;
		}

		void GLVertexCacheManager::ForceFlushAll( )
		{
			CACHELIST *pCache = m_pCacheList;
			while( pCache )
			{
				pCache->pCache->Flush( );
				pCache = pCache->pNext;
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
#endif // ZED_PLATFORM_SUPPORTS_OPENGL

