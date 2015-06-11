#ifndef __ZED_RENDERER_PRIMITIVE_HPP__
#define __ZED_RENDERER_PRIMITIVE_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Primitive
		{
		public:
			Primitive( );
			virtual ~Primitive( );
			
			ZED_UINT32 Create( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices,
				const VertexAttributes &p_VertexAttributes,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );
				

		protected:
			ZED_MEMSIZE	m_PolygonCount;
		};
	}
}

#endif // __ZED_RENDERER_PRIMITIVE_HPP__

