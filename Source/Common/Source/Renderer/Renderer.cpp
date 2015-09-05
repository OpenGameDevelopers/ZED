#include <Renderer/Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_RENDERER_BACKEND Renderer::GetBackEnd( ) const
		{
			return m_BackEnd;
		}

		ZED_BOOL Renderer::GetShaderSupport( ) const
		{
			return m_ShaderSupport;
		}
	}
}

