#include <Renderer/Sprite.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Material.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		Sprite::Sprite( ZED::Renderer::Renderer * const &p_pRenderer ) :
			m_pRenderer( p_pRenderer ),
			m_pMaterial( ZED_NULL )
		{
		}

		Sprite::~Sprite( )
		{
			zedSafeDelete( m_pMaterial );
		}

		ZED_UINT32 Sprite::Load( const ZED_CHAR8 *p_pSpriteFile )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 Sprite::SetTexture( const ZED_UINT32 p_TextureID )
		{
			return ZED_FAIL;
		}

		void Sprite::Render( )
		{
		}
	}
}

