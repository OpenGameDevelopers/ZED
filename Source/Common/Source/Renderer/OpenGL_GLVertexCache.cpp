#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLVertexCache.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <cstring>

#define BUFFER_OFFSET( offset ) ( ( char * )NULL + ( offset ) )

namespace ZED
{
	namespace Renderer
	{
		GLVertexCache::GLVertexCache( )
		{
			// Null any pointers and initialise all variables to sane values
			m_VertexAttributeID = 0;

			// Give default values if not present
			m_MaxVertices = 1000;
			m_MaxIndices = 1000;
			m_AttributeCount = 0;
			m_VertexAttributes = 0;
			m_CacheLines = 10;

			m_pIndexCount = ZED_NULL;
			m_pVertexBuffer = ZED_NULL;
			m_pIndexBuffer = ZED_NULL;
			m_pVertexCount = ZED_NULL;
			m_pIndexCount = ZED_NULL;
			m_pMaterialID = ZED_NULL;

			// Get the stride from the Attribs passed in
			m_Stride = 0;

			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{	
				// As an attribute is a four-bit value, extract a half byte
				// from the attributes pushed in
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				
				m_Stride += AttribToSize( Attrib );
			}
		}

		GLVertexCache::GLVertexCache( const ZED_MEMSIZE p_VertexCount,
			const ZED_MEMSIZE p_IndexCount, const ZED_MEMSIZE p_AttributeCount,
			const ZED_UINT64 p_VertexAttributes,
			const ZED_MEMSIZE p_CacheLines )
		{
			// Null any pointers and initialise all variables to sane values
			m_VertexAttributeID = 0;

			// Give default values if not present
			m_MaxVertices = p_VertexCount ? p_VertexCount : 1000;
			m_MaxIndices = p_IndexCount ? p_IndexCount : 1000;
			m_AttributeCount = p_AttributeCount ? p_AttributeCount : 0;
			m_VertexAttributes = p_VertexAttributes ?
				p_VertexAttributes : 0;
			m_CacheLines = p_CacheLines ? p_CacheLines : 10;

			m_pIndexCount = ZED_NULL;
			m_pVertexBuffer = ZED_NULL;
			m_pIndexBuffer = ZED_NULL;
			m_pVertexCount = ZED_NULL;
			m_pIndexCount = ZED_NULL;
			m_pMaterialID = ZED_NULL;

			// Get the stride from the Attribs passed in
			m_Stride = 0;

			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{	
				// As an attribute is a four-bit value, extract a nybble
				// from the attributes pushed in
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				
				m_Stride += AttribToSize( Attrib );
			}
		}

		GLVertexCache::~GLVertexCache( )
		{
			// Flush all buffers and delete them
			this->Flush( );

			zglDeleteVertexArrays( 1, &m_VertexAttributeID );

			if( m_pVertexBuffer != ZED_NULL )
			{
				// Free the buffers and delete the handles
				zglDeleteBuffers( m_CacheLines, m_pVertexBuffer );
				delete [ ] m_pVertexBuffer;
			}
			if( m_pIndexBuffer != ZED_NULL )
			{
				// Free the buffers and delete the handles
				zglDeleteBuffers( m_CacheLines, m_pIndexBuffer );
				delete [ ] m_pIndexBuffer;
			}
			if( m_pIndexCount != ZED_NULL )
			{
				delete [ ] m_pIndexCount;
				m_pIndexCount = ZED_NULL;
			}

			if( m_pVertexCount != ZED_NULL )
			{
				delete [ ] m_pVertexCount;
				m_pVertexCount = ZED_NULL;
			}

			if( m_pMaterialID != ZED_NULL )
			{
				delete [ ] m_pMaterialID;
			}
		}

		ZED_UINT32 GLVertexCache::Initialise( )
		{
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
				// Set the index and vertex counts to zero as well as the
				// material ID for each cache line
				m_pVertexCount[ i ] = 0;
				m_pIndexCount[ i ] = 0;
				m_pMaterialID[ i ] = 0;

				zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ i ] );

				// Allocate memory for a streaming buffer
				zglBufferData( GL_ARRAY_BUFFER, m_MaxVertices*m_Stride,
					ZED_NULL, GL_STREAM_DRAW );

				zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer[ i ] );
				zglBufferData( GL_ELEMENT_ARRAY_BUFFER,
					m_MaxIndices*sizeof( ZED_UINT16 ), ZED_NULL,
					GL_STREAM_DRAW );

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

			zglBindVertexArray( 0 );

			return ZED_OK;
		}

		ZED_UINT32 GLVertexCache::Add( const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT32 p_MaterialID,
			const ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			// Find a cache line with the same material ID.
			// If there isn't a cache line using the same material ID, render
			// the fullest cache line and use a new line.
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

			// The material IDs don't match
			if( MaterialMatch != ZED_TRUE )
			{
				CacheLine = FlushFullestLine( );
			}

			// See if the vertices and indices will fit into this cache
			if( p_VertexCount > m_MaxVertices )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Failed to allocate vertices.\n" );
				zedTrace( "\tTried to allocate %d vertices.  Capacity: %d\n",
					p_VertexCount, m_MaxVertices );
				zedAssert( ZED_FALSE );
				return ZED_FAIL;
			}

			if( p_IndexCount > m_MaxIndices )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Failed to allocate indices.\n" );
				zedAssert( ZED_FALSE );
				return ZED_FAIL;
			}

			// If the vertices and indices will fit, but there are currently
			// too many already present, then flush the cache to put them into
			if( p_VertexCount >
				( m_MaxVertices - m_pVertexCount[ CacheLine ] ) )
			{
				FlushLine( CacheLine );
			}

			if( p_IndexCount > ( m_MaxIndices - m_pIndexCount[ CacheLine ] ) )
			{
				FlushLine( CacheLine );
			}

			// Okay, now that the cache is ready, add the vertices
			zglBindVertexArray( m_VertexAttributeID );
			
			// Fill the vertex buffer
			// These should both be timed to see which is quicker...
			/*zglBufferSubData( GL_ARRAY_BUFFER,
				m_pVertexCount[ CacheLine ]*m_Stride,
				p_VertexCount*m_Stride, p_pVertices );*/

			ZED_BYTE *pVertices = ( ZED_BYTE * )( zglMapBufferRange(
				GL_ARRAY_BUFFER, m_pVertexCount[ CacheLine ]*m_Stride,
				p_VertexCount*m_Stride,
				GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT ) );

			memcpy( pVertices, p_pVertices, p_VertexCount * m_Stride );

			zglUnmapBuffer( GL_ARRAY_BUFFER );


			// The accumulated dimension is used for the offset between values
			// in the vertex attributes
			ZED_MEMSIZE AccDimension = 0;
			// Set up the vertex format being used
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; ++i )
			{
				// Extract the type and dimension from the attributes
				ZED_MEMSIZE Dimension = 0;
				GLenum Type = GL_INVALID_ENUM;
				ZED_MEMSIZE TypeSize = 0;
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				
				// Get the dimensions by extracting the nth nybble from the 
				// attribute ID
				Dimension = 0x03 & ( Attrib )+1;

				// Get the type using a similar technique
				switch( ( 0x0C & ( Attrib ) ) >> 2 )
				{
					case 0:
					{
						Type = GL_INT;
						TypeSize = sizeof( ZED_SINT32 );
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

				zglVertexAttribPointer( i, Dimension, Type, GL_FALSE, m_Stride,
					BUFFER_OFFSET( TypeSize*AccDimension ) );

				AccDimension += Dimension;
			}
				
			// Initialise the amount of attributes available
			for( ZED_MEMSIZE j = 0; j < m_AttributeCount; j++ )
			{
				zglEnableVertexAttribArray( j );
			}

			// There shouldn't be a problem, but just in case...
			GLenum Error = glGetError( );

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
				p_IndexCount*sizeof( ZED_UINT16 ),
				pIndices );

			delete [ ] pIndices;

			Error = glGetError( );
			// Again, this shouldn't happen if the cache was allocated already
			if( Error == GL_OUT_OF_MEMORY )
			{
				// Unbind the buffers and complain
				zglBindBuffer( GL_ARRAY_BUFFER, 0 );
				zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Not enough memory avilable on the graphics device.\n" );
				zedAssert( ZED_FALSE );
				return ZED_GRAPHICS_ERROR;
			}

			// Everything went fine, increment the vertex and index count
			m_pVertexCount[ CacheLine ] += p_VertexCount;
			m_pIndexCount[ CacheLine ] += p_IndexCount;

			// Unbind the buffer
			zglBindVertexArray( 0 );

			m_PrimitiveType = p_PrimitiveType;

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
			
			// Anything to render?
			if( ( m_pVertexCount[ p_Index ] == 0 ) ||
				( m_pIndexCount[ p_Index ] == 0 ) )
			{
				return;
			}

			// Bind the buffer and draw using the material
			zglBindVertexArray( m_VertexAttributeID );

			zglDrawElements( m_PrimitiveType, m_pIndexCount[ p_Index ],
				GL_UNSIGNED_SHORT, ( GLubyte* )NULL + 0 );

			// Unbind the buffer
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

			// Slowest part ;P
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

			// If the indices are the fullest, they get flushed, otherwise the
			// line with the most vertices is flushed
			this->FlushLine( FullestLine );

			return m_CacheLines;
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
#endif // ZED_PLATFORM_SUPPORTS_OPENGL

