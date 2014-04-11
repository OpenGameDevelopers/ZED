#include <Renderer/Font.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		Font::~Font( )
		{
			zedSafeDeleteArray( m_pGlyphSet );
		}

		void Font::SetType( const FONT_TYPE p_Type )
		{
			m_Type = p_Type;
		}

		FONT_TYPE Font::GetType( ) const
		{
			return m_Type;
		}
	}
}

