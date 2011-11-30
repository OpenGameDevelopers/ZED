#ifndef __ZED_RENDERER_VERTEXCACHEMANAGER_HPP__
#define __ZED_RENDERER_VERTEXCACHEMANAGER_HPP__

#include <Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		class VertexCacheManager
		{
		public:
			ZED_INLINE virtual ~VertexCacheManager( ){ }

			virtual ZED_UINT32 CreateCache( const ZED_MEMSIZE p_VertexCount,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_MEMSIZE p_AttributeCount,
				const ZED_UINT64 p_Attributes,
				const ZED_MEMSIZE p_LineCount ) = 0;

			virtual ZED_UINT32 Renderer( ) = 0;
		};
	}
}

#endif

