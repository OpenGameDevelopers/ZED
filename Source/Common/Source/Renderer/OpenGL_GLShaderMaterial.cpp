#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLShaderMaterial.hpp>
#include <Renderer/OGL/GLShader.hpp>
#include <System/Debugger.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShaderMaterial::GLShaderMaterial(
			const ZED_CHAR8 *p_pMaterialName,
			GLShader *p_pGLShader ) :
				ShaderMaterial::ShaderMaterial( p_pMaterialName ),
				m_pGLShader( p_pGLShader )
		{
		}

		GLShaderMaterial::~GLShaderMaterial( )
		{
			zedSafeDelete( m_pGLShader );
		}

		ZED_UINT32 GLShaderMaterial::SetNamedConstantData(
			const ZED_CHAR8 *p_pName, const void *p_pData )
		{
			ZED_UINT32 ConstantIndex;

			if( m_pGLShader->GetConstantIndex( ConstantIndex, p_pName ) !=
				ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetNamedConstant]"
					" <ERROR> Failed to get constant index from name\n" );

				return ZED_FAIL;
			}

			if( m_pGLShader->SetConstantData( ConstantIndex, p_pData ) !=
				ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetNamedConstant]"
					" <ERROR> Failed to set constant data\n" );

				return ZED_FAIL;
			}

			return ZED_FAIL;
		}

		ZED_UINT32 GLShaderMaterial::SetRegisterConstantData(
			const ZED_UINT32 p_Register, const void *p_pData )
		{
			zedTrace( "[ZED::Renderer::GLShaderMaterial::SetRegisterConstant] "
				"<WARN> OpenGL does not support register constants\n" );

			return ZED_FAIL;
		}

		ZED_UINT32 GLShaderMaterial::GetRegisterCount( )
		{
			zedTrace( "[ZED::Renderer::GLShaderMaterial::GetRegisterCount] "
				"<WARN> OpenGL does not support register constants\n" );

			return 0;
		}

		// The following c_ constant names should be replaced by user-defined
		// constant naming conventions
		ZED_UINT32 GLShaderMaterial::SetOpacity( const ZED_FLOAT32 p_Opacity )
		{
			Material::SetOpacity( p_Opacity );
			ZED_UINT32 OpacityIndex;

			if( m_pGLShader->GetConstantIndex( OpacityIndex, "c_Opacity" ) !=
				ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetOpacity] "
					"<ERROR> No such constant name \"c_Opacity\" exists\n" );

				return ZED_FAIL;
			}

			if( m_pGLShader->SetConstantData( OpacityIndex, &p_Opacity ) !=
				ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetOpacity] "
					"<ERROR> Could not set shader constant \"c_Opacity\"\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShaderMaterial::SetDiffuseColour(
			const ZED_COLOUR &p_Colour )
		{
			Material::SetDiffuseColour( p_Colour );

			ZED_UINT32 DiffuseColourIndex;

			if( m_pGLShader->GetConstantIndex( DiffuseColourIndex,
				"c_DiffuseColour" ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetDiffuseColour "
					"<ERROR> No such constant name \"c_DiffuseColour\" "
					"exists\n" );

				return ZED_FAIL;
			}

			if( m_pGLShader->SetConstantData( DiffuseColourIndex,
				&p_Colour ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetDiffuseColour "
					"<ERROR> Could not set shader constant "
					"\"c_DiffuseColour\"\n" );
				
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShaderMaterial::SetAmbientColour(
			const ZED_COLOUR &p_Colour )
		{
			Material::SetAmbientColour( p_Colour );

			ZED_UINT32 AmbientColourIndex;

			if( m_pGLShader->GetConstantIndex( AmbientColourIndex,
				"c_AmbientColour" ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetAmbientColour]"
					" <ERROR> No such constant name \"c_AmbientColour\" "
					"exists\n" );

				return ZED_FAIL;
			}

			if( m_pGLShader->SetConstantData( AmbientColourIndex,
				&p_Colour ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetAmbientColour]"
					" <ERROR> Could not set shader constant "
					"\"c_AmbientColour\"\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShaderMaterial::SetSpecularColour(
			const ZED_COLOUR &p_Colour )
		{
			Material::SetSpecularColour( p_Colour );

			ZED_UINT32 SpecularColourIndex;

			if( m_pGLShader->GetConstantIndex( SpecularColourIndex,
				"c_SpecularColour" ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::"
					"SetSpecularColour] <ERROR> No such constant name "
					"\"c_SpecularColour\" exists\n" );
				
				return ZED_FAIL;
			}

			if( m_pGLShader->SetConstantData( SpecularColourIndex,
				&p_Colour ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::"
					"SetSpecularColour] <ERROR> Could not set shader constant "
					"\"c_SpecularColour\"\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShaderMaterial::SetSpecularPower(
			const ZED_FLOAT32 p_Power )
		{
			Material::SetSpecularPower( p_Power );
			
			ZED_UINT32 SpecularPowerIndex;

			if( m_pGLShader->GetConstantIndex( SpecularPowerIndex,
				"c_SpecularPower" ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetSpecularPower]"
					" <ERROR> No such constant name \"c_SpecularPower\" "
					"exists\n" );

				return ZED_FAIL;
			}

			if( m_pGLShader->SetConstantData( SpecularPowerIndex,
				&p_Power ) != ZED_OK )
			{
				zedTrace( "[ZED::Renderer::GLShaderMaterial::SetSpecularPower]"
					" <ERROR> Could not set shader constant "
					"\"c_SpecularPower\"\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

