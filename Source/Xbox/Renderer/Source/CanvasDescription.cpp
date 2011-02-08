#include <Renderer.hpp>
#include <CanvasDescription.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 CanvasDescription::SetBPP( const ZED_FORMAT &p_BPP )
		{
			ZED_UINT32 ReturnStatus = ZED_FAIL;
			switch( p_BPP )
			{
			case ZED_FORMAT_A8R8G8B8:
				{
					m_Colour = p_BPP;
					ReturnStatus = ZED_OK;
					break;
				}
			case ZED_FORMAT_X8R8G8B8:
				{
					m_Colour = p_BPP;
					ReturnStatus = ZED_OK;
					break;
				}
			default:
				{
					break;
				}
			}

			return ReturnStatus;
		}

		ZED_UINT32 CanvasDescription::SetDepthStencil( const ZED_FORMAT &p_DS )
		{
			ZED_UINT32 ReturnStatus = ZED_FAIL;
			switch( p_DS )
			{
			case ZED_FORMAT_D24S8:
				{
					m_DepthStencil = p_DS;
					ReturnStatus = ZED_OK;
					break;
				}
			default:
				{
					break;
				}
			}

			return ReturnStatus;
		}
	}
}
