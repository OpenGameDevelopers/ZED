#ifndef __ZED_RENDERER_RENDERERFACTORY_HPP__
#define __ZED_RENDERER_RENDERERFACTORY_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/OGL/GLRenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Renderer;

		// The renderer string is formatted as follows:
		// <Backend> <Version>
		// For example:
		// Direct3D 8.0
		// Direct3D 8.1
		// Direct3D 9.0c
		// Direct3D 12
		// OpenGL 1.1
		// OpenGL 3.0
		// OpenGL 4.5
		// Vulkan 1.0
		ZED_UINT32 CreateRenderer( const ZED_CHAR8 *p_pRendererString,
			Renderer **p_ppRenderer );

		ZED_UINT32 GetHighestSupportedOpenGLVersion(
			ZED_GLVERSION &p_GLVersion );
	}
}

#endif // __ZED_RENDERER_RENDERERFACTORY_HPP__

