#include <Renderer/Sprite.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Material.hpp>
#include <System/Memory.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		Sprite::Sprite( ZED::Renderer::Renderer * const &p_pRenderer ) :
			m_pRenderer( p_pRenderer ),
			m_pMaterial( ZED_NULL ),
			m_Scale( 1 ),
			m_ScaledX( 0 ),
			m_ScaledY( 0 )
		{
			m_HitBox.X = m_HitBox.Y = m_HitBox.Width = m_HitBox.Height = 0;
			memset( m_Vertices, 0, sizeof( m_Vertices[ 0 ] ) * 4 );
			memset( m_Indices, 0, sizeof( m_Indices[ 0 ] ) * 6 );
		}

		Sprite::~Sprite( )
		{
			zedSafeDelete( m_pMaterial );
		}

		ZED_UINT32 Sprite::Load( const ZED_CHAR8 *p_pSpriteFile )
		{
			return ZED_FAIL;
		}

		void Sprite::Render( )
		{
			// FIXME
			// Set up for rendering to the screen, the depth test should be
			// disabled (though, this makes rendering sprites which overlap
			// harder to do, perhaps some kind of sprite ordering could occur?)
			
			m_pRenderer->Render(
				4, reinterpret_cast< ZED_BYTE * >( m_Vertices ),
				6, m_Indices, 0x56,
				m_pMaterial->GetID( ), ZED_TRIANGLE_LIST );
		}

		void Sprite::Move( const ZED_POINT &p_Velocity )
		{
			m_HitBox.X += p_Velocity.X;
			m_HitBox.Y += p_Velocity.Y;
			m_HitBox.Width += p_Velocity.X;
			m_HitBox.Height += p_Velocity.Y;
		}

		ZED_UINT32 Sprite::SetTexture( const ZED_UINT32 p_TextureID )
		{
			return ZED_FAIL;
		}

		void Sprite::SetPosition( const ZED_POINT &p_Position )
		{
			m_HitBox.X = p_Position.X;
			m_HitBox.Y = p_Position.Y;
			m_HitBox.Width = p_Position.X + m_ScaledX;
			m_HitBox.Height = p_Position.Y + m_ScaledY;
		}

		ZED_UINT32 Sprite::GetPosition( ZED_POINT *p_pPosition ) const
		{
			if( p_pPosition )
			{
				p_pPosition->X = m_HitBox.X;
				p_pPosition->Y = m_HitBox.Y;

				return ZED_OK;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 Sprite::GetHitBox( ZED_RECTANGLE *p_pHitBox ) const
		{
			if( p_pHitBox )
			{
				( *p_pHitBox ) = m_HitBox;

				return ZED_OK;
			}

			return ZED_FAIL;
		}
	}
}

