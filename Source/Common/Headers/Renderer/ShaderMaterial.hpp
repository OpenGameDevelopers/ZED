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
			ZED_EXPLICIT ShaderMaterial( const ZED_CHAR8 *p_pMaterialName );
			virtual ~ShaderMaterial( );

			virtual ZED_UINT32 SetNamedConstantData( const ZED_CHAR8 *p_pName,
				const void *p_pData ) = 0;
			virtual ZED_UINT32 SetRegisterConstantData(
				const ZED_UINT32 p_Register, const void *p_pData ) = 0;

			virtual ZED_UINT32 GetRegisterCount( ) = 0;
		};
	}
}

#endif // __ZED_RENDERER_SHADERMATERIAL_HPP__

