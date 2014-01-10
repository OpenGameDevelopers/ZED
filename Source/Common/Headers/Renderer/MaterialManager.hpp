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

			ZED_UINT32 AddMaterial( Material * const &p_pMaterial );

			ZED_UINT32 GetMaterial( const ZED_UINT32 p_MaterialID,
				Material *p_pMaterial ) const;
			ZED_UINT32 GetMaterial( ZED_CHAR8 * const &p_pMaterialName,
				Material *p_pMaterial ) const;

		private:
			typedef std::map< ZED_UINT32, Material * > MaterialIDMap;
			typedef std::pair< MaterialIDMap::iterator, bool >
				MaterialIDInsertResult;
			typedef std::map< ZED_CHAR8 *, Material * > MaterialNameMap;
			typedef std::pair< MaterialNameMap::iterator, bool >
				MaterialNameInsertResult;

			MaterialIDMap	m_MaterialIDMap;
			MaterialNameMap	m_MaterialNameMap;
		};
	}
}

#endif

