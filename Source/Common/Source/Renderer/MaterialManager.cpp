#include <Renderer/MaterialManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		MaterialManager::MaterialManager( )
		{
		}

		MaterialManager::~MaterialManager( )
		{
		}

		ZED_UINT32 MaterialManager::AddMaterial(
			Material * const &p_pMaterial )
		{
			return ZED_OK;
		}

		ZED_UINT32 MaterialManager::GetMaterial( const ZED_UINT32 p_MaterialID,
			Material *p_pMaterial ) const
		{
			return ZED_OK;
		}

		ZED_UINT32 MaterialManager::GetMaterial(
			const ZED_CHAR8 *p_pMaterialName, Material *p_pMaterial ) const
		{
			return ZED_OK;
		}
	}
}

