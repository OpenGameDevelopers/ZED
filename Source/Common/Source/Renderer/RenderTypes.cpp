#include <Renderer/RenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		const ZED_CHAR8 *FormatToString( const ZED_FORMAT p_Format )
		{
			switch( p_Format )
			{
				case ZED_FORMAT_ARGB8:
				{
					return "ARGB8";
				}
				case ZED_FORMAT_XRGB8:
				{
					return "XRGB8";
				}
				case ZED_FORMAT_RGB565:
				{
					return "RGB565";
				}
				case ZED_FORMAT_ARGB1555:
				{
					return "ARGB1555";
				}
				case ZED_FORMAT_RGB8:
				{
					return "RGB8";
				}
				case ZED_FORMAT_D24S8:
				{
					return "D24S8";
				}
				case ZED_FORMAT_UNDEFINED:
				{
					return "Undefined";
				}
				case ZED_FORMAT_UNKNOWN:
				{
					return "Unknown";
				}
				default:
				{
					return "Unrecognised";
				}
			}
		}

		ZED_MEMSIZE FormatToBytes( const ZED_FORMAT p_Format )
		{
			switch( p_Format )
			{
				case ZED_FORMAT_XRGB8:
				case ZED_FORMAT_ARGB8:
				{
					return 4;
				}
				case ZED_FORMAT_RGB8:
				{
					return 3;
				}
				case ZED_FORMAT_ARGB1555:
				case ZED_FORMAT_RGB565:
				{
					return 2;
				}
				default:
				{
					return 0;
				}
			}
		}
	}
}

