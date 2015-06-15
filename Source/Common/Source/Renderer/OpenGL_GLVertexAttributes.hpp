#include <Renderer/OGL/GLVertexAttributes.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLenum ConvertVertexAttributeToGLenum(
			const VERTEXATTRIBUTE &p_VertexAttribute )
		{
			switch( p_VertexAttribute.Type )
			{
				case VERTEXATTRIBUTE_TYPE_FLOAT1:
				case VERTEXATTRIBUTE_TYPE_FLOAT2:
				case VERTEXATTRIBUTE_TYPE_FLOAT3:
				case VERTEXATTRIBUTE_TYPE_FLOAT4:
				case VERTEXATTRIBUTE_TYPE_MAT2X2:
				case VERTEXATTRIBUTE_TYPE_MAT3X3:
				case VERTEXATTRIBUTE_TYPE_MAT4X4:
				{
					return GL_FLOAT;
				}
				case VERTEXATTRIBUTE_TYPE_UNKNOWN:
				defualt:
				{
					break;
				}
			}

			return GL_ENUM_INVALID;
		}
	}
}

