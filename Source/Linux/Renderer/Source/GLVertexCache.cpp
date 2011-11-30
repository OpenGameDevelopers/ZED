#include <GLVertexCache.hpp>
#include <GLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLVertexCache::GLVertexCache( )
		{
		}

		GLVertexCache::GLVertexCache( const ZED_MEMSIZE p_VertexCount,
			const ZED_MEMSIZE p_IndexCount, const ZED_MEMSIZE p_AttributeCount,
			const ZED_UINT64 p_VertexAttributes,
			const ZED_MEMSIZE p_CacheLines )
		{
			// Null any pointers and initialise all variables to sane values
			m_pVertices = ZED_NULL;
			m_pIndices = ZED_NULL;
			m_pVBO = ZED_NULL;

			m_VAID = 0;
	
			// Give default values if not present
			m_MaxVertices = p_VertexCount ? p_VertexCount : 1000;
			m_MaxIndices = p_IndexCount ? p_IndexCount : 1000;
			m_AttributeCount = p_AttributeCount ? p_AttributeCount : 1;
			m_VertexAttributes = p_VertexAttributes ?
				p_VertexAttributes : 0x06; // 01 == ZED_FLOAT32 | 10 == 3D
			m_CacheLines = p_CacheLines ? p_CacheLines : 1;

			m_VertexCount = new ZED_MEMSIZE[ m_CacheLines ];
			m_IndexCount = new ZED_MEMSIZE[ m_CacheLines ];
			m_MaterialID = new ZED_UINT32[ m_CacheLines ];


			// Get the stride from the Attribs passed in
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{	
				// As an attribute is a four-bit value, extract a half byte
				// from the attributes pushed in
				ZED_BYTE Attrib = m_VertexAttributes >> ( i*4 );

				m_Stride += AttribToSize( Attrib );
			}

			// Set any per-cache values to zero
			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				m_VertexCount[ i ] = 0;
				m_IndexCount[ i ] = 0;
				m_MaterialID[ i ] = 0;
			}
		}

		ZED_UINT32 GLVertexCache::Initialise( )
		{
			// Generate and bind the vertex array ID and vertex buffer
			zglGenVertexArrays( 1, &m_VAID );
			zglGenBuffers( m_CacheLines, m_pVBO );

			zglBindVertexArray( m_VAID );
			
			// Initialise the amount of attributes available
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				zglEnableVertexAttribArray( i );
			}

			GLenum Error = 0;
			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				zglBindBuffer( GL_ARRAY_BUFFER, m_pVBO[ i ] );
				zglBufferData( GL_ARRAY_BUFFER, m_MaxVertices*m_Stride,
					ZED_NULL, GL_STREAM_DRAW );
				// Check that the buffer was allocated without any problems
				Error = glGetError( );

				if( Error == GL_OUT_OF_MEMORY )
				{
					// No GPU memory available =(
					zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] "
						"<ERROR> Failed to allocate memory on the graphics "
						"device.\n" );
					zedAssert( ZED_FALSE );
					return ZED_GRAPHICS_ERROR;
				}
			}
			
			return ZED_OK;
		}

		void GLVertexCache::Add( const ZED_MEMSIZE p_VertexCount,
			const void *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices )
		{
			
		}

		ZED_UINT64 GLVertexCache::GetAttributes( )
		{
			return m_VertexAttributes;
		}

		ZED_MEMSIZE GLVertexCache::GetAttributeCount( )
		{
			return m_AttributeCount;
		}

		ZED_MEMSIZE GLVertexCache::GetVertexCapacity( )
		{
			return m_MaxVertices;
		}

		ZED_MEMSIZE GLVertexCache::GetIndexCapacity( )
		{
			return m_MaxIndices;
		}

		ZED_MEMSIZE GLVertexCache::GetLineCount( )
		{
			return m_CacheLines;
		}

		ZED_MEMSIZE GLVertexCache::AttribToSize( ZED_BYTE p_Attrib )
		{
			// Should be TTSS, where TT is type and SS is size
			// Get the first two bytes and multiply by the last two bytes
			ZED_BYTE Type = p_Attrib >> 2;
			// Mask the size bits
			// 0000 0011
			ZED_MEMSIZE Size = ( 0x03 & p_Attrib )+1;

			switch( Type )
			{
				case 0:
				{
					Size *= sizeof( ZED_INT32 );
					break;
				}

				case 1:
				{
					Size *= sizeof( ZED_FLOAT32 );
					break;
				}

				case 2:
				{
					Size *= sizeof( ZED_FLOAT64 );
					break;
				}
				case 3:
				{
					// Assuming square matrices
					Size *= Size;
					Size *= sizeof( ZED_FLOAT32 );
					break;
				}
			}

			return Size;
		}
	}
}

