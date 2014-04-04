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
			~Text( );

			void Render( const ZED_UINT32 p_X, const ZED_UINT32 p_Y,
				const ZED_CHAR8 *p_pString, ... );


			void SetScale( const ZED_FLOAT32 p_Scale );

			ZED_FLOAT32 GetScale( ) const;

		private:
			Font		*m_pFont;
			ZED_FLOAT32	m_Scale;
		};
	}
}

#endif
