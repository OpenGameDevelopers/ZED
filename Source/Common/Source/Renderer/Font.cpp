#include <Renderer/Font.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		Font::Font( ZED::Renderer::Renderer * const &p_pRenderer ) :
			m_pRenderer( p_pRenderer )
		{
		}

		Font::~Font( )
		{
			zedSafeDeleteArray( m_pGlyphSet );
			zedSafeDelete( m_pTexture );
		}

		void Font::SetType( const FONT_TYPE p_Type )
		{
			m_Type = p_Type;
		}

		FONT_TYPE Font::GetType( ) const
		{
			return m_Type;
		}

		void Font::SetForegroundColour( const ZED_COLOUR &p_Colour )
		{
			m_TextColour = p_Colour;
		}
		
		void Font::SetBackgroundColour( const ZED_COLOUR &p_Colour )
		{
			m_BackgroundColour = p_Colour;
		}

		void Font::SetBackgroundMode( const FONT_BACKGROUNDMODE &p_Mode )
		{
			m_Mode = p_Mode;
		}
	}
}

