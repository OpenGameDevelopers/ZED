#include <GLShader.hpp>
#include <GLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShader::GLShader( )
		{
		}

		GLShader::GLShader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
			const ZED_BOOL p_Geometry )
		{
			if( p_Vertex == ZED_TRUE )
			{
				//m_VertexID = glCreateShader( GL_VERTEX_SHADER );
			}
		}
	}
}
