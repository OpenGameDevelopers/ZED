#include <Renderer/Font.hpp>
#include <System/Memory.hpp>
#include <Arithmetic/Vector4.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		Font::Font( ZED::Renderer::Renderer * const &p_pRenderer ) :
			m_pGlyphSet( ZED_NULL ),
			m_pRenderer( p_pRenderer ),
			m_pTexture( ZED_NULL )
		{
			m_TextColour = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_BackgroundColour = { 0.0f, 0.0f, 0.0f, 1.0f };
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

		ZED_UINT32 Font::GetGlyphMetrics( const ZED_CHAR8 p_Char,
			GLYPH *p_pGlyph ) const
		{
			if( p_pGlyph )
			{
				for( ZED_UINT32 i = 0; i < m_GlyphCount; ++i )
				{
					if( p_Char == m_pGlyphSet[ i ].Character )
					{
						memcpy( p_pGlyph, &( m_pGlyphSet[ i ] ),
							sizeof( GLYPH ) );

						return ZED_OK;
					}
				}
			}

			return ZED_FAIL;
		}
		
		void Font::SetViewport( const ZED_FLOAT32 p_Left,
			const ZED_FLOAT32 p_Top, const ZED_FLOAT32 p_Right,
			const ZED_FLOAT32 p_Bottom )
		{
			ZED::Arithmetic::Vector4 Row0, Row1, Row2, Row3;

			Row0[ 0 ] = ( 2.0f ) / ( p_Right - p_Left );
			Row0[ 1 ] = 0.0f;
			Row0[ 2 ] = 0.0f;
			Row0[ 3 ] = -( ( p_Right + p_Left ) / ( p_Right - p_Left ) );

			Row1[ 0 ] = 0.0f;
			Row1[ 1 ] = ( 2.0f ) / ( p_Top - p_Bottom );
			Row1[ 2 ] = 0.0f;
			Row1[ 3 ] = -( ( p_Top + p_Bottom ) / ( p_Top - p_Bottom ) );

			Row2[ 0 ] = 0.0f;
			Row2[ 1 ] = 0.0f;
			Row2[ 2 ] = ( -2.0f ) / ( 1000.0f / -1.0f );
			Row2[ 3 ] = -( ( 1000.0f + -1.0f ) / ( 1000.0f - ( -1.0f ) ) );

			Row3[ 0 ] = 0.0f;
			Row3[ 1 ] = 0.0f;
			Row3[ 2 ] = 0.0f;
			Row3[ 3 ] = 1.0f;

			m_ProjectionMatrix.SetRows( Row0, Row1, Row2, Row3 );
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

