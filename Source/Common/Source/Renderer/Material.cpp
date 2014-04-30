#include <Renderer/Material.hpp>
#include <Renderer/Texture.hpp>
#include <System/Memory.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		Material::Material( ) :
			m_ID( 0 ),
			m_pName( ZED_NULL ),
			m_Opacity( 1.0f ),
			m_SpecularPower( 1.0f ),
			m_ShadowReceiver( ZED_TRUE )
		{
			m_DiffuseColour = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_AmbientColour = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_SpecularColour = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		Material::~Material( )
		{
			zedSafeDeleteArray( m_pName );
		}

		ZED_UINT32 Material::AttachTexture(
			ZED::Renderer::Texture * const &p_pTexture )
		{
			m_TextureSet.insert( p_pTexture );

			return ZED_FAIL;
		}

		ZED_UINT32 Material::GetID( ) const
		{
			return m_ID;
		}

		ZED_CHAR8 *Material::GetName( ) const
		{
			return m_pName;
		}

		ZED_FLOAT32 Material::GetOpacity( ) const
		{
			return m_Opacity;
		}

		ZED_UINT32 Material::GetDiffuseColour( ZED_COLOUR *p_pColour ) const
		{
			( *p_pColour ) = m_DiffuseColour;

			return ZED_OK;
		}

		ZED_UINT32 Material::GetAmbientColour( ZED_COLOUR *p_pColour ) const
		{
			( *p_pColour ) = m_AmbientColour;

			return ZED_OK;
		}

		ZED_UINT32 Material::GetSpecularColour( ZED_COLOUR *p_pColour ) const
		{
			( *p_pColour ) = m_SpecularColour;

			return ZED_OK;
		}

		ZED_FLOAT32 Material::GetSpecularPower( ) const
		{
			return m_SpecularPower;
		}

		ZED_BOOL Material::IsShadowReceiver( ) const
		{
			return m_ShadowReceiver;
		}

		ZED_UINT32 Material::SetOpacity( const ZED_FLOAT32 p_Opacity )
		{
			if( p_Opacity > 1.0f )
			{
				m_Opacity = 1.0f;
				return ZED_VALUEOVERMAX;
			}

			if( p_Opacity < 0.0f )
			{
				m_Opacity = 0.0f;

				return ZED_VALUEUNDERMIN;
			}

			m_Opacity = p_Opacity;

			return ZED_OK;
		}

		ZED_UINT32 Material::SetDiffuseColour( const ZED_COLOUR &p_Colour )
		{
			m_DiffuseColour = p_Colour;

			return ZED_OK;
		}

		ZED_UINT32 Material::SetAmbientColour( const ZED_COLOUR &p_Colour )
		{
			m_AmbientColour = p_Colour;

			return ZED_OK;
		}

		ZED_UINT32 Material::SetSpecularColour( const ZED_COLOUR &p_Colour )
		{
			m_SpecularColour = p_Colour;

			return ZED_OK;
		}

		ZED_UINT32 Material::SetSpecularPower(
			const ZED_FLOAT32 p_SpecularPower )
		{
			m_SpecularPower = p_SpecularPower;

			return ZED_OK;
		}

		ZED_UINT32 Material::SetShadowReceiver(
			const ZED_BOOL p_ShadowReceiver )
		{
			m_ShadowReceiver = p_ShadowReceiver;
			return ZED_OK;
		}
	}
}

