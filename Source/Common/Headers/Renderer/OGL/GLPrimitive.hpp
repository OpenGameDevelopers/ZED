#ifndef __ZED_RENDERER_GLPRIMITIVE_HPP__
#define __ZED_RENDERER_GLPRIMITIVE_HPP__

#include <Renderer/Primitive.hpp>
#include <GL/gl.h>

namespace ZED
{
	namespace Renderer
	{
		class GLPrimitive : public Primitive
		{
		public:
			GLPrimitive( );
			virtual ~GLPrimitive( );

			virtual ZED_UINT32 Create( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices,
				const VertexAttributes &p_VertexAttributes,
				const PRIMITIVE_TYPE p_PrimitiveType );

			virtual void Destroy( );

			virtual ZED_UINT32 Render( );

			virtual ZED_UINT32 RenderNormals( );
			virtual ZED_UINT32 RenderWireframe( );

		private:
			GLenum	m_GLPrimitiveType;

			GLuint	m_VertexBufferObject;
			GLuint	m_IndexBufferObject;
			GLuint	m_VertexArrayObject;

			GLuint	m_DebugVertexBufferObject[ 2 ];
			GLuint	m_DebugIndexBufferObject[ 2 ];
			GLuint	m_DebugVertexArrayObject[ 2 ];
		};
	}
}

#endif // __ZED_RENDERER_GLPRIMITIVE_HPP__

