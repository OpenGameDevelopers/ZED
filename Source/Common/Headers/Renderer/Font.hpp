#ifndef __ZED_RENDERER_FONT_HPP__
#define __ZED_RENDERER_FONT_HPP__

#include <Renderer/Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		const ZED_UINT16 CHUNK_FONT_GLYPH	= 0x0100;
		const ZED_UINT16 CHUNK_FONT_TEXTURE	= 0x0200;

		typedef enum __ZED_FONT_TYPE
		{
			BITMAP_FONT = 1,
			VECTOR_FONT,
			SIGNED_DISTANCE_FONT
		}FONT_TYPE;

		typedef enum __ZED_FONT_BACKGROUNDMODE
		{
			COLOURKEY_BACKGROUND = 1,
			SOLID_BACKGROUND,
			TRANSPARENT_BACKGROUND,
			TRANSLUCENT_BACKGROUND
		}FONT_BACKGROUNDMODE;

#pragma pack( 1 )
		typedef struct __ZED_FONTHEADER
		{
			ZED_CHAR8			ID[ 4 ];
			FONT_TYPE			Type;
			FONT_BACKGROUNDMODE	BackgroundMode;
			ZED_COLOUR			ColourKey;
		}FONTHEADER, *LPFONTHEADER;

		typedef struct __ZED_GLYPH
		{
			ZED_CHAR8	Character;
			ZED_UINT32	X;
			ZED_UINT32	Y;
			ZED_UINT32	Width;
			ZED_UINT32	Height;
		}GLYPH, *LPGLYPH;
#pragma pack( )

		class Font
		{
		public:
			virtual ~Font( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilePath ) = 0;

			void SetType( const FONT_TYPE p_Type );
			FONT_TYPE GetType( ) const;

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

		protected:
			ZED_UINT32	m_Height;
			ZED_COLOUR	m_TextColour;
			ZED_COLOUR	m_BackgroundColour;

			FONT_TYPE			m_Type;
			FONT_BACKGROUNDMODE	m_Mode;

			GLYPH	*m_pGlyphSet;
		};
	}
}

#endif // __ZED_RENDERER_FONT_HPP__

