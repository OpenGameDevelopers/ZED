#include <Renderer/MaterialManager.hpp>
#include <Renderer/Material.hpp>
#include <System/Memory.hpp>

namespace ZED
{
	namespace Renderer
	{
		MaterialManager::MaterialManager( )
		{
		}

		MaterialManager::~MaterialManager( )
		{
			MaterialIDMap::iterator IDIterator = m_MaterialIDMap.begin( );

			while( IDIterator != m_MaterialIDMap.end( ) )
			{
				zedSafeDelete( IDIterator->second );
				++IDIterator;
			}

			MaterialNameMap::iterator NameIterator =
				m_MaterialNameMap.begin( );

			while( NameIterator != m_MaterialNameMap.end( ) )
			{
				zedSafeDelete( NameIterator->second );
				++NameIterator;
			}
		}

		ZED_UINT32 MaterialManager::AddMaterial(
			Material * const &p_pMaterial )
		{
			const ZED_UINT32 ID = p_pMaterial->GetID( );
			MaterialIDMap::iterator IDIterator = m_MaterialIDMap.find( ID );

			if( IDIterator->first != 0 )
			{
				return ZED_OK;
			}

			MaterialIDInsertResult IDResult;

			IDResult = m_MaterialIDMap.insert(
				std::pair< ZED_UINT32, Material * >( ID, p_pMaterial ) );

			if( !IDResult.second )
			{
				return ZED_FAIL;
			}

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

