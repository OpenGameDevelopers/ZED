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

			virtual void Add( const ZED_MEMSIZE p_VertexCount,
				const void *p_pVertices,
				const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices );

			virtual void RemoveLine( const ZED_MEMSIZE p_Index );
			virtual void Clear( );
			virtual void RemoveFullestLine( );
			virtual void Flush( );

			virtual ZED_UINT64 GetAttributes( );
			virtual ZED_MEMSIZE GetAttributeCount( );
			virtual ZED_MEMSIZE GetVertexCapacity( );
			virtual ZED_MEMSIZE GetIndexCapacity( );
			virtual ZED_MEMSIZE GetLineCount( );

		private:
			virtual ZED_MEMSIZE AttribToSize( ZED_BYTE p_Attrib );
			
			// The per-cache vertex and index counts
			ZED_MEMSIZE	*m_VertexCount;
			ZED_MEMSIZE *m_IndexCount;
			ZED_MEMSIZE m_AttributeCount;

			ZED_MEMSIZE m_MaxVertices;
			ZED_MEMSIZE m_MaxIndices;
			ZED_MEMSIZE m_Stride;
			ZED_MEMSIZE m_CacheLines;

			void 		*m_pVertices;
			ZED_UINT16	*m_pIndices;
			ZED_UINT32	*m_MaterialID;

			ZED_UINT64	m_VertexAttributes;

			// Declare the OpenGL-specific members
			GLuint		*m_pVBO;
			GLuint		m_VAID;
		};
	}
}

#endif

