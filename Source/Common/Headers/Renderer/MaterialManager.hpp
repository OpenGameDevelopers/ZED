#ifndef __ZED_RENDERER_MATERIALMANAGER_HPP__
#define __ZED_RENDERER_MATERIALMANAGER_HPP__

#include <System/DataTypes.hpp>
#include <map>

namespace ZED
{
	namespace Renderer
	{
		class Material;

		class MaterialManager
		{
		public:
			MaterialManager( );
			~MaterialManager( );

			ZED_UINT32 AddMaterial( const Material *p_pMaterial );

			ZED_UINT32 GetMaterial( const ZED_UINT32 p_MaterialID,
				Material *p_pMaterial ) const;
			ZED_UINT32 GetMaterial( const ZED_CHAR8 *p_pMaterialName,
				Material *p_pMaterial ) const;

		private:
			std::map< ZED_UINT32, Material * >	m_MaterialIDs;
			std::map< ZED_CHAR8 *, Material * >	m_MaterialNames;
		};
	}
}

#endif

