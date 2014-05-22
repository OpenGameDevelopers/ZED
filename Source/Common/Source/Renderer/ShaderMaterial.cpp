#include <Renderer/ShaderMaterial.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		ShaderMaterial::ShaderMaterial( const ZED_CHAR8 *p_pMaterialName ) :
			Material::Material( p_pMaterialName )
		{
		}

		ShaderMaterial::~ShaderMaterial( )
		{
		}
	}
}

