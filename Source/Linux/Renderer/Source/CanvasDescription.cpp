#include <Renderer.hpp>
#include <CanvasDescription.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 CanvasDescription::Colour( const ZED_FORMAT &p_BPP )
		{
			if( p_BPP == ZED_FORMAT_ARGB8 )
			{
				m_Colour = p_BPP;
				return ZED_OK;
			}
			else if( p_BPP == ZED_FORMAT_XRGB8 )
			{
				m_Colour = p_BPP;
				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 CanvasDescription::DepthStencil( const ZED_FORMAT &p_DS )
		{
			if( p_DS == ZED_FORMAT_D24S8 )
			{
				m_DepthStencil = p_DS;
				return ZED_OK;
			}

			return ZED_FAIL;
		}
	}
}
