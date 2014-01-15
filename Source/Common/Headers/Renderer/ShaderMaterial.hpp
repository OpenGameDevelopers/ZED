#ifndef __ZED_RENDERER_SHADERMATERIAL_HPP__
#define __ZED_RENDERER_SHADERMATERIAL_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/Material.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Shader;
		class ShaderMaterial : public Material
		{
		public:
			ZED_EXPLICIT ShaderMaterial( ZED_CHAR8 * const &p_pMaterialName );
			virtual ~ShaderMaterial( );

			virtual ZED_UINT32 AttachShader(
				ZED::Renderer::Shader * const &p_pShader ) = 0;
			virtual ZED_UINT32 SetNamedConstant( const ZED_CHAR8 *p_pName,
				const void *p_pData ) = 0;
			virtual ZED_UINT32 SetRegisterConstant(
				const ZED_UINT32 p_Register, const void *p_pData ) = 0;

			virtual ZED_UINT32 GetRegisterCount( ) = 0;

		protected:
			Shader	*m_pShader;
		};
	}
}

#endif // __ZED_RENDERER_SHADERMATERIAL_HPP__

