#include <GLVertexCache.hpp>
#include <GLExtender.hpp>

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

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
			m_VertexAttributeID = 0;

			// Give default values if not present
			m_MaxVertices = p_VertexCount ? p_VertexCount : 1000;
			m_MaxIndices = p_IndexCount ? p_IndexCount : 1000;
			m_AttributeCount = p_AttributeCount ? p_AttributeCount : 1;
			m_VertexAttributes = p_VertexAttributes ?
				p_VertexAttributes : 0x06; // 01 == ZED_FLOAT32 | 10 == 3D
			m_CacheLines = p_CacheLines ? p_CacheLines : 1;

			m_pVertexBuffer = new GLuint[ m_CacheLines ];
			m_pIndexBuffer = new GLuint[ m_CacheLines ];
			m_pVertexCount = new ZED_MEMSIZE[ m_CacheLines ];
			m_pIndexCount = new ZED_MEMSIZE[ m_CacheLines ];
			m_pMaterialID = new ZED_UINT32[ m_CacheLines ];

			// Get the stride from the Attribs passed in
			m_Stride = 0;
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{	
				// As an attribute is a four-bit value, extract a half byte
				// from the attributes pushed in
				ZED_BYTE Attrib = m_VertexAttributes >> ( i*4 );
				
				m_Stride += AttribToSize( Attrib );

#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::GLVertexCache] "
					"<INFO> Attribute: %d\n", Attrib );
#endif
			}
		}

		GLVertexCache::~GLVertexCache( )
		{
			zglBindVertexArray( 0 );

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
			}

			if( m_pVertexCount != ZED_NULL )
			{
				delete [ ] m_pVertexCount;
			}

			if( m_pMaterialID != ZED_NULL )
			{
				delete [ ] m_pMaterialID;
			}
		}

		ZED_UINT32 GLVertexCache::Initialise( )
		{
			// Generate and bind the vertex array ID and vertex buffer
			zglGenVertexArrays( 1, &m_VertexAttributeID );
			zglGenBuffers( m_CacheLines, m_pVertexBuffer );
			zglGenBuffers( m_CacheLines, m_pIndexBuffer );

			zglBindVertexArray( m_VertexAttributeID );
			
			// Initialise the amount of attributes available
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				zglEnableVertexAttribArray( i );
			}

			GLenum Error = 0;
#ifdef ZED_BUILD_DEBUG
			GLint TotalGPUMemory = 0;
			GLint AvailableGPUMemory = 0;
#endif

			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				// Set the index and vertex counts to zero as well as the
				// material ID for each cache line
				m_pVertexCount[ i ] = 0;
				m_pIndexCount[ i ] = 0;
				m_pMaterialID[ i ] = 0;

#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] <INFO> "
					"Binding Vertex Buffer Object: %d\n",
					m_pVertexBuffer[ i ] );
#endif
				zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ i ] );
				zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer[ i ] );
#ifdef ZED_BUILD_DEBUG
				// Show the GPU memory usage [TEMP!]
				glGetIntegerv( GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
					&TotalGPUMemory );
				glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
					&AvailableGPUMemory );
				zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] <INFO> "
					"Total GPU Memory: %dKiB | Used GPU Memory %dKiB | "
					"Available GPU Memory: %dKiB\n", TotalGPUMemory,
					( TotalGPUMemory-AvailableGPUMemory ),
					AvailableGPUMemory );

				zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] <INFO> "
					"Allocating %d bytes\n", m_MaxVertices*m_Stride );
#endif

				// Allocate memory for a streaming buffer
				zglBufferData( GL_ARRAY_BUFFER, m_MaxVertices*m_Stride,
					ZED_NULL, GL_STREAM_DRAW );
				zglBufferData( GL_ELEMENT_ARRAY_BUFFER,
					m_MaxIndices*sizeof( ZED_UINT16 ), ZED_NULL,
					GL_STREAM_DRAW );

#ifdef ZED_BUILD_DEBUG
				glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
					&AvailableGPUMemory );
				zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] <INFO> "
					"Total GPU Memory: %dKiB | Used GPU Memory %dKiB | "
					"Available GPU Memory: %dKiB\n", TotalGPUMemory,
					( TotalGPUMemory-AvailableGPUMemory ),
					AvailableGPUMemory );
#endif

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

		ZED_UINT32 GLVertexCache::Add( const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices, const ZED_UINT32 p_MaterialID )
		{
			// Find a cache line with the same material ID.
			// If there isn't a cache line using the same material ID, render
			// the fullest cache line and use a new line.
			ZED_UINT32 Material = 0;
			ZED_MEMSIZE CacheLine = 0;
			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
				if( m_pMaterialID[ i ] == p_MaterialID )
				{
					Material = p_MaterialID;
					CacheLine = i;
					break;
				}
			}

			// The material IDs don't match
			if( Material == 0 )
			{
				CacheLine = FlushFullestLine( );
			}

			// See if the vertices and indices will fit into this cache
			if( p_VertexCount > m_MaxVertices )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <ERROR> "
					"Failed to allocate vertices.\n" );
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
			zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ CacheLine ] );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexBuffer[ CacheLine ] );

			zglBufferSubData( GL_ARRAY_BUFFER, m_pVertexCount[ CacheLine ],
				p_VertexCount*m_Stride, p_pVertices );

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

			// Fill the index buffer
			zglBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexCount[ CacheLine ], p_IndexCount*m_Stride,
				p_pIndices );

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

			// Unbind the buffers
			zglBindBuffer( GL_ARRAY_BUFFER, 0 );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

			return ZED_OK;
		}

		void GLVertexCache::FlushLine( const ZED_MEMSIZE p_Index )
		{
			// Anything to render?
			if( ( m_pVertexCount[ p_Index ] == 0 ) ||
				( m_pIndexCount[ p_Index ] == 0 ) )
			{
#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::FlushLine] <INFO> "
					"No vertices or indices to render in cache line %d\n",
					p_Index );
#endif
				return;
			}

			// Bind the buffer and draw using the material
			zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ p_Index ] );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexBuffer[ p_Index ] );
			
			zglBindVertexArray( m_VertexAttributeID );
			
			zglDrawElements( GL_TRIANGLES, m_pIndexCount[ p_Index ],
				GL_UNSIGNED_SHORT, ( GLvoid * )ZED_NULL+0 );

			// Unbind the buffers
			zglBindBuffer( GL_ARRAY_BUFFER, 0 );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
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
					Size *= sizeof( ZED_INT32 );
					zedTrace( "Encountered 0\n" );
					break;
				}

				case 1:
				{
					Size *= sizeof( ZED_FLOAT32 );
					zedTrace( "Encountered 1\n" );
					break;
				}

				case 2:
				{
					Size *= sizeof( ZED_FLOAT64 );
					zedTrace( "Encountered 2\n" );
					break;
				}
				case 3:
				{
					// Assuming square matrices
					Size *= Size;
					Size *= sizeof( ZED_FLOAT32 );
					zedTrace( "Encountered 3\n" );
					break;
				}
			}

			zedTrace( "Size: %d | Type %d\n", Size, Type );

			return Size;
		}
	}
}

