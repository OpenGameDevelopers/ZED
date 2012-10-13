#include <Matrix3x3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		void Matrix3x3::Identity( )
		{
			m_M[ 0 ] = m_M[ 4 ] = m_M[ 8 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] =
				0.0f;
		}

		ZED_BOOL Matrix3x3::IsZero( ) const
		{
			for( ZED_MEMSIZE i = 0; i < 9; ++i )
			{
				if( !Arithmetic::IsZero( m_M[ i ] ) )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix3x3::IsIdentity( ) const
		{
			if( Arithmetic::Equal( m_M[ 0 ], 1.0f ) &&
				Arithmetic::IsZero( m_M[ 1 ] ) &&
				Arithmetic::IsZero( m_M[ 2 ] ) &&
				Arithmetic::IsZero( m_M[ 3 ] ) &&
				Arithmetic::Equal( m_M[ 4 ], 1.0f ) &&
				Arithmetic::IsZero( m_M[ 5 ] ) &&
				Arithmetic::IsZero( m_M[ 6 ] ) &&
				Arithmetic::IsZero( m_M[ 7 ] ) &&
				Arithmetic::Equal( m_M[ 8 ], 1.0f ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		void Matrix3x3::SetRows( const Vector3 &p_Row1, const Vector3 &p_Row2,
			const Vector3 &p_Row3 )
		{
			m_M[ 0 ] = p_Row1[ 0 ];
			m_M[ 3 ] = p_Row1[ 1 ];
			m_M[ 6 ] = p_Row1[ 2 ];

			m_M[ 1 ] = p_Row2[ 0 ];
			m_M[ 4 ] = p_Row2[ 1 ];
			m_M[ 7 ] = p_Row2[ 2 ];

			m_M[ 2 ] = p_Row3[ 0 ];
			m_M[ 5 ] = p_Row3[ 1 ];
			m_M[ 8 ] = p_Row3[ 2 ];
		}

		void Matrix3x3::GetRows( Vector3 &p_Row1, Vector3 &p_Row2,
			Vector3 &p_Row3 ) const
		{
			p_Row1[ 0 ] = m_M[ 0 ];
			p_Row1[ 1 ] = m_M[ 3 ];
			p_Row1[ 2 ] = m_M[ 6 ];

			p_Row2[ 0 ] = m_M[ 1 ];
			p_Row2[ 1 ] = m_M[ 4 ];
			p_Row2[ 2 ] = m_M[ 7 ];

			p_Row3[ 0 ] = m_M[ 2 ];
			p_Row3[ 1 ] = m_M[ 5 ];
			p_Row3[ 2 ] = m_M[ 8 ];
		}

		Vector3 Matrix3x3::GetRow( const ZED_MEMSIZE p_Index ) const
		{
			// No index range checking!
			return Vector3( m_M[ 0+p_Index ],
							m_M[ 3+p_Index ],
							m_M[ 6+p_Index ] );
		}
	}
}

