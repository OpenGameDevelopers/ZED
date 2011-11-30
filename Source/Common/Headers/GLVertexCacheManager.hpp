#ifndef __ZED_RENDERER_GLVERTEXCACHEMANAGER_HPP__
#define __ZED_RENDERER_GLVERTEXCACHEMANAGER_HPP__

#include <GLVertexCache.hpp>
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
				const ZED_UINT64 p_Type,
				const ZED_MEMSIZE p_LineCount );

			virtual ZED_UINT32 Render( );

		private:
			GLVertexCache *m_pCache;

			ZED_MEMSIZE	m_NumCaches;
		};
	}
}

#endif

