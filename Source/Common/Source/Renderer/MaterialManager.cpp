#include <Renderer/MaterialManager.hpp>
#include <Renderer/Material.hpp>
#include <System/Memory.hpp>
#include <System/Debugger.hpp>

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
			MaterialIDMap::const_iterator IDIterator =
				m_MaterialIDMap.find( ID );

			if( IDIterator->first != 0 )
			{
				zedTrace( "[ZED::Renderer::MaterialManager::AddMaterial] "
					"<ERROR> Material's ID is zero\n" );

				return ZED_OK;
			}

			MaterialIDInsertResult IDResult;

			IDResult = m_MaterialIDMap.insert(
				std::pair< ZED_UINT32, Material * >( ID, p_pMaterial ) );

			if( !IDResult.second )
			{
				zedTrace( "[ZED::Renderer::MaterialManager::AddMaterial] "
					"<ERROR> Unable to insert the material ID to material "
					"mapping [ID: %u | Name: %s | Pointer: 0x%08X]\n",
					p_pMaterial->GetID( ), p_pMaterial->GetName( ),
					p_pMaterial );

				return ZED_FAIL;
			}

			MaterialNameMap::const_iterator NameIterator =
				m_MaterialNameMap.find( p_pMaterial->GetName( ) );

			// Name collision!
			if( NameIterator->first != ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::MaterialManager::AddMaterial] "
					"<ERROR> There is a material name collision\n" );
				zedTrace( "\tMaterial in name map [ID: %u | Name: %s | "
					"Pointer: 0x%08X]\n", NameIterator->second->GetID( ),
					NameIterator->second->GetName( ), NameIterator->second );
				zedTrace( "\tMaterial trying to be inserted [ID: %u | "
					"Name: %s | Pointer: 0x%08X]\n", p_pMaterial->GetID( ),
					p_pMaterial->GetName( ), p_pMaterial );

				return ZED_FAIL;
			}

			MaterialNameInsertResult NameResult;

			NameResult = m_MaterialNameMap.insert(
				std::pair< ZED_CHAR8 *, Material * >(
					p_pMaterial->GetName( ), p_pMaterial ) );

			if( !NameResult.second )
			{
				zedTrace( "[ZED::Renderer::MaterialManager::AddMaterial] "
					"<ERROR> Failed to insert the material name to material "
					"mapping [ID: %u | Name: %s | Pointer: 0x%08X]\n",
					p_pMaterial->GetID( ), p_pMaterial->GetName( ),
					p_pMaterial );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 MaterialManager::GetMaterial( const ZED_UINT32 p_MaterialID,
			Material *p_pMaterial ) const
		{
			MaterialIDMap::const_iterator IDIterator = m_MaterialIDMap.find(
				p_MaterialID );

			if( IDIterator->first != 0 )
			{
				p_pMaterial = IDIterator->second;

				return ZED_OK;
			}

			zedTrace( "[ZED::Renderer::MaterialManager::GetMaterial] <ERROR> "
				"Unable to locate material by ID: %d\n", p_MaterialID );

			return ZED_FAIL;
		}

		ZED_UINT32 MaterialManager::GetMaterial(
			ZED_CHAR8 * const &p_pMaterialName, Material *p_pMaterial ) const
		{
			MaterialNameMap::const_iterator NameIterator =
				m_MaterialNameMap.find( p_pMaterialName );

			if( NameIterator->first != ZED_NULL )
			{
				p_pMaterial = NameIterator->second;

				return ZED_OK;
			}

			zedTrace( "[ZED::Renderer::MaterialManager::GetMaterial] <ERROR> "
				"Unable to locate material by name: %s\n", p_pMaterialName );

			return ZED_FAIL;
		}
	}
}

