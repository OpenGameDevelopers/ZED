#ifndef __ZED_RENDERER_GLVERTEXCACHEMANAGER_HPP__
#define __ZED_RENDERER_GLVERTEXCACHEMANAGER_HPP__

#include <Renderer/OGL/GLVertexCache.hpp>
#include <Renderer/VertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		typedef struct ZED_CACHELIST
		{
			GLVertexCache	*pCache;
			ZED_CACHELIST	*pNext;
		}CACHELIST,*LPCACHELIST;

		class GLVertexCacheManager : public VertexCacheManager
		{
		public:
			GLVertexCacheManager( );
			~GLVertexCacheManager( );

			virtual VertexCache *CreateCache( const ZED_MEMSIZE p_VertexCount,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_MEMSIZE p_AttributeCount,
				const ZED_UINT64 p_Attributes,
				const ZED_MEMSIZE p_LineCount );

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

			virtual void ForceFlushAll( );

			virtual void SetDefaultMaximumVertices(
				const ZED_MEMSIZE p_MaximumVertices );
			virtual void SetDefaultMaximumIndices(
				const ZED_MEMSIZE p_MaximumIndices );
			virtual void SetDefaultCacheLineCount(
				const ZED_MEMSIZE p_DefaultLineCount );

		private:
			CACHELIST	*m_pCacheList;

			ZED_MEMSIZE m_DefaultVertexMaximum;
			ZED_MEMSIZE m_DefaultIndexMaximum;
			ZED_MEMSIZE m_DefaultLineCount;
		};
	}
}

#endif

