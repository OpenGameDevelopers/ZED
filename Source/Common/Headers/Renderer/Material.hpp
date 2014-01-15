#ifndef __ZED_RENDERER_MATERIAL_HPP__
#define __ZED_RENDERER_MATERIAL_HPP__

#include <System/DataTypes.hpp>
#include <set>

namespace ZED
{
	namespace Renderer
	{
		class Texture;

		class Material
		{
		public:
			ZED_EXPLICIT Material( ZED_CHAR8 * const &p_pMaterialName );
			virtual ~Material( );

			ZED_UINT32 AttachTexture(
				ZED::Renderer::Texture * const &p_pTexture );

			ZED_UINT32 GetID( ) const;
			ZED_CHAR8 *GetName( ) const;

			void SetID( const ZED_UINT32 p_ID );
			void SetName( const ZED_CHAR8 *p_pName );

		protected:
			typedef std::set< ZED::Renderer::Texture * > TextureSet;

			ZED_UINT32		m_ID;
			ZED_CHAR8		*m_pName;
			TextureSet		m_TextureSet;
		};
	}
}

#endif // __ZED_RENDERER_MATERIAL_HPP__

