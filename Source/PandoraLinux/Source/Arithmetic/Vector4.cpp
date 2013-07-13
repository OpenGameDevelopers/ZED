#include <Arithmetic/Arithmetic.hpp>
#include <Arithmetic/Vector4.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <cstring>

namespace ZED
{
	namespace Arithmetic
	{
		Vector4::Vector4( const Vector4 &p_Other ) :
			m_X( p_Other.m_X ),
			m_Y( p_Other.m_Y ),
			m_Z( p_Other.m_Z ),
			m_W( p_Other.m_W )
		{
		}

		Vector4 &Vector4::operator=( const Vector4 &p_Other )
		{
			memcpy( &m_X, &p_Other.m_X, sizeof( ZED_FLOAT32 )*4 );

			return *this;
		}

		void Vector4::Normalise( )
		{
			ZED_FLOAT32 Length = m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W;

			if( Arithmetic::IsZero( Length ) )
			{
				return;
			}
			else
			{
				ZED_FLOAT32 Factor = Arithmetic::InvSquareRoot( Length );

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
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;
			ZED_FLOAT32 W = m_W - p_Other.m_W;

			return Arithmetic::InvSquareRoot( X*X + Y*Y + Z*Z  + W*W);
		}

		ZED_FLOAT32 Vector4::DistanceSq( const Vector4 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;
			ZED_FLOAT32 W = m_W - p_Other.m_W;

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
			return (	m_X*p_Other.m_X + m_Y*p_Other.m_Y +
						m_Z*p_Other.m_Z + m_W*p_Other.m_W );
		}

		ZED_BOOL Vector4::operator==( const Vector4 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other.m_X ) &&
				Arithmetic::Equal( m_Y, p_Other.m_Y ) &&
				Arithmetic::Equal( m_Z, p_Other.m_Z ) &&
				Arithmetic::Equal( m_W, p_Other.m_W ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		ZED_BOOL Vector4::operator!=( const Vector4 &p_Other ) const
		{
			if( !Arithmetic::Equal( m_X, p_Other.m_X ) ||
				!Arithmetic::Equal( m_Y, p_Other.m_Y ) ||
				!Arithmetic::Equal( m_Z, p_Other.m_Z ) ||
				!Arithmetic::Equal( m_W, p_Other.m_W ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		Vector4 Vector4::operator+( const Vector4 &p_Other ) const
		{
			return Vector4( m_X+p_Other.m_X, m_Y+p_Other.m_Y, m_Z+p_Other.m_Z,
				m_W+p_Other.m_W );
		}

		Vector4 Vector4::operator-( const Vector4 &p_Other ) const
		{
			return Vector4( m_X-p_Other.m_X, m_Y-p_Other.m_Y, m_Z-p_Other.m_Z,
				m_W-p_Other.m_W );
		}

		Vector4 Vector4::operator*( const Vector4 &p_Other ) const
		{
			return Vector4( m_X*p_Other.m_X, m_Y*p_Other.m_Y, m_Z*p_Other.m_Z,
				m_W*p_Other.m_W );
		}

		Vector4 Vector4::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector4( m_X*p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar,
				m_W*p_Scalar );
		}

		Vector4 Vector4::operator*( const Matrix4x4 &p_Matrix ) const
		{	
			Vector4 Vector;
			
			Vector.m_X =	m_X*p_Matrix[ 0 ] + m_Y*p_Matrix[ 1 ] +
							m_Z*p_Matrix[ 2 ] + m_W*p_Matrix[ 3 ];
			Vector.m_Y =	m_X*p_Matrix[ 4 ] + m_Y*p_Matrix[ 5 ] +
							m_Z*p_Matrix[ 6 ] + m_W*p_Matrix[ 7 ];
			Vector.m_Z =	m_X*p_Matrix[ 8 ] + m_Y*p_Matrix[ 9 ] +
							m_Z*p_Matrix[ 10 ] + m_W*p_Matrix[ 11 ];
			Vector.m_W =	m_X*p_Matrix[ 12 ] + m_Y*p_Matrix[ 13 ] +
							m_Z*p_Matrix[ 14 ] + m_W*p_Matrix[ 15 ];

			return Vector;
		}

		Vector4 Vector4::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector4( m_X/p_Scalar, m_Y/p_Scalar, m_Z/p_Scalar,
				m_W/p_Scalar );
		}

		Vector4 &Vector4::operator+=( const Vector4 &p_Other )
		{
			m_X += p_Other.m_X;
			m_Y += p_Other.m_Y;
			m_Z += p_Other.m_Z;
			m_W += p_Other.m_W;

			return *this;
		}

		Vector4 &Vector4::operator-=( const Vector4 &p_Other )
		{
			m_X -= p_Other.m_X;
			m_Y -= p_Other.m_Y;
			m_Z -= p_Other.m_Z;
			m_W -= p_Other.m_W;

			return *this;
		}

		Vector4 &Vector4::operator*=( const Vector4 &p_Other )
		{
			m_X *= p_Other.m_X;
			m_Y *= p_Other.m_Y;
			m_Z *= p_Other.m_Z;
			m_W *= p_Other.m_W;

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

		Vector4 &Vector4::operator/=( const ZED_FLOAT32 p_Scalar )
		{
			m_X /= p_Scalar;
			m_Y /= p_Scalar;
			m_Z /= p_Scalar;
			m_W /= p_Scalar;
			
			return *this;
		}
	}
}

