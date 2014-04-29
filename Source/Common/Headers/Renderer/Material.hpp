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
			Material( );
			virtual ~Material( );

			ZED_UINT32 AttachTexture(
				ZED::Renderer::Texture * const &p_pTexture );

			ZED_UINT32 GetID( ) const;
			ZED_CHAR8 *GetName( ) const;
			ZED_FLOAT32 GetOpacity( ) const;

			ZED_UINT32 SetOpacity( const ZED_FLOAT32 p_Opacity );

		protected:
			typedef std::set< ZED::Renderer::Texture * > TextureSet;

			ZED_UINT32		m_ID;
			ZED_CHAR8		*m_pName;
			TextureSet		m_TextureSet;
			ZED_FLOAT32		m_Opacity;
		};
	}
}

#endif // __ZED_RENDERER_MATERIAL_HPP__

