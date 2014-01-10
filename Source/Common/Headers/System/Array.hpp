#ifndef __ZED_SYSTEM_ARRAY_HPP__
#define __ZED_SYSTEM_ARRAY_HPP__

#include <System/DataTypes.hpp>
#include <System/Debugger.hpp>

namespace ZED
{
	namespace System
	{
		template< class T >
		class Array
		{
		public:
			// Constructors & Destructor

			//--------
			// [Function]
			//	Array constructor
			// [Parameters]
			//	p_Capacity - The initial capacity of the array being created
			//	p_Growth - How much to grow the array each time the capcaity is
			//		exceeded
			// [Description]
			//	Creates an array of size p_Capacity and sets the growth rate
			//	when the array needs to allocate more slots.
			//--------
			Array( ZED_UINT32 p_Capacity, ZED_UINT32 p_Growth )
			{
				if( p_Capacity <= 0 )
				{
					// Nothing to do
					zedTrace( "Error, array could not be created sucessfully\n" );
				}
				else
				{
					m_pArray = new T[ p_Capacity ];
					m_Capacity = p_Capacity;
					m_Growth = p_Growth;
				}
			}

			~Array( )
			{
				if( m_pArray != NULL )
				{
					delete [ ] m_pArray;
				}
				m_pArray = 0;
			}

			ZED_UINT32 GetCapacity( ) const{ return m_Capacity; }
			ZED_UINT32 GetCount( ) const { return m_Count; }

			T* GetObjects( );
			const T* GetObjects( ) const;

			void Append( const T &p_Object );
			void Set( const ZED_UINT32 p_Index, const T &p_Object );
			void Remove( const ZED_UINT32 p_Index );
			void RemoveAll( );

			void SetCapacity( const ZED_UINT32 p_MaxCapacity );
			void SetGrowth( const ZED_UINT32 p_Growth );
			ZED_UINT32 GetGrowth( ) const;

			void Resize( const ZED_UINT32 p_NewCapacity )
			{
				// For Visual Studio 2002 and greater, C++ will throw an exception if the
				// array cannot be allocated
#if _MSC_VER < 1300
				T *pNewArray = new T[ p_NewCapacity ];

				if( pNewArray == 0 )
				{
					return;
				}
#else
				try
				{
					T *pNewArray = new T[ p_NewCapacity ];
				}
				catch( bad_alloc &Bad )
				{
					zedTrace( "Bad allocation caught: %s\n", Bad.what( ); );
				}
#endif
				ZED_UINT32 Min;

				// Make sure that the array isn't oversubscribed!
				if( p_NewCapacity < m_Capacity )
				{
					Min = p_NewCapacity;
				}
				else
				{
					Min = m_Capacity;
				}

				for( ZED_UINT32 i = 0; i < Min; i++ )
				{
					pNewArray[ i ] = m_pArray[ i ];
				}

				m_Capacity  = p_NewCapacity;

				if( m_pArray != NULL )
				{
					delete [ ] m_pArray;
				}

				m_pArray = pNewArray;
			}

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
