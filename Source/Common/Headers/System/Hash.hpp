#ifndef __ZED_SYSTEM_HASH_HPP__
#define __ZED_SYSTEM_HASH_HPP__

#include <DataTypes.hpp>
#include <Array.hpp>
#include <List.hpp>

namespace ZED
{
	namespace System
	{
		// Think of this as a struct, hence why it's all public
		template< class TKEY, class TVALUE >
		class HASH
		{
		public:
			TKEY	Key;
			TVALUE	Value;
		};

		template< class TKEY, class TVALUE >
		class HashTable
		{
		public:
			HashTable( );
			~HashTable( );
			// Saves typing more than needed
			typedef HASH< TKEY, TVALUE > Entry;

			ZED_UINT32 ( *m_Hash )( TKEY );

		private:
			ZED_UINT32 m_Size;
			ZED_UINT32 m_Count;
			Array< LinkedList< Entry > > m_Table;
		};
	}
}

#endif

