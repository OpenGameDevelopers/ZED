#ifndef __ZED_SYSTEM_RESOURCE_HPP__
#define __ZED_SYSTEM_RESOURCE_HPP__

#include <System/DataTypes.hpp>
#include <map>
#include <list>
#include <vector>

namespace ZED
{
	namespace System
	{
		class ResourceCache;
		class ResourceContainer;
		class File;

		typedef struct __tagRESOURCE
		{
			ZED_UINT64	Offset;
			ZED_UINT64	Size;
		}RESOURCE,*PRESOURCE;

		// ResourceHandle contains the data to use, given a valid resource
		class ResourceHandle
		{
		public:
			ResourceHandle( ResourceContainer *p_pResourceContainer );
			~ResourceHandle( );

			ZED_UINT32 GetData( void **p_ppData );

			void SetThreshold( const ZED_UINT8 p_Threshold );

		protected:
			ZED_BYTE		*m_pData;
			ZED_CHAR8		*m_pFileName;
			ResourceCache	*m_pResourceCache;
			ZED_UINT8		m_Threshold;
		};

		class ResourceCache
		{
		public:
			ResourceCache( const ZED_MEMSIZE p_Size,
				ResourceContainer *p_pResourceContainer );
			
			ZED_UINT32 Initialise( );
			void Flush( );

		protected:
			std::vector< ResourceContainer * >	m_ContainerArray;

			// There needs to be a pointer to the in-use data here, whether or
			// not a custom memory manager for the resource system is used or
			// one is created for managing all memory is to be decided
		};

		// ResourceContainer is to be inherited from, so caches for different
		// resource files (i.e. PKG/ZIP/TAR) can be used.
		class ResourceContainer
		{
		public:
			virtual ~ResourceContainer( );

			virtual ZED_UINT32 Open( const ZED_CHAR8 *p_pContainerRoot ) = 0;

			virtual void Rescan( const ZED_BOOL p_PurgeOldData ) = 0;

		protected:
			std::map< ZED_CHAR8 *, RESOURCE >	m_ResourceMap;
		};

		class ResourceManager
		{
		public:
			ResourceManager( );
			~ResourceManager( );

			ZED_UINT32 OpenContainer( const ZED_CHAR8 *p_pContainerRoot );

			ZED_UINT32 OpenResource( const ZED_CHAR8 *p_pFileName,
				ResourceHandle *p_pResource );
			
			ZED_UINT32 CloseResource( ResourceHandle *p_pResource );

		private:
			std::vector< ResourceCache * >	m_Cache;
		};
	}
}

#endif
