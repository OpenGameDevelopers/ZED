#include <Arithmetic/Vector3.hpp>
#include <Arithmetic/Matrix3x3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Vector3::Vector3( const Vector3 &p_Other )
		{
			m_X = p_Other.m_X;
			m_Y = p_Other.m_Y;
			m_Z = p_Other.m_Z;
		}

		Vector3 &Vector3::operator=( const Vector3 &p_Other )
		{
			m_X = p_Other.m_X;
			m_Y = p_Other.m_Y;
			m_Z = p_Other.m_Z;
		}

		void Vector3::Normalise( )
		{
			ZED_FLOAT32 All = ( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
			
			if( ZED::Arithmetic::IsZero( All ) )
			{
				Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor = ZED::Arithmetic::InvSquareRoot( All );
				m_X *= Factor;
				m_Y *= Factor;
				m_Z *= Factor;
			}
		}

		ZED_FLOAT32 Vector3::Magnitude( ) const
		{
			return Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3::MagnitudeSq( ) const
		{
			return ( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3::Distance( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];

			return Arithmetic::SquareRoot( X*X + Y*Y + Z*Z );
		}

		ZED_FLOAT32 Vector3::DistanceSq( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];

			return ( X*X + Y*Y + Z*Z );
		}

		void Vector3::Clean( )
		{
			if( Arithmetic::IsZero( m_X ) )
			{
				m_X = 0.0f;
			}
			if( Arithmetic::IsZero( m_Y ) )
			{
				m_Y = 0.0f;
			}
			if( Arithmetic::IsZero( m_Z ) )
			{
				m_Z = 0.0f;
			}
		}

		ZED_FLOAT32 Vector3::Dot( const Vector3 &p_Other ) const
		{
			return ( m_X*p_Other[ 0 ] + m_Y*p_Other[ 1 ] + m_Z*p_Other[ 2 ] );
		}

		Vector3 Vector3::Cross( const Vector3 &p_Other ) const
		{
			return Vector3( ( m_Y*p_Other[ 2 ] - m_Z*p_Other[ 1 ] ),
							( m_Z*p_Other[ 0 ] - m_X*p_Other[ 2 ] ),
							( m_X*p_Other[ 1 ] - m_Y*p_Other[ 0 ] ) );
		}

		System::Writer &operator<<( System::Writer &p_Out,
			const Vector3 &p_Source )
		{
			p_Out << "< " << p_Source[ 0 ] << " "<< p_Source[ 1 ] << " "
				<< p_Source[ 2 ] << " >" << std::endl;
			return p_Out;
		}

		ZED_BOOL Vector3::operator==( const Vector3 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				Arithmetic::Equal( m_Z, p_Other[ 2 ] ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		ZED_BOOL Vector3::operator!=( const Vector3 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				Arithmetic::Equal( m_Z, p_Other[ 2 ] ) )
			{
				return ZED_FALSE;
			}

			return ZED_TRUE;
		}

		Vector3 Vector3::operator-( ) const
		{
			return Vector3( -m_X, -m_Y, -m_Z );
		}

		Vector3 Vector3::operator+( const Vector3 &p_Other ) const
		{
			return Vector3( m_X+p_Other[ 0 ], m_Y+p_Other[ 1 ],
				m_Z+p_Other[ 2 ] );
		}

		Vector3 Vector3::operator-( const Vector3 &p_Other ) const
		{
			return Vector3( m_X-p_Other[ 0 ], m_Y-p_Other[ 1 ],
				m_Z-p_Other[ 2 ] );
		}

		Vector3 Vector3::operator*( const Vector3 &p_Other ) const
		{
			return Vector3( m_X*p_Other[ 0 ], m_Y*p_Other[ 1 ],
				m_Z*p_Other[ 2 ] );
		}

		Vector3 Vector3::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X*p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar );
		}

		Vector3 Vector3::operator*( const Matrix3x3 &p_Matrix ) const
		{
			Vector3 Mul;

			Mul.m_X =	m_X * p_Matrix.m_M[ 0 ] +
						m_Y * p_Matrix.m_M[ 1 ] +
						m_Z * p_Matrix.m_M[ 2 ];
			Mul.m_Y =	m_X * p_Matrix.m_M[ 3 ] +
						m_Y * p_Matrix.m_M[ 4 ] +
						m_Z * p_Matrix.m_M[ 5 ];
			Mul.m_Z =	m_X * p_Matrix.m_M[ 6 ] +
						m_Y * p_Matrix.m_M[ 7 ] +
						m_Z * p_Matrix.m_M[ 8 ];

			return Mul;
		}

		Vector3 operator*( const ZED_FLOAT32 p_Scalar, const Vector3 &p_Self )
		{
			return Vector3( p_Scalar*p_Self[ 0 ], p_Scalar*p_Self[ 1 ],
				p_Scalar*p_Self[ 2 ] );
		}

		Vector3 Vector3::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			// No zero checking!
			return Vector3( m_X/p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar );
		}

		Vector3 &Vector3::operator+=( const Vector3 &p_Other )
		{
			m_X += p_Other[ 0 ];
			m_Y += p_Other[ 1 ];
			m_Z += p_Other[ 2 ];

			return *this;
		}

		Vector3 &Vector3::operator-=( const Vector3 &p_Other )
		{
			m_X -= p_Other[ 0 ];
			m_Y -= p_Other[ 1 ];
			m_Z -= p_Other[ 2 ];

			return *this;
		}

		Vector3 &Vector3::operator*=( const Vector3 &p_Other )
		{
			m_X *= p_Other[ 0 ];
			m_Y *= p_Other[ 1 ];
			m_Z *= p_Other[ 2 ];

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
			p_Self[ 0 ] *= p_Scalar;
			p_Self[ 1 ] *= p_Scalar;
			p_Self[ 2 ] *= p_Scalar;

			return p_Self;
		}

		Vector3 &Vector3::operator/=( const ZED_FLOAT32 p_Scalar )
		{
			// No divide by zero check!
			m_X /= p_Scalar;
			m_Y /= p_Scalar;
			m_Z /= p_Scalar;

			return *this;
		}
	}
}

