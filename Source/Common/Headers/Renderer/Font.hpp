#ifndef __ZED_RENDERER_FONT_HPP__
#define __ZED_RENDERER_FONT_HPP__

#include <Renderer/Renderer.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <map>

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

		typedef struct __ZED_GLYPHVERTEX
		{
			ZED_FLOAT32	Position[ 3 ];
			ZED_FLOAT32	UV[ 2 ];
		}GLYPHVERTEX;

		typedef struct __ZED_GLYPHPOLY
		{
			GLYPHVERTEX Vertex[ 4 ];
			ZED_FLOAT32 Width;
			ZED_FLOAT32 Height;
		}GLYPHPOLY;
#pragma pack( )

		class Texture;

		class Font
		{
		public:
			ZED_EXPLICIT Font( ZED::Renderer::Renderer * const &p_pRenderer );
			virtual ~Font( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilePath ) = 0;

			virtual void RenderGlyph( const ZED_CHAR8 p_Character,
				const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Scale ) = 0;

			void SetType( const FONT_TYPE p_Type );
			FONT_TYPE GetType( ) const;
			ZED_UINT32 GetGlyphMetrics( const ZED_CHAR8 p_Char,
				GLYPH *p_pGlyph ) const;

			void SetViewport( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Width, const ZED_FLOAT32 p_Height );
			void SetForegroundColour( const ZED_COLOUR &p_Colour );
			void SetBackgroundColour( const ZED_COLOUR &p_Colour );
			void SetBackgroundMode( const FONT_BACKGROUNDMODE &p_Mode );

		protected:
			ZED_UINT32	m_Height;
			ZED_COLOUR	m_TextColour;
			ZED_COLOUR	m_BackgroundColour;

			FONT_TYPE			m_Type;
			FONT_BACKGROUNDMODE	m_Mode;

			GLYPH		*m_pGlyphSet;
			ZED_UINT32	m_GlyphCount;
			Texture		*m_pTexture;
			Renderer	*m_pRenderer;

			ZED::Arithmetic::Matrix4x4	m_ProjectionMatrix;

			std::map< ZED_CHAR8, GLYPHPOLY > m_GlyphPolys;
		};
	}
}

#endif // __ZED_RENDERER_FONT_HPP__

