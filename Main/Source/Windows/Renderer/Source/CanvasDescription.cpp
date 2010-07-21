#include <Renderer.hpp>
#include <CanvasDescription.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 CanvasDescription::SetBPP( const ZED_FORMAT &p_BPP )
		{
			if( p_BPP == ZED_FORMAT_A8R8G8B8 )
			{
				m_Colour = p_BPP;
				return ZED_OK;
			}
			else if( p_BPP == ZED_FORMAT_X8R8G8B8 )
			{
				m_Colour = p_BPP;
				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 CanvasDescription::SetDepthStencil( const ZED_FORMAT &p_DS )
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