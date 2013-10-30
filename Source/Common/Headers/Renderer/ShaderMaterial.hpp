#ifndef __ZED_RENDERER_SHADERMATERIAL_HPP__
#define __ZED_RENDERER_SHADERMATERIAL_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Shader.hpp>

namespace ZED
{
	namespace Renderer
	{
		class ShaderMaterial : public Material
		{
		public:
			virtual ~ShaderMaterial( ) { }

			virtual ZED_UINT32 AttachShader(
				const ZED::Renderer::Shader *p_pShader ) = 0;
			virtual ZED_UINT32 SetNamedConstant( const ZED_CHAR8 *p_pName,
				const void *p_pData ) = 0;
			virtual ZED_UINT32 SetRegisterConstant(
				const ZED_UINT32 p_Register, const void *p_pData ) = 0;

			virtual ZED_UINT32 RegisterCount( ) = 0;

		protected:
		};
	}
}

#endif // __ZED_RENDERER_SHADERMATERIAL_HPP__

