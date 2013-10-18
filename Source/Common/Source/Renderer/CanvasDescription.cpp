#include <Renderer/Renderer.hpp>
#include <Renderer/CanvasDescription.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 CanvasDescription::ColourFormat( const ZED_FORMAT p_Colour )
		{
			if( p_Colour == ZED_FORMAT_ARGB8 ||
				p_Colour == ZED_FORMAT_XRGB8 ||
				p_Colour == ZED_FORMAT_RGB565 )
			{
				m_Colour = p_Colour;

				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 CanvasDescription::DepthStencilFormat( const ZED_FORMAT
			p_DepthStencil )
		{
			if( p_DepthStencil == ZED_FORMAT_D24S8 )
			{
				m_DepthStencil = p_DepthStencil;

				return ZED_OK;
			}

			return ZED_FAIL;
		}
	}
}

