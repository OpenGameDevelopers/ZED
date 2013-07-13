#include <Arithmetic/Arithmetic.hpp>
#include <Arithmetic/Vector4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		void Vector4::Normalise( )
		{
			ZED_FLOAT32 LengthSq = m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W;

			if( Arithmetic::IsZero( LengthSq ) )
			{
				Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor = Arithmetic::InvSquareRoot( LengthSq );

				m_X *= Factor;
				m_Y *= Factor;
				m_Z *= Factor;
				m_W *= Factor;
			}
		}

		ZED_FLOAT32 Vector4::Magnitude( ) const
		{
			return Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y + m_Z*m_Z +
				m_W*m_W );
		}

		ZED_FLOAT32 Vector4::MagnitudeSq( ) const
		{
			return ( m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W );
		}

		ZED_FLOAT32 Vector4::Distance( const Vector4 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];
			ZED_FLOAT32 W = m_W - p_Other[ 3 ];

			return Arithmetic::InvSquareRoot( X*X + Y*Y + Z*Z + W*W );
		}

		ZED_FLOAT32 Vector4::DistanceSq( const Vector4 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];
			ZED_FLOAT32 W = m_W - p_Other[ 3 ];

			return ( X*X + Y*Y + Z*Z + W*W );
		}

		void Vector4::Clean( )
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
			if( Arithmetic::IsZero( m_W ) )
			{
				m_W = 0.0f;
			}
		}

		ZED_FLOAT32 Vector4::Dot( const Vector4 &p_Other ) const
		{
			return ( m_X*p_Other[ 0 ] + m_Y*p_Other[ 1 ] + m_Z*p_Other[ 2 ],
				m_W*p_Other[ 3 ] );
		}

		System::Writer &operator<<( System::Writer &p_Out,
			const Vector4 &p_Source )
		{
			p_Out << "< " << p_Source[ 0 ] << " " << p_Source[ 1 ] <<
				" " << p_Source[ 2 ] << " " << p_Source[ 3 ] << " >" <<
				std::endl;
			
			return p_Out;
		}

		ZED_BOOL Vector4::operator==( const Vector4 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				Arithmetic::Equal( m_Z, p_Other[ 2 ] ) &&
				Arithmetic::Equal( m_W, p_Other[ 3 ] ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector4::operator!=( const Vector4 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				Arithmetic::Equal( m_Z, p_Other[ 2 ] ) &&
				Arithmetic::Equal( m_W, p_Other[ 3 ] ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
		}

		Vector4 Vector4::operator+( const Vector4 &p_Other ) const
		{
			return Vector4( m_X+p_Other[ 0 ], m_Y+p_Other[ 1 ],
				m_Z+p_Other[ 2 ], m_W+p_Other[ 3 ] );
		}

		Vector4 Vector4::operator-( const Vector4 &p_Other ) const
		{
			return Vector4( m_X-p_Other[ 0 ], m_Y-p_Other[ 1 ],
				m_Z-p_Other[ 2 ], m_W-p_Other[ 3 ] );
		}

		Vector4 Vector4::operator*( const Vector4 &p_Other ) const
		{
			return Vector4( m_X*p_Other[ 0 ], m_Y*p_Other[ 1 ],
				m_Z*p_Other[ 2 ], m_W*p_Other[ 3 ] );
		}

		Vector4 Vector4::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector4( m_X*p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar,
				m_W*p_Scalar );
		}

		Vector4 operator*( const ZED_FLOAT32 p_Scalar, const Vector4 &p_Self )
		{
			return Vector4( p_Scalar*p_Self[ 0 ], p_Scalar*p_Self[ 1 ],
				p_Scalar*p_Self[ 2 ], p_Scalar*p_Self[ 3 ] );
		}

		Vector4 Vector4::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			// No divide by zero check!
			return Vector4( m_X/p_Scalar, m_Y/p_Scalar, m_Z/p_Scalar,
				m_W/p_Scalar );
		}

		Vector4 &Vector4::operator+=( const Vector4 &p_Other )
		{
			m_X += p_Other[ 0 ];
			m_Y += p_Other[ 1 ];
			m_Z += p_Other[ 2 ];
			m_W += p_Other[ 3 ];
			
			return *this;
		}

		Vector4 &Vector4::operator-=( const Vector4 &p_Other )
		{
			m_X -= p_Other[ 0 ];
			m_Y -= p_Other[ 1 ];
			m_Z -= p_Other[ 2 ];
			m_W -= p_Other[ 3 ];
			
			return *this;
		}

		Vector4 &Vector4::operator*=( const Vector4 &p_Other )
		{
			m_X *= p_Other[ 0 ];
			m_Y *= p_Other[ 1 ];
			m_Z *= p_Other[ 2 ];
			m_W *= p_Other[ 3 ];

			return *this;
		}

		Vector4 &Vector4::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			m_X *= p_Scalar;
			m_Y *= p_Scalar;
			m_Z *= p_Scalar;
			m_W *= p_Scalar;

			return *this;
		}

		Vector4 &operator*=( const ZED_FLOAT32 p_Scalar, Vector4 &p_Self )
		{
			p_Self[ 0 ] *= p_Scalar;
			p_Self[ 1 ] *= p_Scalar;
			p_Self[ 2 ] *= p_Scalar;
			p_Self[ 3 ] *= p_Scalar;
			
			return p_Self;
		}

		Vector4 &Vector4::operator/=( const ZED_FLOAT32 p_Scalar )
		{
			// No divide by zero checking!
			m_X /= p_Scalar;
			m_Y /= p_Scalar;
			m_Z /= p_Scalar;
			m_W /= p_Scalar;

			return *this;
		}
	}
}

