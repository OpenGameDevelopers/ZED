#ifndef __ZED_RENDERER_PRIMITIVE_HPP__
#define __ZED_RENDERER_PRIMITIVE_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/RenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class VertexAttributes;

		typedef enum __zedPRIMITIVE_DEBUG_TYPE_INDEX
		{
			PRIMITIVE_DEBUG_TYPE_INDEX_WIREFRAME	= 0,
			PRIMITIVE_DEBUG_TYPE_INDEX_NORMALS	= 1,
			PRIMITIVE_DEBUG_TYPE_INDEX_UNKNOWN	= ZED_ENUM_PADDING
		}PRIMITIVE_DEBUG_TYPE_INDEX;

		class Primitive
		{
		public:
			virtual ~Primitive( );
			
			virtual ZED_UINT32 Create( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices,
				const VertexAttributes &p_VertexAttributes,
				const PRIMITIVE_TYPE p_PrimitiveType ) = 0;

			virtual void Destroy( ) = 0;

			virtual ZED_UINT32 Render( ) = 0;

			// Debug
			virtual ZED_UINT32 RenderNormals( ) = 0;
			virtual ZED_UINT32 RenderWireframe( ) = 0;

		protected:
			ZED_MEMSIZE		m_PrimitiveCount;
			ZED_MEMSIZE		m_MemoryUsed;
			ZED_MEMSIZE		m_VertexCount;
			ZED_MEMSIZE		m_IndexCount;
			PRIMITIVE_TYPE	m_PrimitiveType;
		};
	}
}

#endif // __ZED_RENDERER_PRIMITIVE_HPP__

