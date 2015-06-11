#include <Renderer/VertexAttributes.hpp>
#include <System/Debugger.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		VertexAttributes::VertexAttributes(
			ZED_MEMSIZE p_MaximumVertexAttributeCount ) :
			m_VertexStride( 0 ),
			m_MaximumVertexAttributeCount( p_MaximumVertexAttributeCount )
		{
		}

		VertexAttributes::~VertexAttributes( )
		{
		}

		ZED_UINT32 VertexAttributes::AddVertexAttribute(
			const VERTEXATTRIBUTE_TYPE p_Type,
			const VERTEXATTRIBUTE_INTENT p_Intent )
		{
			if( m_VertexAttributes.size( ) > m_MaximumVertexAttributeCount )
			{
				zedTrace( "[ZED::Renderer::VertexAttributes::"
					"AddVErtexAttribute] <ERROR> Unable to allocate another "
					"vertex attribute.  Maximum exceeded\n" );
				return ZED_FAIL;
			}

			VERTEXATTRIBUTE NewAttribute;
			NewAttribute.Type = p_Type;
			NewAttribute.Intent = p_Intent;

			m_VertexStride += ConvertVertexAttributeToSize( NewAttribute );
			m_VertexAttributes.push_back( NewAttribute );

			return ZED_OK;
		}

		ZED_UINT32 VertexAttributes::GetVertexAttributeAt(
			const ZED_MEMSIZE p_Index, VERTEXATTRIBUTE &p_VertexAttributeOut )
		{
			if( p_Index > m_VertexAttributes.size( ) )
			{
				return ZED_FAIL;
			}

			p_VertexAttributeOut = m_VertexAttributes[ p_Index ];

			return ZED_OK;
		}

		ZED_MEMSIZE VertexAttributes::GetVertexAttributeCount( ) const
		{
			return m_VertexAttributes.size( );
		}

		ZED_UINT32 VertexAttributes::GetVertexAttributes(
			VERTEXATTRIBUTE *p_pVertexAttributes ) const
		{
			if( !p_pVertexAttributes )
			{
				return ZED_FAIL;
			}

			for( ZED_MEMSIZE Index = 0; Index < m_VertexAttributes.size( );
				++Index )
			{
				p_pVertexAttributes[ Index ] = m_VertexAttributes[ Index ];
			}

			return ZED_OK;
		}

		ZED_MEMSIZE VertexAttributes::GetVertexStride( ) const
		{
			return m_VertexStride;
		}

		ZED_MEMSIZE ConverteVertexAttributeToSize(
			const VERTEXATTRIBUTE &p_Attribute )
		{
			switch( p_Attribute.Type )
			{
				case VERTEXATTRIBUTE_TYPE_FLOAT1:
				{
					return sizeof( ZED_FLOAT32 );
				}
				case VERTEXATTRIBUTE_TYPE_FLOAT2:
				{
					return sizeof( ZED_FLOAT32 ) * 2;
				}
				case VERTEXATTRIBUTE_TYPE_FLOAT3:
				{
					return sizeof( ZED_FLOAT32 ) * 3;
				}
				case VERTEXATTRIBUTE_TYPE_FLOAT4:
				{
					return sizeof( ZED_FLOAT32 ) * 4;
				}
				case VERTEXATTRIBUTE_TYPE_MAT2X2:
				{
					return sizeof( ZED_FLOAT32 ) * 4;
				}
				case VERTEXATTRIBUTE_TYPE_MAT3X3:
				{
					return sizeof( ZED_FLOAT32 ) * 9;
				}
				case VERTEXATTRIBUTE_TYPE_MAT4X4:
				{
					return sizeof( ZED_FLOAT32 ) * 16;
				}
				case VERTEXATTRIBUTE_TYPE_UNKNOWN:
				default:
				{
					break;
				}
			}

			return 0;
		}

		ZED_MEMSIZE ConvertVertexAttributeToElementCount(
			const VERTEXATTRIBUTE &p_Attribute )
		{
			switch( p_Attribute.Type )
			{
				case VERTEXATTRIBUTE_TYPE_FLOAT1:
				{
					return 1;
				}
				case VERTEXATTRIBUTE_TYPE_FLOAT2:
				{
					return 2;
				}
				case VERTEXATTRIBUTE_TYPE_FLOAT3:
				{
					return 3;
				}
				case VERTEXATTRIBUTE_TYPE_FLOAT4:
				case VERTEXATTRIBUTE_TYPE_MAT2X2:
				{
					return 4;
				}
				case VERTEXATTRIBUTE_TYPE_MAT3X3:
				{
					return 9;
				}
				case VERTEXATTRIBUTE_TYPE_MAT4X4:
				{
					return 16;
				}
				case VERTEXATTRIBUTE_TYPE_UNKNOWN:
				default:
				{
					break;
				}
			}

			return 0;
		}
	}
}

