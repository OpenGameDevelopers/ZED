#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		/*Vector3::Vector3( const Vector3 &p_Copy )
		{
			m_X = p_Copy.m_X;
			m_Y = p_Copy.m_Y;
			m_Z = p_Copy.m_Z;
		}*/

		// SIMD
		void Vector3::Normalise( )
		{
			ZED_FLOAT32 Squared = m_X*m_X + m_Y*m_Y + m_Z*m_Z;

			// Check if the values added together are not zero
			if( ZED::Arithmetic::IsZero( Squared ) )
			{
				// Zero the vector
				Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor = ZED::Arithmetic::InvSquareRoot( Squared );
				m_X *= Factor;
                m_Y *= Factor;
				m_Z *= Factor;
			}
		}

		ZED_FLOAT32 Vector3::Magnitude( ) const
		{
			return ZED::Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3::MagnitudeSq( ) const
		{
			return m_X*m_X + m_Y*m_Y + m_Z*m_Z;
		}

		ZED_FLOAT32 Vector3::Distance( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;

			return( ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z ) );
		}

		ZED_FLOAT32 Vector3::DistanceSq( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;

			return ( X*X + Y*Y + Z*Z );
		}

		void Vector3::Clean( ) 
		{
			if( ZED::Arithmetic::IsZero( m_X ) )
			{
				m_X = 0.0f;
			}
			if( ZED::Arithmetic::IsZero( m_Y ) )
			{
				m_Y = 0.0f;
			}
			if( ZED::Arithmetic::IsZero( m_Z ) )
			{
				m_Z = 0.0f;
			}
		}

		ZED_FLOAT32 Vector3::Dot( const Vector3 &p_Other ) const
		{
			return m_X*p_Other.m_X + m_Y*p_Other.m_Y + m_Z*p_Other.m_Z;
		}

		Vector3 Vector3::Cross( const Vector3 &p_Other ) const
		{
			return Vector3(
				( m_Y*p_Other.m_Z - m_Z*p_Other.m_Y ),
				( m_Z*p_Other.m_X - m_X*p_Other.m_Z ),
				( m_X*p_Other.m_Y - m_Y*p_Other.m_Z ) );
		}

		ZED::System::Writer &operator<<( ZED::System::Writer &p_Out,
			const Vector3 &p_Source )
		{
			p_Out << "Vector3 [ " << p_Source[ 0 ] << ", " << p_Source[ 1 ]
				<< ", " << p_Source[ 2 ] << " ]" << std::endl;
			return p_Out;
		}

		ZED_BOOL Vector3::operator==( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( p_Other.m_X, m_X ) &&
				ZED::Arithmetic::Equal( p_Other.m_Y, m_Y ) &&
				ZED::Arithmetic::Equal( p_Other.m_Z, m_Z ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		ZED_BOOL Vector3::operator!=( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( p_Other.m_X, m_X ) &&
				ZED::Arithmetic::Equal( p_Other.m_Y, m_Y ) &&
				ZED::Arithmetic::Equal( p_Other.m_Z, m_Z ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
		}

		/*Vector3 &Vector3::operator=( const Vector3 &p_Vec3 )
		{
			m_X = p_Vec3.m_X;
			m_Y = p_Vec3.m_Y;
			m_Z = p_Vec3.m_Z;

			return *this;
		}*/

		Vector3 Vector3::operator+( const Vector3 &p_Other ) const
		{
			return Vector3( m_X+p_Other.m_X, m_Y+p_Other.m_Y,
				m_Z+p_Other.m_Z );
		}

		Vector3 Vector3::operator-( const Vector3 &p_Other ) const
		{
			return Vector3( m_X-p_Other.m_X, m_Y-p_Other.m_Y,
				m_Z-p_Other.m_Z );
		}

		Vector3 Vector3::operator*( const Vector3 &p_Other ) const
		{
			return Vector3( m_X*p_Other.m_X, m_Y*p_Other.m_Y,
				m_Z*p_Other.m_Z );
		}

		Vector3 Vector3::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X*p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar );
		}

		Vector3 operator*( const ZED_FLOAT32 p_Scalar, const Vector3 &p_Self )
		{
			return Vector3( p_Self.m_X*p_Scalar, p_Self.m_Y*p_Scalar,
				p_Self.m_Z*p_Scalar );
		}

		Vector3 Vector3::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X*p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar );
		}

		Vector3 &Vector3::operator+=( const Vector3 &p_Other )
		{
			m_X += p_Other.m_X;
			m_Y += p_Other.m_Y;
			m_Z += p_Other.m_Z;

			return *this;
		}

		Vector3 &Vector3::operator-=( const Vector3 &p_Other )
		{
			m_X -= p_Other.m_X;
			m_Y -= p_Other.m_Y;
			m_Z -= p_Other.m_Z;

			return *this;
		}

		Vector3 &Vector3::operator*=( const Vector3 &p_Other ) 
		{
			m_X *= p_Other.m_X;
			m_Y *= p_Other.m_Y;
			m_Z *= p_Other.m_Z;

			return *this;
		}

		Vector3 &Vector3::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			m_X *= p_Scalar;
			m_Y *= p_Scalar;
			m_Z *= p_Scalar;

			return *this;
		}

		Vector3 &operator*=( const ZED_FLOAT32 p_Scalar, Vector3 &p_Self )
		{
			p_Self.m_X *= p_Scalar;
			p_Self.m_Y *= p_Scalar;
			p_Self.m_Z *= p_Scalar;

			return p_Self;
		}

		Vector3 &Vector3::operator/=( const ZED_FLOAT32 p_Scalar )
		{
			m_X /= p_Scalar;
			m_Y /= p_Scalar;
			m_Z /= p_Scalar;

			return *this;
		}

		Vector3 Add( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			return Vector3( p_Vec1[ 0 ]+ p_Vec2[ 0 ], p_Vec1[ 1 ]+p_Vec2[ 1 ],
				p_Vec1[ 2 ]+p_Vec2[ 2 ] );
		}

		Vector3 Sub( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			return Vector3( p_Vec1[ 0 ]-p_Vec2[ 0 ], p_Vec1[ 1 ]-p_Vec2[ 1 ],
				p_Vec1[ 2 ]-p_Vec2[ 2 ] );
		}

		Vector3 Mul( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			return Vector3( p_Vec1[ 0 ]*p_Vec2[ 0 ], p_Vec1[ 1 ]*p_Vec2[ 1 ],
				p_Vec1[ 2 ]*p_Vec2[ 2 ] );
		}

		Vector3 Mul( const Vector3 &p_Vec, const ZED_FLOAT32 p_Scalar )
		{
			return Vector3( p_Vec[ 0 ]*p_Scalar, p_Vec[ 1 ]*p_Scalar,
				p_Vec[ 2 ]*p_Scalar );
		}

		Vector3 Div( const Vector3 &p_Vec, const ZED_FLOAT32 p_Scalar )
		{
			return Vector3( p_Vec[ 0 ]/p_Scalar, p_Vec[ 1 ]/p_Scalar,
				p_Vec[ 2 ]/p_Scalar );
		}

		ZED_FLOAT32 Distance( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			ZED_FLOAT32 X = p_Vec1[ 0 ] - p_Vec2[ 0 ];
			ZED_FLOAT32 Y = p_Vec1[ 1 ] - p_Vec2[ 1 ];
			ZED_FLOAT32 Z = p_Vec2[ 2 ] - p_Vec2[ 2 ];

			return( ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z ) );
		}

		ZED_FLOAT32 DistanceSq( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			ZED_FLOAT32 X = p_Vec1[ 0 ] - p_Vec2[ 0 ];
			ZED_FLOAT32 Y = p_Vec1[ 1 ] - p_Vec2[ 1 ];
			ZED_FLOAT32 Z = p_Vec2[ 2 ] - p_Vec2[ 2 ];

			return( X*X + Y*Y + Z*Z );
		}
	}
}
