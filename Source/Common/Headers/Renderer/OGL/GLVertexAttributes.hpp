#ifndef __ZED_RENDERER_GLVERTEXATTRIBUTES_HPP__
#define __ZED_RENDERER_GLVERTEXATTRIBUTES_HPP__

#include <Renderer/VertexAttributes.hpp>
#include <GL/gl.h>

namespace ZED
{
	namespace Renderer
	{
		GLenum ConvertVertexAttributeToGLenum(
			const VERTEXATTRIBUTE &p_VertexAttribute );
	}
}

#endif // __ZED_RENDERER_GLVERTEXATTRIBUTES_HPP__

