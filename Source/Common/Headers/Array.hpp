#ifndef __ZED_SYSTEM_ARRAY_HPP__
#define __ZED_SYSTEM_ARRAY_HPP__

#include <DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		template< class T >
		class Array
		{
		public:
			// Constructors & Destructor
			Array( ZED_UINT32 p_Capacity, ZED_UINT32 p_Growth );
			~Array( );

			ZED_UINT32 GetCapacity( ) const;
			ZED_UINT32 GetCount( ) const;

			T* GetObjects( );
			const T* GetObjects( ) const;

			void Append( const T &p_Object );
			void Set( const ZED_UINT32 p_Index, const T &p_Object );
			void Remove( const ZED_UINT32 p_Index );
			void RemoveAll( );

			void SetCapacity( const ZED_UINT32 p_MaxCapacity );
			void SetGrowth( const ZED_UINT32 p_Growth );
			ZED_UINT32 GetGrowth( ) const;

			// Operators
			T &operator[ ]( const ZED_UINT32 p_Index );
			const T &operator[ ]( const ZED_UINT32 p_Index ) const;

		private:
			ZED_UINT32	m_Capacity;
			ZED_UINT32	m_Count;
			ZED_UINT32	m_Growth;
			T			*m_pArray;
		};
	}
}

#endif
