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
			// Null any pointers and initialise all variables to sane values
			m_VertexAttributeID = 0;

			// Give default values if not present
			m_MaxVertices = 1000;
			m_MaxIndices = 1000;
			m_AttributeCount = 0;//1;
			m_VertexAttributes = 0;// 0x06; // 01 == ZED_FLOAT32 | 10 == 3D
			m_CacheLines = 10;

			m_pIndexCount = ZED_NULL;

			m_pVertexBuffer = ZED_NULL;//new GLuint[ m_CacheLines ];
			m_pIndexBuffer = ZED_NULL;//new GLuint[ m_CacheLines ];
			m_pVertexCount = ZED_NULL;//new ZED_MEMSIZE[ m_CacheLines ];
			m_pIndexCount = ZED_NULL;//new ZED_MEMSIZE[ m_CacheLines ];
			m_pMaterialID = ZED_NULL;//new ZED_UINT32[ m_CacheLines ];

			// Get the stride from the Attribs passed in
			m_Stride = 0;
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{	
				// As an attribute is a four-bit value, extract a half byte
				// from the attributes pushed in
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				
				m_Stride += AttribToSize( Attrib );

#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::GLVertexCache] "
					"<INFO> Attribute: %d\n", Attrib );
#endif
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
			m_AttributeCount = p_AttributeCount ? p_AttributeCount : 0;//1;
			m_VertexAttributes = p_VertexAttributes ?
				p_VertexAttributes : 0;//0x06; // 01 == ZED_FLOAT32 | 10 == 3D
			m_CacheLines = p_CacheLines ? p_CacheLines : 10;

			m_pIndexCount = ZED_NULL;

			m_pVertexBuffer = ZED_NULL;//new GLuint[ m_CacheLines ];
			m_pIndexBuffer = ZED_NULL;//new GLuint[ m_CacheLines ];
			m_pVertexCount = ZED_NULL;//new ZED_MEMSIZE[ m_CacheLines ];
			m_pIndexCount = ZED_NULL;//new ZED_MEMSIZE[ m_CacheLines ];
			m_pMaterialID = ZED_NULL;//new ZED_UINT32[ m_CacheLines ];

			// Get the stride from the Attribs passed in
			m_Stride = 0;
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{	
				// As an attribute is a four-bit value, extract a half byte
				// from the attributes pushed in
				ZED_BYTE Attrib = 0x0F & ( m_VertexAttributes >> ( i*4 ) );
				
				m_Stride += AttribToSize( Attrib );

#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::GLVertexCache] "
					"<INFO> Attribute: %d\n", Attrib );
#endif
			}
		}

		GLVertexCache::~GLVertexCache( )
		{
			// Flush all buffers and delete them
			this->Flush( );

			zglBindVertexArray( 0 );

			if( m_pVertexBuffer != ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GLVetexCache::~GLVertexCache] "
					"<INFO> Deleting m_pVertexBuffer: 0x%016X\n",
					m_pVertexBuffer );
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
				zedTrace( "[ZED::Renderer::GLVertexCache::~GLVertexCache] "
					"<INFO> Deleting m_pIndexCount: 0x%08X\n", m_pIndexCount );
				delete [ ] m_pIndexCount;
				m_pIndexCount = ZED_NULL;
			}

			if( m_pVertexCount != ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::~GLVertexCache] "
					"<INFO> Deleting m_pVertexCount: 0x%08X\n",
					m_pVertexCount );
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
			zglGenBuffers( m_CacheLines, m_pVertexBuffer );
			zglGenBuffers( m_CacheLines, m_pIndexBuffer );

			zglBindVertexArray( m_VertexAttributeID );	
			
			// Initialise the amount of attributes available
			for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::Initialise] <INFO> "
					"Enabling attribute array %d for %d\n", i,
					m_VertexAttributeID );
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

				// A loop here would be ideal for setting the attribute
				// pointers
				zglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE,
					8*sizeof( float ), ( GLubyte* )NULL+( 0 ) );
				zglVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE,
					8*sizeof( float ),
					( GLubyte* )NULL+( sizeof( float )*3 ) );
				zglVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE,
					8*sizeof( float ),
					( GLubyte* )NULL+( sizeof( float )*6 ) );
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
			ZED_BOOL MaterialMatch = ZED_FALSE;

			for( ZED_MEMSIZE i = 0; i < m_CacheLines; i++ )
			{
#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <INFO> "
					"m_pMaterialID[ %d ] = %d | m_MaterialID = %d\n",
					i, m_pMaterialID[ i ], p_MaterialID );
