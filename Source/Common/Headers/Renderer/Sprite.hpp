#ifndef __ZED_RENDERER_SPRITE_HPP__
#define __ZED_RENDERER_SPRITE_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/RenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Material;
		class Renderer;

		typedef struct __SPRITE_VERTEX
		{
			ZED_FLOAT32	Posiiton[ 3 ];
			ZED_FLOAT32 UV[ 2 ];
		}SPRITE_VERTEX;

		class Sprite
		{
		public:
			ZED_EXPLICIT Sprite(
				ZED::Renderer::Renderer * const &p_pRenderer );
			~Sprite( );

			ZED_UINT32 Load( const ZED_CHAR8 *p_pSpriteFile );

			void Render( );
			void Move( const ZED_POINT &p_Velocity );

			ZED_UINT32 SetTexture( const ZED_UINT32 p_TextureID );
			void SetPosition( const ZED_POINT &p_Position );
			void SetScale( const ZED_SINT32 p_Scale );

			ZED_UINT32 GetPosition( ZED_POINT *p_pPosition ) const;
			ZED_UINT32 GetHitBox( ZED_RECTANGLE *p_pHitBox ) const;
			ZED_SINT32 GetScale( ) const;

		private:
			ZED::Renderer::Material	*m_pMaterial;
			ZED::Renderer::Renderer	*m_pRenderer;
			ZED_RECTANGLE			m_HitBox;
			SPRITE_VERTEX			m_Vertices[ 4 ];
			ZED_UINT16				m_Indices[ 6 ];
			ZED_SINT32				m_Scale;
			ZED_UINT32				m_ScaledX;
			ZED_UINT32				m_ScaledY;
		};
	}
}

#endif // __ZED_RENDERER_SPRITE_HPP__

