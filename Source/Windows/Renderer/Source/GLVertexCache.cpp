#include <OGL/GLVertexCache.hpp>
#include <OGL/GLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLVertexCache::GLVertexCache( )
		{
			// Null and set variables to sane values
			m_pVertexCount = ZED_NULL;
			m_pIndexCount = ZED_NULL;
			m_pMaterialID = ZED_NULL;
			m_pVertexBuffer = ZED_NULL;
			m_pIndexBuffer = ZED_NULL;

			m_MaxVertices = GLVC_MAXVERTS;
			m_MaxIndices = GLVC_MAXINDICES;
			m_AttributeCount = GLVC_ATTRIBCOUNT;
			m_CacheLines = GLVC_CACHELINES;
			m_VertexAttributes = GLVC_ATTRIBUTES;

			// Get the stride from the attributes passed in
			m_Stride = 0;

			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				// Extract a nybble from the attributes
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				m_Stride += AttribToSize( Attrib );
			}
		}

		GLVertexCache::GLVertexCache( const ZED_MEMSIZE p_VertexCount,
			const ZED_MEMSIZE p_IndexCount, const ZED_MEMSIZE p_AttribCount,
			const ZED_UINT64 p_VertexAttributes,
			const ZED_MEMSIZE p_CacheLines )
		{
			// Set the pointers to NULL/nullptr and determine whether to use
			// the default values or the ones passed in
			m_pVertexCount = ZED_NULL;
			m_pIndexCount = ZED_NULL;
			m_pMaterialID = ZED_NULL;
			m_pVertexBuffer = ZED_NULL;
			m_pIndexBuffer = ZED_NULL;

			m_MaxVertices = p_VertexCount ? p_VertexCount : GLVC_MAXVERTS;
			m_MaxIndices = p_IndexCount ? p_IndexCount : GLVC_MAXINDICES;
			m_AttributeCount = p_AttribCount ? p_AttribCount : GLVC_ATTRIBCOUNT;
			m_CacheLines = p_CacheLines ? p_CacheLines : GLVC_CACHELINES;
			m_VertexAttributes = p_VertexAttributes ? p_VertexAttributes :
				GLVC_ATTRIBUTES;

			// Get the stride from the attributes passed in
			m_Stride = 0;

			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				// Extract a nybble from the attribute
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				m_Stride += AttribToSize( Attrib );
			}
		}

		GLVertexCache::~GLVertexCache( )
		{
			// Flush all buffers and delete them
			this->Flush( );
			zglBindVertexArray( 0 );

			if( m_pVertexBuffer != ZED_NULL )
			{
				zglDeleteBuffers( m_CacheLines, m_pVertexBuffer );
				delete [ ] m_pVertexBuffer;
				m_pVertexBuffer = ZED_NULL;
			}

			if( m_pIndexBuffer != ZED_NULL )
			{
				zglDeleteBuffers( m_CacheLines, m_pIndexBuffer );
				delete [ ] m_pIndexBuffer;
				m_pIndexBuffer = ZED_NULL;
			}

			if( m_pVertexCount != ZED_NULL )
			{
				delete [ ] m_pVertexCount;
				m_pVertexCount = ZED_NULL;
			}

			if( m_pIndexCount != ZED_NULL )
			{
				delete [ ] m_pIndexCount;
				m_pIndexCount = ZED_NULL;
			}

			if( m_pMaterialID != ZED_NULL )
			{
				delete [ ] m_pMaterialID;
				m_pMaterialID = ZED_NULL;
			}
		}

		ZED_UINT32 GLVertexCache::Initialise( )
		{
			// Initialise the pointers
			m_pVertexBuffer = new GLuint[ m_CacheLines ];
			m_pIndexBuffer = new GLuint[ m_CacheLines ];
			m_pVertexCount = new ZED_MEMSIZE[ m_CacheLines ];
			m_pIndexCount = new ZED_MEMSIZE[ m_CacheLines ];
			m_pMaterialID = new ZED_UINT32[ m_CacheLines ];

			// Generate and bind the vertex array ID and vertex buffer
			zglGenVertexArrays( 1, &m_VertexAttributeID );
			zglBindVertexArray( m_VertexAttributeID );

			zglGenBuffers( m_CacheLines, m_pVertexBuffer );
			zglGenBuffers( m_CacheLines, m_pIndexBuffer );

			GLenum Error = 0;

			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				m_pVertexCount[ i ] = 0;
				m_pIndexCount[ i ] = 0;
				m_pMaterialID[ i ] = 0;

				zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ i ] );
				zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer[ i ] );

				// Allocate memory for a streaming buffer
				zglBufferData( GL_ARRAY_BUFFER, m_MaxVertices*m_Stride,
					ZED_NULL, GL_STATIC_DRAW );

				// Check for errors
				Error = zglGetError( );

				if( Error == GL_OUT_OF_MEMORY )
				{
					zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] "
						"<ERROR> Failed to allocate memory for vertex cache.\n"
						);
					zedAssert( ZED_FALSE );
					return ZED_GRAPHICS_ERROR;
				}

				zglBufferData( GL_ELEMENT_ARRAY_BUFFER,
					m_MaxIndices*sizeof( ZED_UINT16 ),
					ZED_NULL, GL_STATIC_DRAW );

				// Check for any errors
				Error = zglGetError( );
				if( Error == GL_OUT_OF_MEMORY )
				{
					zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] "
						"<ERROR> Failed to allocate memory for index cache.\n"
						);
					zedAssert( ZED_FALSE );
					return ZED_GRAPHICS_ERROR;
				}
			}

			// Just to be kind...
			zglBindVertexArray( 0 );

			return ZED_OK;
		}

		ZED_UINT32 GLVertexCache::Add( const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT32 p_MaterialID )
		{
			// Find a cache line with the same material ID.
			// If there isn't a cache line using the same one, render the
			// fullest cache line and use the flushed one
			ZED_UINT32 Material = 0;
			ZED_MEMSIZE CacheLine = 0;
			ZED_BOOL MaterialMatch = ZED_FALSE;

			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				if( m_pMaterialID[ i ] == p_MaterialID )
				{
					MaterialMatch = ZED_TRUE;
					Material = p_MaterialID;
					CacheLine = i;
					break;
				}
			}

			// The material IDs do not match, use the fullest line after
			// flushing it
			if( MaterialMatch != ZED_TRUE )
			{
				CacheLine = FlushFullestLine( );
			}

			// See if the vertices and indices will fit into this cache
			if( p_VertexCount > m_MaxVertices )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Failed to add vertices.\n"
					"\tTried to add %d vertices.  Capacity: %d\n",
					p_VertexCount, m_MaxVertices );
				zedAssert( ZED_FALSE );
				return ZED_FAIL;
			}

			if( p_IndexCount > m_MaxIndices )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERRPR> "
					"Failed to add indices.\n"
					"\tTried to add %d indices.  Capacity: %d\n",
					p_IndexCount, m_MaxIndices );
				zedAssert( ZED_FALSE );
				return ZED_FAIL;
			}

			// Okay, the vertices and indices will fit into the cache.
			// If there are too many vertices and indices already present, then
			// the cache will need to be flushed
			if( p_VertexCount >
				( m_MaxVertices - m_pVertexCount[ CacheLine ] ) )
			{
				FlushLine( CacheLine );
			}

			if( p_IndexCount >
				( m_MaxIndices - m_pIndexCount[ CacheLine ] ) )
			{
				FlushLine( CacheLine );
			}

			zglBindVertexArray( m_VertexAttributeID );

			// Fill the vertex buffer
			zglBufferSubData( GL_ARRAY_BUFFER,
				m_pVertexCount[ CacheLine ]*m_Stride,
				p_VertexCount*m_Stride, p_pVertices );

			GLenum Error = zglGetError( );
			if( Error == GL_OUT_OF_MEMORY )
			{
				// Unbind the buffers and complain
				zglBindBuffer( GL_ARRAY_BUFFER, 0 );
				zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Not enough memory available on graphics device.\n" );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			// Set the vertex attributes.
			// The accumulated dimension is used fot the offset between values
			// in the vertex attributes
			ZED_MEMSIZE AccDimension = 0;

			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				// Extract the type and dimension from the attributes
				ZED_MEMSIZE Dimension = 0;
				GLenum Type = GL_INVALID_ENUM;
				ZED_MEMSIZE TypeSize = 0;
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );

				// Get the dimensions by extracting the nth nybble from the
				// attribute ID
				Dimension = 0x03 & ( Attrib )+1;

				switch( ( 0x0C & ( Attrib ) ) >> 2 )
				{
					case 0:
					{
						Type = GL_INT;
						TypeSize = sizeof( ZED_UINT32 );
						break;
					}
					case 1:
					{
						Type = GL_FLOAT;
						TypeSize = sizeof( ZED_FLOAT32 );
						break;
					}
					case 2:
					{
						Type = GL_DOUBLE;
						TypeSize = sizeof( ZED_FLOAT64 );
						break;
					}
					case 3:
					{
						Type = GL_FLOAT;
						TypeSize = sizeof( ZED_FLOAT32 );
						// Assuming square matrices
						Dimension *= Dimension;
						break;
					}
				}

				zglVertexAttribPointer( i, Dimension, Type, GL_FALSE,
					m_Stride,
					ZGL_BUFFER_OFFSET( TypeSize*AccDimension ) );

				AccDimension += Dimension;
			}

			// Initialise the attributes
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				zglEnableVertexAttribArray( i );
			}

			// Before filling the index buffer, the indices need to be offset
			// by the maximum vertex
			ZED_UINT16 *pIndices = new ZED_UINT16[ p_IndexCount ];
			for( ZED_MEMSIZE i = 0; i < p_IndexCount; i++ )
			{
				// Add the index
				pIndices[ i ] = p_pIndices[ i ] + m_pVertexCount[ CacheLine ];
			}

			// Fill the index buffer
			zglBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexCount[ CacheLine ]*sizeof( ZED_UINT16 ),
				p_IndexCount*sizeof( ZED_UINT16 ), pIndices );

			delete [ ] pIndices;
			pIndices = ZED_NULL;

			Error = zglGetError( );

			// This shouldn't occur as the index buffer was already allocated
			if( Error == GL_OUT_OF_MEMORY )
			{
				// Unbind the buffers and complain
				zglBindBuffer( GL_ARRAY_BUFFER, 0 );
				zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Not enough memory available on graphics device." );
				zedAssert( ZED_FALSE );

				return ZED_GRAPHICS_ERROR;
			}

			// Increment the cache line's vertex and index count
			m_pVertexCount[ CacheLine ] += p_VertexCount;
			m_pIndexCount[ CacheLine ] += p_IndexCount;

			// Unbind the array
			zglBindVertexArray( 0 );

			return ZED_OK;
		}

		void GLVertexCache::FlushLine( const ZED_MEMSIZE p_Index )
		{
			// Nothing to render?
			if( ( m_pVertexCount == ZED_NULL ) ||
				( m_pIndexCount == ZED_NULL ) )
			{
				return;
			}

			// Anything at all to render?
			if( ( m_pVertexCount[ p_Index ] == 0 ) ||
				( m_pIndexCount[ p_Index ] == 0 ) )
			{
				return;
			}

			// Bind the buffer and draw using the material
			zglBindVertexArray( m_VertexAttributeID );

			zglDrawElements( GL_TRIANGLES, m_pIndexCount[ p_Index ],
				GL_UNSIGNED_SHORT, ( ZED_BYTE * )NULL + 0 );

			// Done with the array
			zglBindVertexArray( 0 );

			// Reset the size of the vertices and indices
			m_pVertexCount[ p_Index ] = 0;
			m_pIndexCount[ p_Index ] = 0;
		}

		void GLVertexCache::Clear( )
		{
			zglDeleteBuffers( m_CacheLines, m_pVertexBuffer );
			zglDeleteBuffers( m_CacheLines, m_pIndexBuffer );
		}

		ZED_MEMSIZE GLVertexCache::FlushFullestLine( )
		{
			ZED_MEMSIZE VertexCount = 0;
			ZED_MEMSIZE IndexCount = 0;
			ZED_MEMSIZE FullestLine = 0;

			// Slow!
			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				if( m_pVertexCount[ i ] > VertexCount )
				{
					VertexCount = m_pVertexCount[ i ];
					FullestLine = i;
				}
				if( m_pIndexCount[ i ] > IndexCount )
				{
					IndexCount = m_pIndexCount[ i ];
					FullestLine = i;
				}
			}

			// Render the cache with the most indices or vertices
			if( VertexCount > IndexCount )
			{
				FullestLine = VertexCount;
			}
			else
			{
				FullestLine = IndexCount;
			}

			this->FlushLine( FullestLine );

			return FullestLine;
		}

		void GLVertexCache::Flush( )
		{
			// Render all cache lines
			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				this->FlushLine( i );
			}
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
			// Format should be TTSS.  Where TT is Type and SS is Size
			// Get the first two bytes and multipy by the last two bytes.
			ZED_BYTE Type = p_Attrib >> 2;
			// Mask the size bits
			// 0000 0011
			ZED_MEMSIZE Size = ( 0x03 & p_Attrib )+1;

			switch( Type )
			{
				case 0:
				{
					Size *= sizeof( ZED_SINT32 );
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
					// Square matrices only!
					Size *= Size;
					Size *= sizeof( ZED_FLOAT32 );
					break;
				}
			}

			return Size;
		}
	}
}
