#ifndef __ZED_RENDERER_GLSHADERMATERIAL_HPP__
#define __ZED_RENDERER_GLSHADERMATERIAL_HPP__

#include <Renderer/ShaderMaterial.hpp>
#include <Renderer/RenderTypes.hpp>
#include <map>
#include <string>

namespace ZED
{
	namespace Renderer
	{
		// Forward declarations
		class GLTexture;
		class Texture;

		class GLShaderMaterial : public ShaderMaterial
		{
		public:
			ZED_EXPLICIT GLShaderMaterial(
				ZED_CHAR8 * const &p_pMaterialName  );
			virtual ~GLShaderMaterial( );

			virtual ZED_UINT32 AttachShader(
				ZED::Renderer::Shader * const &p_pShader );

			virtual ZED_UINT32 SetNamedConstant( const ZED_CHAR8 *p_pName,
				const void *p_pData );
			virtual ZED_UINT32 SetRegisterConstant(
				const ZED_UINT32 p_Register, const void *p_pData );

			virtual ZED_UINT32 GetRegisterCount( );

		private:
			typedef std::map< std::string, GLuint > NameIndexMap;

			NameIndexMap				m_ShaderConstantsNameMap;
		};
	}
}

#endif // __ZED_RENDERER_GLSHADERMATERIAL_HPP__

