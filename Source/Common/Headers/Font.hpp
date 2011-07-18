#ifndef __ZED_RENDERER_FONT_HPP__
#define __ZED_RENDERER_FONT_HPP__

#include <Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		typedef enum __ZED_FONT_TYPE
		{
			BITMAP_FONT,
			VECTOR_FONT
		}FONT_TYPE;

		class Font
		{
		public:
			void SetType( );
			FONT_TYPE GetType( );

			ZED_UINT32 SaveTrueTypeAsBitmap( const ZED_CHAR16 *p_Filename );
			ZED_UINT32 OpenBitmpap( );
			ZED_UINT32 OpenTrueType( );

			void SetFontColour( );
			void SetBackgroundColour( );
			void SetBackgroundMode( );

		private:
			ZED_UINT32 m_Height;
		};
	}
}

#endif
