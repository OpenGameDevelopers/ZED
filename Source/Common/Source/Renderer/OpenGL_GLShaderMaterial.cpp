#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLShaderMaterial.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShaderMaterial::GLShaderMaterial( )
		{
			m_pShader = ZED_NULL;
		}

		GLShaderMaterial::~GLShaderMaterial( )
		{
		}

		ZED_UINT32 GLShaderMaterial::AttachShader(
			const ZED::Renderer::Shader *p_pShader )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 GLShaderMaterial::SetNamedConstant(
			const ZED_CHAR8 * p_pName, const void *p_pData )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 GLShaderMaterial::SetRegisterConstant(
			const ZED_UINT32 p_Register, const void *p_pData )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 GLShaderMaterial::GetRegisterCount( )
		{
			return 0;
		}
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

