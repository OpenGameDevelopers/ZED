#ifndef __ZED_SYSTEM_RESOURCE_HPP__
#define __ZED_SYSTEM_RESOURCE_HPP__

#include <DataTypes.hpp>
#include <map>
#include <list>

namespace ZED
{
	namespace System
	{
		class ResourceCache;
		class ResourceContainer;

		// Resource is used as a data fragment, rather than something which
		// holds the data itself (such as the ResourceHandle)
		typedef struct __ZED_Resource
		{
			ZED_MEMSIZE ID;
			ZED_MEMSIZE Size;
		}Resource,*LPResource;

		// ResourceHandle contains the data to use, given a valid Resource
		class ResourceHandle
		{
		friend ResourceCache;
		protected:
			ZED_BYTE	*m_pData;
			Resource	m_Resource;
		public:
			ResourceHandle( Resource p_Resource, ZED_BYTE *m_pData );
			~ResourceHandle( );
		};

		// Typedef a map and list for resources
		typedef std::map< ZED_MEMSIZE, ResourceHandle * > ResourceMap;
		typedef std::list< ResourceHandle * > ResourceList;

		class ResourceCache
		{
		public:
			ResourceCache( const ZED_MEMSIZE p_Size,
				ResourceContainer *p_pResContainer );
			
			ZED_UINT32 Initialise( );
			ZED_BOOL Create( Resource &p_Resource );
			ZED_BYTE *Get( const Resource &p_Resource );
			void Flush( );

		protected:
			ResourceList		m_LRU;
			ResourceMap			m_Map;

			ResourceContainer	*m_pResource;

			ZED_MEMSIZE			m_Allocated;
			ZED_MEMSIZE			m_TotalSize;

			ZED_UINT32 CreateSpace( ZED_MEMSIZE p_Size );
			ZED_BYTE *Allocate( ZED_MEMSIZE p_Size );
			void Free( ResourceHandle *p_pHandle );
			void FreeLastResource( );
			ZED_BYTE *Load( const Resource &p_Resource );
			ResourceHandle *Find( const Resource &p_Resource );
			ZED_BYTE *Refresh( ResourceHandle *p_pHandle );
		};

		// ResourceContainer is to be inherited from, so caches for different
		// resource files (i.e. PKG/ZIP/TAR) can be used.
		class ResourceContainer
		{
		public:
			virtual ~ResourceContainer( );

			virtual ZED_UINT32 Open( )=0;
			virtual ZED_MEMSIZE GetSize( const Resource &p_Resource )=0;
			virtual ZED_BYTE *GetData( const Resource &p_Resource )=0;
		};
	}
}

#endif
