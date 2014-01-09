#ifndef __ZED_RENDERER_SPRITE_HPP__
#define __ZED_RENDERER_SPRITE_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Material;
		class Renderer;

		class Sprite
		{
		public:
			ZED_EXPLICIT Sprite(
				ZED::Renderer::Renderer * const &p_pRenderer );
			~Sprite( );

			ZED_UINT32 Load( const ZED_CHAR8 *p_pSpriteFile );

			ZED_UINT32 SetTexture( const ZED_UINT32 p_TextureID );

			void Render( );

		private:
			ZED::Renderer::Material	*m_pMaterial;
			ZED::Renderer::Renderer	*m_pRenderer;
		};
	}
}

#endif // __ZED_RENDERER_SPRITE_HPP__

