#include <Renderer/ShaderMaterial.hpp>

namespace ZED
{
	namespace Renderer
	{
		ShaderMaterial::ShaderMaterial( ZED_CHAR8 * const &p_pMaterialName ) :
			Material::Material( p_pMaterialName )
		{
		}

		ShaderMaterial::~ShaderMaterial( )
		{
		}
	}
}

