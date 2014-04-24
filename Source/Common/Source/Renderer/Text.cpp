#include <Renderer/Text.hpp>
#include <Renderer/Font.hpp>
#include <System/Memory.hpp>
#include <cstring>
#include <cstdarg>

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

		ZED_UINT32 Text::Render( const ZED_UINT32 p_X, const ZED_UINT32 p_Y,
			const ZED_CHAR8 *p_pString, ... )
		{
			ZED_MEMSIZE StringLength = 0;
			ZED_SINT32 ReturnVal;
			va_list ArgPtr;

			va_start( ArgPtr, p_pString );
			StringLength = vsnprintf( ZED_NULL, 0, p_pString, ArgPtr );
			va_end( ArgPtr );

			++StringLength;

			char *pCompleteMessage = new char[ StringLength ];

			va_start( ArgPtr, p_pString );
			ReturnVal = vsnprintf( pCompleteMessage, StringLength, p_pString,
				ArgPtr );
			va_end( ArgPtr );

			if( ReturnVal < 0 )
			{
				zedSafeDeleteArray( pCompleteMessage );
				return ZED_FAIL;
			}

			pCompleteMessage[ StringLength - 1 ] = '\0';

			ZED_FLOAT32 X = static_cast< ZED_FLOAT32 >( p_X );
			ZED_FLOAT32 Y = static_cast< ZED_FLOAT32 >( p_Y );

			for( ZED_MEMSIZE i = 0; i < StringLength; ++i )
			{
				GLYPH Glyph;
				if( m_pFont->GetGlyphMetrics( pCompleteMessage[ i ],
					&Glyph ) != ZED_OK )
				{
					break;
				}
				m_pFont->RenderGlyph( pCompleteMessage[ i ], X, Y, m_Scale );

				X += static_cast< ZED_FLOAT32 >( Glyph.Width ) * m_Scale;
			}

			zedSafeDeleteArray( pCompleteMessage );
			
			return ZED_OK;
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

		ZED_UINT32 Text::MeasureString( ZED_FLOAT32 *p_pWidth,
			ZED_FLOAT32 *p_pHeight, const ZED_CHAR8 *p_pString, ... ) const
		{
			if( !p_pWidth )
			{
				return ZED_FAIL;
			}

			if( !p_pHeight )
			{
				return ZED_FAIL;
			}

			if( !p_pString )
			{
				return ZED_FAIL;
			}

			ZED_MEMSIZE StringLength = 0;
			ZED_SINT32 ReturnVal;
			va_list ArgPtr;

			va_start( ArgPtr, p_pString );
			StringLength = vsnprintf( ZED_NULL, 0, p_pString, ArgPtr );
			va_end( ArgPtr );

			++StringLength;

			char *pCompleteMessage = new char[ StringLength ];

			va_start( ArgPtr, p_pString );
			ReturnVal = vsnprintf( pCompleteMessage, StringLength, p_pString,
				ArgPtr );
			va_end( ArgPtr );

			if( ReturnVal < 0 )
			{
				zedSafeDeleteArray( pCompleteMessage );
				return ZED_FAIL;
			}

			pCompleteMessage[ StringLength - 1 ] = '\0';

			ZED_FLOAT32 MeasuredWidth = 0.0f;
			ZED_FLOAT32 MeasuredHeight = 0.0f;

			for( ZED_MEMSIZE i = 0; i < StringLength; ++i )
			{
				GLYPH Glyph;
				if( m_pFont->GetGlyphMetrics( pCompleteMessage[ i ],
					&Glyph ) != ZED_OK )
				{
					break;
				}

				MeasuredWidth += static_cast< ZED_FLOAT32 >( Glyph.Width ) *
					m_Scale;

				MeasuredHeight = fmax( MeasuredHeight,
					static_cast< ZED_FLOAT32 >( Glyph.Height ) * m_Scale );
			}

			zedSafeDeleteArray( pCompleteMessage );

			( *p_pWidth ) = MeasuredWidth;
			( *p_pHeight ) = MeasuredHeight;

			return ZED_OK;
		}
	}
}

