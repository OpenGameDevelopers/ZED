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
		class GLShader;
		class Texture;

		class GLShaderMaterial : public ShaderMaterial
		{
		public:
			ZED_EXPLICIT GLShaderMaterial(
				const ZED_CHAR8 *p_pMaterialName, GLShader *p_pGLShader );
			virtual ~GLShaderMaterial( );

			virtual ZED_UINT32 SetNamedConstantData( const ZED_CHAR8 *p_pName,
				const void *p_pData );
			virtual ZED_UINT32 SetRegisterConstantData(
				const ZED_UINT32 p_Register, const void *p_pData );

			virtual ZED_UINT32 GetRegisterCount( );

			ZED_UINT32 SetOpacity( const ZED_FLOAT32 p_Opacity );
			ZED_UINT32 SetDiffuseColour( const ZED_COLOUR &p_Colour );
			ZED_UINT32 SetAmbientColour( const ZED_COLOUR &p_Colour );
			ZED_UINT32 SetSpecularColour( const ZED_COLOUR &p_Colour );
			ZED_UINT32 SetSpecularPower( const ZED_FLOAT32 p_Power );

		private:
			typedef std::map< std::string, GLuint > NameIndexMap;

			NameIndexMap				m_ShaderConstantsNameMap;

			GLShader	*m_pGLShader;
		};
	}
}

#endif // __ZED_RENDERER_GLSHADERMATERIAL_HPP__

