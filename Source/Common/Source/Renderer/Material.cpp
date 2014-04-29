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
			m_Opacity( 1.0f )
		{
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
	}
}

