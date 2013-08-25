#include <Renderer/Font.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 Font::SaveTrueTypeAsBitmap( const ZED_CHAR8 *p_pFileName )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 Font::OpenBitmap( const ZED_CHAR8 *p_pFileName )
		{
			return ZED_OK;
		}

		ZED_UINT32 Font::OpenTrueType( const ZED_CHAR8 *p_pFileName )
		{
			return ZED_OK;
		}
	}
}

