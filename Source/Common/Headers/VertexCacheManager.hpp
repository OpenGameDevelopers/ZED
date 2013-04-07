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

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Atrributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType ) = 0;

			virtual void ForceFlushAll( ) = 0;

			virtual void SetDefaultMaximumVertices(
				const ZED_MEMSIZE p_MaximumVertices ) = 0;
			virtual void SetDefaultMaximumIndices(
				const ZED_MEMSIZE p_MaximumIndices ) = 0;
			virtual void SetDefaultCacheLineCount(
				const ZED_MEMSIZE p_DefaultLineCount ) = 0;
		};
	}
}

#endif

