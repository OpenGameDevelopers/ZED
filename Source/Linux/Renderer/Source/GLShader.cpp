#include <GLShader.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShader::GLShader( )
		{
		}

		GLShader::GLShader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
			const ZED_BOOL p_Geometry )
		{
			m_InitShader |= p_Vertex ? 0x01 : 0x00;
			m_InitShader |= p_Fragment ? 0x02 : 0x00;
			m_InitShader |= p_Geometry ? 0x04 : 0x00;
		}

		ZED_UINT32 GLShader::Compile( const ZED_CHAR8 **p_ppData,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_FromFile )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::Create( const ZED_UCHAR8 *p_pData,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_FromFile )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::Activate( )
		{
			return ZED_OK;
		}

		void GLShader::Deactivate( )
		{
		}

		void GLShader::AddType( const ZED_SHADER_TYPE p_Type )
		{
		}

		void GLShader::Delete( )
		{
		}

		ZED_UINT32 GLShader::Save( const ZED_UCHAR8 *p_pFile,
			const ZED_BOOL p_HLSL )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVertexAttributeTypes(
			const ZED_SHADER_VA_MAP *p_pVAMap, const ZED_MEMSIZE p_Count )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVariableTypes(
			const ZED_SHADER_INPUT_MAP *p_pTypes,
			const ZED_MEMSIZE p_Count )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVariable( const ZED_UINT32 p_Index,
			const void *p_pValue )
		{
			return ZED_OK;
		}
	}
}

