#include <Renderer/OGL/GLPrimitive.hpp>
#include <Renderer/OGL/GLVertexAttributes.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <System/Debugger.hpp>

#define BUFFER_OFFSET( Offset ) ( ( unsigned char * ) ZED_NULL + ( Offset ) )

namespace ZED
{
	namespace Renderer
	{
		GLPrimitive::GLPrimitive( ) :
			m_GLPrimitiveType( GL_INVALID_ENUM ),
			m_VertexBufferObject( 0 ),
			m_IndexBufferObject( 0 ),
			m_VertexArrayObject( 0 )
		{
			Primitive::m_PrimitiveCount = 0;
			Primitive::m_MemoryUsed = 0;
			Primitive::m_VertexCount = 0;
			Primitive::m_IndexCount = 0;
			Primitive::m_PrimitiveType = PRIMITIVE_TYPE_UNKNOWN;

			m_DebugVertexBufferObject[
				PRIMITIVE_DEBUG_TYPE_INDEX_WIREFRAME ] = 0U;
			m_DebugIndexBufferObject[
				PRIMITIVE_DEBUG_TYPE_INDEX_WIREFRAME ] = 0U;
			m_DebugVertexArrayObject[
				PRIMITIVE_DEBUG_TYPE_INDEX_WIREFRAME ] = 0U;

			m_DebugVertexBufferObject[
				PRIMITIVE_DEBUG_TYPE_INDEX_NORMALS ] = 0U;
			m_DebugIndexBufferObject[
				PRIMITIVE_DEBUG_TYPE_INDEX_NORMALS ] = 0U;
			m_DebugVertexArrayObject[
				PRIMITIVE_DEBUG_TYPE_INDEX_NORMALS ] = 0U;
		}

		GLPrimitive::~GLPrimitive( )
		{
			this->Destroy( );
		}

		ZED_UINT32 GLPrimitive::Create( const ZED_MEMSIZE p_VertexCount,
			const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
			const ZED_UINT16 *p_pIndices,
			const VertexAttributes &p_VertexAttributes,
			const PRIMITIVE_TYPE p_PrimitiveType )
		{
			if( p_pVertices == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GLPrimitive::Create] <ERROR> "
					"Pointer to vertices was not valid\n" );

				return ZED_FAIL;
			}

			if( p_pIndices == ZED_NULL )
			{
				return ZED_FAIL;
			}

			switch( p_PrimitiveType )
			{
				case PRIMITIVE_TYPE_TRIANGLE_LIST:
				{
					m_PrimitiveCount = p_IndexCount / 3;
					m_GLPrimitiveType = GL_TRIANGLES;

					break;
				}
				case PRIMITIVE_TYPE_TRIANGLE_STRIP:
				{
					m_PrimitiveCount = p_IndexCount - 2;
					m_GLPrimitiveType = GL_TRIANGLE_STRIP;

					break;
				}
				case PRIMITIVE_TYPE_TRIANGLE_FAN:
				{
					m_PrimitiveCount = p_IndexCount - 2;
					m_GLPrimitiveType = GL_TRIANGLE_FAN;

					break;
				}
				case PRIMITIVE_TYPE_LINE_LIST:
				{
					m_PrimitiveCount = p_IndexCount / 2;
					m_GLPrimitiveType = GL_LINES;

					break;
				}
				case PRIMITIVE_TYPE_LINE_STRIP:
				{
					m_PrimitiveCount = p_IndexCount - 1;
					m_GLPrimitiveType = GL_LINE_STRIP;

					break;
				}
				case PRIMITIVE_TYPE_LINE_LOOP:
				{
					m_PrimitiveCount = p_IndexCount;
					m_GLPrimitiveType = GL_LINE_LOOP;

					break;
				}
				case PRIMITIVE_TYPE_UNKNOWN:
				default:
				{
					return ZED_FAIL;
				}
			}

			if( m_PrimitiveCount == 0 )
			{
				return ZED_FAIL;
			}

			zglGenBuffers( 1, &m_VertexBufferObject );
			zglGenBuffers( 1, &m_IndexBufferObject );
			zglGenVertexArrays( 1, &m_VertexArrayObject );
			
