#include <Renderer/Text.hpp>
#include <Renderer/Font.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		Text::Text( ) :
			m_pFont( ZED_NULL ),
			m_Scale( 1.0f )
		{
		}

		Text::~Text( )
		{
		}

		void Text::Render( const ZED_UINT32 p_X, const ZED_UINT32 p_Y,
			const ZED_CHAR8 *p_pString, ... )
		{
			ZED_MEMSIZE StringLen = strlen( p_pString );

			ZED_FLOAT32 X = static_cast< ZED_FLOAT32 >( p_X );
			ZED_FLOAT32 Y = static_cast< ZED_FLOAT32 >( p_Y );

			for( ZED_MEMSIZE i = 0; i < StringLen; ++i )
			{
				GLYPH Glyph;
				if( m_pFont->GetGlyphMetrics( p_pString[ i ], &Glyph ) !=
					ZED_OK )
				{
					break;
				}
				m_pFont->RenderGlyph( p_pString[ i ], X, Y, m_Scale );

				X += static_cast< ZED_FLOAT32 >( Glyph.Width ) * m_Scale;
			}
		}

		ZED_UINT32 Text::SetFont( Font * const &p_pFont )
		{
			if( p_pFont )
			{
				m_pFont = p_pFont;
				return ZED_OK;
			}

			return ZED_FAIL;
		}

		void Text::SetScale( const ZED_FLOAT32 p_Scale )
		{
			m_Scale = p_Scale;
		}

		ZED_FLOAT32 Text::GetScale( ) const
		{
			return m_Scale;
		}
	}
}

