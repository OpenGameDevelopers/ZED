#include <Font.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 Font::OpenBitmap( const ZED_CHAR16 *p_pFilename )
		{
			FILE *pFile = ZED_NULL;
			pFile = _wfopen( p_pFilename, L"rb" );

			if( pFile == ZED_NULL )
			{
				// No font found
				zedTrace( "[ZED|Renderer|Font] Failed to load font %s.\n",
					p_pFilename );
				return ZED_FAIL;
			}

			// Good, seems the font was in the correct location after all
			//m_FontTexture = LoadBMP( pFile );

			return ZED_OK;
		}
	}
}