			zglBindVertexArray( m_VertexArrayObject );
			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObject );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject );

			ZED_MEMSIZE Offset = 0;
			ZED_SINT32 PositionOffset = -1;
			ZED_MEMSIZE PositionElementCount = 0;
			ZED_SINT32 NormalsOffset = -1;
			ZED_MEMSIZE NormalsElementCount = 0;

			for( ZED_MEMSIZE Index = 0;
				Index < p_VertexAttributes.GetVertexAttributeCount( );
				++Index )
			{
				ZED_MEMSIZE Dimension = 0;
				ZED_MEMSIZE TypeSize = 0;
				GLenum GLType = GL_INVALID_ENUM;
				VERTEXATTRIBUTE VertexAttribute;

				if( p_VertexAttributes.GetVertexAttributeAt( Index,
					VertexAttribute ) != ZED_OK )
				{
					return ZED_FAIL;
				}

				GLType = ConvertVertexAttributeToGLenum( VertexAttribute );
				TypeSize = ConvertVertexAttributeToSize( VertexAttribute );
				Dimension = ConvertVertexAttributeToElementCount(
					VertexAttribute );

				if( VertexAttribute.Intent == VERTEXATTRIBUTE_INTENT_POSITION )
				{
					PositionOffset = Offset;
					PositionElementCount = Dimension;
				}
				
				if( VertexAttribute.Intent == VERTEXATTRIBUTE_INTENT_NORMALS )
				{
					NormalsOffset = Offset;
					NormalsElementCount = Dimension;
				}

				zglVertexAttribPointer( Index, Dimension, GLType, GL_FALSE,
					p_VertexAttributes.GetVertexStride( ),
					BUFFER_OFFSET( Offset ) );

				Offset += TypeSize;

				zglEnableVertexAttribArray( Index );
			}

			zglBufferData( GL_ARRAY_BUFFER,
				p_VertexCount * p_VertexAttributes.GetVertexStride( ),
				p_pVertices, GL_STATIC_DRAW );

			GLenum Error = glGetError( );

			if( Error == GL_OUT_OF_MEMORY )
			{
				this->Destroy( );

				return ZED_FAIL;
			}

			zglBufferData( GL_ELEMENT_ARRAY_BUFFER,
				p_IndexCount * sizeof( ZED_UINT16 ), p_pIndices,
				GL_STATIC_DRAW );

			Error = glGetError( );

			if( Error == GL_OUT_OF_MEMORY )
			{
				this->Destroy( );

				return ZED_FAIL;
			}

			m_VertexCount = p_VertexCount;
			m_IndexCount = p_IndexCount;

			zglBindVertexArray( 0 );
			zglBindBuffer( GL_ARRAY_BUFFER, 0 );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

			// Need to do the normals and wireframe

			return ZED_OK;
		}

		void GLPrimitive::Destroy( )
		{
			if( m_VertexBufferObject )
			{
				zglDeleteBuffers( 1, &m_VertexBufferObject );
				m_VertexBufferObject = 0;
			}
			if( m_IndexBufferObject )
			{
				zglDeleteBuffers( 1, &m_IndexBufferObject );
				m_IndexBufferObject = 0;
			}
			if( m_VertexArrayObject )
			{
				zglDeleteVertexArrays( 1, &m_VertexArrayObject );
				m_VertexArrayObject = 0;
			}
		}

		ZED_UINT32 GLPrimitive::Render( )
		{
			if( m_PrimitiveCount == 0 )
			{
				return ZED_FAIL;
			}

			zglBindVertexArray( m_VertexArrayObject );

			zglBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObject );
			zglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject );

			zglDrawElements( m_GLPrimitiveType, m_IndexCount,
				GL_UNSIGNED_SHORT, BUFFER_OFFSET( 0 ) );

			zglBindVertexArray( 0 );

			return ZED_OK;
		}

		ZED_UINT32 GLPrimitive::RenderNormals( )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLPrimitive::RenderWireframe( )
		{
			return ZED_OK;
		}
	}
}

