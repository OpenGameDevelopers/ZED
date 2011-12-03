#ifndef __ZED_RENDERER_GLVERTEXCACHE_HPP__
#define __ZED_RENDERER_GLVERTEXCACHE_HPP__

#include <VertexCache.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLVertexCache : public VertexCache
		{
		public:
			GLVertexCache( );
			GLVertexCache( const ZED_MEMSIZE p_VertexCount,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_MEMSIZE p_AttributeCount,
				const ZED_UINT64 p_VertexAttributes,
				const ZED_MEMSIZE p_CacheLines );
			~GLVertexCache( );

			virtual ZED_UINT32 Initialise( );

			virtual ZED_UINT32 Add( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices,
				const ZED_UINT32 p_MaterialID );

			virtual void FlushLine( const ZED_MEMSIZE p_Index );
			virtual void Clear( );
			virtual ZED_MEMSIZE FlushFullestLine( );
			virtual void Flush( );

			virtual ZED_UINT64 GetAttributes( );
			virtual ZED_MEMSIZE GetAttributeCount( );
			virtual ZED_MEMSIZE GetVertexCapacity( );
			virtual ZED_MEMSIZE GetIndexCapacity( );
			virtual ZED_MEMSIZE GetLineCount( );

		private:
			virtual ZED_MEMSIZE AttribToSize( ZED_BYTE p_Attrib );
			
			// The per-cache vertex and index counts
			ZED_MEMSIZE	*m_pVertexCount;
			ZED_MEMSIZE *m_pIndexCount;
			ZED_UINT32	*m_pMaterialID;

			ZED_MEMSIZE m_MaxVertices;
			ZED_MEMSIZE m_MaxIndices;
			ZED_MEMSIZE m_Stride;
			ZED_MEMSIZE m_CacheLines;

			ZED_MEMSIZE m_AttributeCount;
			ZED_UINT64	m_VertexAttributes;

			// Declare the OpenGL-specific members
			GLuint		*m_pVertexBuffer;
			GLuint		*m_pIndexBuffer;
			GLuint		m_VertexAttributeID;
		};
	}
}

#endif

