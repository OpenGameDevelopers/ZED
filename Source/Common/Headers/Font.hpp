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

		typedef enum __ZED_FONT_BACKGROUNDMODE
		{
			SOLID_BACKGROUND,
			TRANSPARENT_BACKGROUND,
			OPAQUE_BACKGROUND
		}FONT_BACKGROUNDMODE;

		class Font
		{
		public:
			void SetType( );
			FONT_TYPE GetType( );

			// If necessary, load the font and convert it to a bitmap at the
			// desired resolution (useful if switching resolutions and want
			// to keep crisp fonts while being cheap to render)
			ZED_UINT32 SaveTrueTypeAsBitmap( const ZED_CHAR16 *p_pFilename );
			// Bitmap fonts are useful for less powerful systems, systems
			// with fixed resolutions (such as Dreamcast, Xbox, and Pandora),
			// or systems without programmable shaders
			ZED_UINT32 OpenBitmap( const ZED_CHAR16 *p_pFilename );
			// TT fonts are great for systems that have GPUs which can handle
			// the extra workload, most OpenGL 1.5 and Direct3D 8.1 parts
			// will be able to handle this with ease
			ZED_UINT32 OpenTrueType( const ZED_CHAR16 *p_pFilename );

			ZED_INLINE void SetForegroundColour( const ZED_COLOUR &p_Colour )
			{
				m_TextColour = p_Colour;
			}
			ZED_INLINE void SetBackgroundColour( const ZED_COLOUR &p_Colour )
			{
				m_BackgroundColour = p_Colour;
			}
			ZED_INLINE void SetBackgroundMode(
				const FONT_BACKGROUNDMODE &p_Mode )
			{
				m_Mode = p_Mode;
			}

		private:
			ZED_UINT32	m_Height;
			ZED_COLOUR	m_TextColour;
			ZED_COLOUR	m_BackgroundColour;

			FONT_TYPE			m_Type;
			FONT_BACKGROUNDMODE	m_Mode;
		};
	}
}

#endif
