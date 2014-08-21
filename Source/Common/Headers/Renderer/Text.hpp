#ifndef __ZED_RENDERER_TEXT_HPP__
#define __ZED_RENDERER_TEXT_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Font;

		class Text
		{
		public:
			Text( );
			~Text( );

			ZED_UINT32 Render( const ZED_UINT32 p_X, const ZED_UINT32 p_Y,
				const ZED_CHAR8 *p_pString, ... );

			ZED_UINT32 SetFont( Font * const &p_pFont );

			void SetScale( const ZED_FLOAT32 p_Scale );

			ZED_FLOAT32 GetScale( ) const;

			ZED_UINT32 MeasureString( ZED_FLOAT32 *p_pWidth,
				ZED_FLOAT32 *p_pHeight,	const ZED_CHAR8 *p_pString,
				... ) const;

		private:
			Font		*m_pFont;
			ZED_FLOAT32	m_Scale;
		};
	}
}

#endif // __ZED_RENDERER_TEXT_HPP__

