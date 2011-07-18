#include <Resource.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace System
	{
		ResourceCache::ResourceCache( const ZED_MEMSIZE p_Size,
			ResourceContainer *p_pResource )
		{
			m_TotalSize = p_Size;
			m_pResource = p_pResource;
			m_Allocated = 0;
		}

		ZED_UINT32 ResourceCache::Initialise( )
		{
			return m_pResource->Open( );
		}

		ZED_BOOL ResourceCache::Create( Resource &p_Resource )
		{
			p_Resource.Size = m_pResource->GetSize( p_Resource );

			return ( p_Resource.Size > 0 );
		}

		ZED_BYTE *ResourceCache::Get( const Resource &p_Resource )
		{
			// if the resource isn't available, load it
			ResourceHandle *pHandle = Find( p_Resource );

			return ( pHandle != ZED_NULL ) ?
				Refresh( pHandle ) : Load( p_Resource );
		}

		void ResourceCache::Flush( )
		{
		}

		ZED_UINT32 ResourceCache::CreateSpace( ZED_MEMSIZE p_Size )
		{
			// Is the space being allocated greater than what the cache
			// actually has available?
			if( p_Size > m_TotalSize )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[ZED::System::ResourceCache::CreateSpace] FAILED: "
					"Tried to allocate %d when %d is available.\n\n",
					p_Size, m_TotalSize );

				return ZED_FAIL;
			}

			while( p_Size > ( m_TotalSize - m_Allocated ) )
			{
				// Couldn't allocate any more!
				if( m_LRU.empty( ) )
				{
					return ZED_FAIL;
				}

				// Get rid of the least recently used resource
				FreeLastResource( );
			}

			return ZED_OK;
		}

		ZED_BYTE *ResourceCache::Allocate( ZED_MEMSIZE p_Size )
		{
			if( CreateSpace( p_Size ) != ZED_OK )
			{
				return ZED_NULL;
			}

			ZED_BYTE *pMemory = new ZED_BYTE[ p_Size ];

			if( pMemory != ZED_NULL )
			{
				m_Allocated += p_Size;
			}
			else
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[ZED::System::ResourceCache::Allocate] FAILED: "
					"Could not allocate %d.\n\n", p_Size );
			}

			return pMemory;
		}

		void ResourceCache::Free( ResourceHandle *p_pHandle )
		{
			// Get rid of the specified resource.  Simple!
			m_LRU.remove( p_pHandle );

			// Also update the map and amount of allocated memory
			m_Map.erase( p_pHandle->m_Resource.ID );
			m_Allocated -= p_pHandle->m_Resource.Size;

			// Delete the resource, for real!
			delete p_pHandle;
		}

		void ResourceCache::FreeLastResource( )
		{
			ResourceList::iterator LRUitr = m_LRU.end( );
			LRUitr--;

			ResourceHandle *pHandle = *LRUitr;

			// Remove this from the map and list
			m_LRU.pop_back( );
			m_Map.erase( pHandle->m_Resource.ID );

			// Update how much space is available
			m_Allocated -= pHandle->m_Resource.Size;

			// Be nice and clean up!
			delete pHandle;
		}

		ZED_BYTE *ResourceCache::Load( const Resource &p_Resource )
		{
			ZED_MEMSIZE Size = m_pResource->GetSize( p_Resource );

			// Try to allocate space
			ZED_BYTE *pData = Allocate( Size );
			if( pData == ZED_NULL )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[ZED::System::ResourceCache::Load] FAILED: "
					"Could not allocate %d.\n\n", Size );
				return ZED_NULL;
			}

			ResourceHandle *pHandle = new ResourceHandle( p_Resource, pData );

			// Add the resource to the cache
			m_LRU.push_front( pHandle );
			m_Map[ p_Resource.ID ] = pHandle;
			pData = m_pResource->GetData( p_Resource );

			// Return the usable data
			return pData;
		}

		ResourceHandle *ResourceCache::Find( const Resource &p_Resource )
		{
			ResourceMap::iterator Itr = m_Map.find( p_Resource.ID );

			if( Itr == m_Map.end( ) )
			{
				return ZED_NULL;
			}

			return ( *Itr ).second;
		}

		ZED_BYTE *ResourceCache::Refresh( ResourceHandle *p_pHandle )
		{
			m_LRU.remove( p_pHandle );
			m_LRU.push_front( p_pHandle );

			return p_pHandle->m_pData;
		}
	}
}
