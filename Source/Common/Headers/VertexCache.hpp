#ifndef __ZED_RENDERER_VERTEXCACHE_HPP__
#define __ZED_RENDERER_VERTEXCACHE_HPP__

#include <Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		class VertexCache
		{
		public:
			ZED_INLINE virtual ~VertexCache( ){ }

			virtual ZED_UINT32 Initialise( ) = 0;

			virtual void Add( const ZED_MEMSIZE p_VertexCount,
				const void *p_pVertices,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices ) = 0;

			virtual void RemoveLine( ZED_MEMSIZE p_Index ) = 0;
			virtual void Clear( ) = 0;
			virtual void RemoveFullestLine( ) = 0;
			virtual void Flush( ) = 0;

			virtual ZED_UINT64 GetAttributes( ) = 0;
			virtual ZED_MEMSIZE GetAttributeCount( ) = 0;
			virtual ZED_MEMSIZE GetVertexCapacity( ) = 0;
			virtual ZED_MEMSIZE GetIndexCapacity( ) = 0;
			virtual ZED_MEMSIZE GetLineCount( ) = 0;

		private:
			virtual ZED_MEMSIZE AttribToSize( ZED_BYTE p_Attrib ) = 0;
		};
	}
}

#endif

