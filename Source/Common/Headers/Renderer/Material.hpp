#ifndef __ZED_RENDERER_MATERIAL_HPP__
#define __ZED_RENDERER_MATERIAL_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/RenderTypes.hpp>
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
			ZED_UINT32 GetDiffuseColour( ZED_COLOUR *p_pColour ) const;
			ZED_UINT32 GetAmbientColour( ZED_COLOUR *p_pColour ) const;
			ZED_UINT32 GetSpecularColour( ZED_COLOUR *p_pColour ) const;
			ZED_FLOAT32 GetSpecularPower( ) const;
			ZED_BOOL IsShadowReceiver( ) const;

			ZED_UINT32 SetOpacity( const ZED_FLOAT32 p_Opacity );
			ZED_UINT32 SetDiffuseColour( const ZED_COLOUR &p_Colour );
			ZED_UINT32 SetAmbientColour( const ZED_COLOUR &p_Colour );
			ZED_UINT32 SetSpecularColour( const ZED_COLOUR &p_Colour );
			ZED_UINT32 SetSpecularPower( const ZED_FLOAT32 p_Power );
			ZED_UINT32 SetShadowReceiver( const ZED_BOOL p_ShadowReceiver );

		protected:
			typedef std::set< ZED::Renderer::Texture * > TextureSet;

			ZED_UINT32		m_ID;
			ZED_CHAR8		*m_pName;
			TextureSet		m_TextureSet;
			ZED_FLOAT32		m_Opacity;
			ZED_COLOUR		m_DiffuseColour;
			ZED_COLOUR		m_AmbientColour;
			ZED_COLOUR		m_SpecularColour;
			ZED_FLOAT32		m_SpecularPower;
			ZED_BOOL		m_ShadowReceiver;
		};
	}
}

#endif // __ZED_RENDERER_MATERIAL_HPP__

