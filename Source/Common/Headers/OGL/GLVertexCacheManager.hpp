#ifndef __ZED_RENDERER_GLVERTEXCACHEMANAGER_HPP__
#define __ZED_RENDERER_GLVERTEXCACHEMANAGER_HPP__

#include <OGL/GLVertexCache.hpp>
#include <VertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLVertexCacheManager : public VertexCacheManager
		{
		public:
			GLVertexCacheManager( );
			GLVertexCacheManager( const ZED_MEMSIZE p_InitialCacheCount );
			~GLVertexCacheManager( );

			virtual ZED_UINT32 CreateCache( const ZED_MEMSIZE p_VertexCount,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_MEMSIZE p_AttributeCount,
				const ZED_UINT64 p_Attributes,
				const ZED_MEMSIZE p_LineCount );

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID );

			virtual void ForceFlushAll( );

			virtual void SetDefaultMaximumVertices(
				const ZED_MEMSIZE p_MaximumVertices );
			virtual void SetDefaultMaximumIndices(
				const ZED_MEMSIZE p_MaximumIndices );
			virtual void SetDefaultCacheLineCount(
				const ZED_MEMSIZE p_DefaultLineCount );

		private:
			GLVertexCache *m_pCache;

			ZED_UINT64	*m_pCacheAttributes;
			ZED_MEMSIZE	m_NumCaches;

			ZED_MEMSIZE m_DefaultVertexMaximum;
			ZED_MEMSIZE m_DefaultIndexMaximum;
			ZED_MEMSIZE m_DefaultLineCount;
		};
	}
}

#endif