#endif
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
#ifdef ZED_BUILD_DEBUG
				zedTrace( "[ZED::Renderer::GLVertexCache::Add] <INFO> "
					"Material ID not found\n" );
#endif
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
			zedTrace( "m_pVertexBuffer: %d | m_pIndexBuffer: %d\n",
				m_pVertexBuffer[ CacheLine ], m_pIndexBuffer[ CacheLine ] );
			// Add a check for buffer here?
			zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ CacheLine ] );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexBuffer[ CacheLine ] );

			zedTrace( "m_pVertexCount[ %d ] = %d\n", CacheLine,
				m_pVertexCount[ CacheLine ] );
			zedTrace( "m_pIndexCount[ %d ] = %d\n", CacheLine,
				m_pIndexCount[ CacheLine ] );
			zedTrace( "Vertex Count: %d\n", p_VertexCount );
			zedTrace( "Index Count: %d\n", p_IndexCount );
			zedTrace( "p_VertexCount*m_Stride = %d\n", p_VertexCount*m_Stride );
			
			for( ZED_MEMSIZE Itr = 0; Itr < p_VertexCount*m_Stride;  )
			{
				zedTrace( "Vertex %d = %f\n", Itr, ( ZED_FLOAT32 )p_pVertices[ Itr ] );
				Itr += 4;
				zedTrace( "Vertex: %d = %f\n", Itr, ( ZED_FLOAT32 )p_pVertices[ Itr ] );
				Itr += 4;
				zedTrace( "Vertex: %d = %f\n", Itr, ( ZED_FLOAT32 )p_pVertices[ Itr ] );
				Itr += 4;
				Itr += 20;
			}

			for( ZED_MEMSIZE Itr = 0 ; Itr < p_IndexCount; Itr++ )
			{
				zedTrace( "Index %d = %d\n", Itr, p_pIndices[ Itr ] );
			}

			zglBufferSubData( GL_ARRAY_BUFFER,
				m_pVertexCount[ CacheLine ]*m_Stride,
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

			zedTrace( "Index count: %d\n", p_IndexCount );

			// Fill the index buffer
			zglBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexCount[ CacheLine ]*sizeof( ZED_UINT16 ),
				24/*p_IndexCount*/*sizeof( ZED_UINT16 ),
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
			m_pIndexCount[ CacheLine ] += 24;//p_IndexCount;

			// Unbind the buffers
			zglBindBuffer( GL_ARRAY_BUFFER, 0 );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

			return ZED_OK;
		}

		void GLVertexCache::FlushLine( const ZED_MEMSIZE p_Index )
		{
			if( ( m_pVertexCount == ZED_NULL ) ||
				( m_pIndexCount == ZED_NULL ) )
			{
				return;
			}

#ifdef ZED_BUILD_DEBUG
			// What are the values of the vertex and index count
			zedTrace( "[ZED::Renderer::GLVertexCache::FlushLine] <INFO> "
				"Vertices being rendered: %d | Indices being rendered: %d\n",
				m_pVertexCount[ p_Index ], m_pIndexCount[ p_Index ] );
#endif
			
			// Anything to render?
			if( ( m_pVertexCount[ p_Index ] == 0 ) ||
				( m_pIndexCount[ p_Index ] == 0 ) )
			{
				zedTrace( "[ZED::Renderer::GLVertexCache::FlushLine] <INFO> "
					"No vertices or indices to render in cache line %d\n",
					p_Index );
				return;
			}

			zedTrace( "[ZED::Renderer::GLVertexCache::FlushLine] <INFO> "
				"Binding and drawing... " );

			zedTrace( "Binding %d VAID | %d VB | %d IB\n", m_VertexAttributeID,
				m_pVertexBuffer[ p_Index ], m_pIndexBuffer[ p_Index ] );

			// Bind the buffer and draw using the material
			zglBindVertexArray( m_VertexAttributeID );
			zglBindBuffer( GL_ARRAY_BUFFER, m_pVertexBuffer[ p_Index ] );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				m_pIndexBuffer[ p_Index ] );
			
			zglDrawElements( GL_TRIANGLES, m_pIndexCount[ p_Index ],
				GL_UNSIGNED_SHORT, ( GLubyte* )NULL + 0 );

			zedTrace( "Done\n" );

			// Unbind the buffers
			zglBindBuffer( GL_ARRAY_BUFFER, 0 );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
			
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
			zedTrace( "Size: %d\n", Size );
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

