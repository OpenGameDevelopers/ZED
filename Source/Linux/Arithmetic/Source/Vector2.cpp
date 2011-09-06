#include <Arithmetic.hpp>
#include <Vector2.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		ZED_FLOAT32 Vector2::Dot( const Vector2 &p_Other ) const
		{
			return ( m_X*p_Other.m_X + m_Y*p_Other.m_Y );
		}

		void Vector2::Normalise( )
		{
			ZED_FLOAT32 Length = m_X*m_X + m_Y*m_Y;

			if( ZED::Arithmetic::IsZero( Length ) )
			{
				zedAssert( "Vector2::Normalise( ) Magnitude is zero\n" );
			}
			else
			{
				ZED_FLOAT32 Factor = 1.0f / Length;

				m_X *= Factor;
				m_Y *= Factor;
			}
		}

		void Vector2::Clean( )
		{
			if( ZED::Arithmetic::IsZero( m_X ) )
			{
				m_X = 0.0f;
			}
			if( ZED::Arithmetic::IsZero( m_Y ) )
			{
				m_Y = 0.0f;
			}
		}

		ZED_BOOL Vector2::IsZero( ) const
		{
			if( ZED::Arithmetic::IsZero( m_X ) &&
				ZED::Arithmetic::IsZero( m_Y ) )
			{
				return ZED_TRUE;
			}
			else
			{
				return ZED_FALSE;
			}
		}

		ZED_FLOAT32 Vector2::Magnitude( ) const
		{
			return Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y );
		}

		ZED_FLOAT32 Vector2::MagnitudeSq( ) const
		{
			return ( m_X*m_X + m_Y*m_Y );
		}

		ZED_FLOAT32 Vector2::Distance( const Vector2 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;

			return Arithmetic::InvSquareRoot( X*X + Y*Y );
		}

		ZED_FLOAT32 Vector2::DistanceSq( const Vector2 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32	Y = m_Y - p_Other.m_Y;

			return ( X*X + Y*Y );
		}

		ZED_FLOAT32 Vector2::PerpendicularDot( const Vector2 &p_Other ) const
		{
			return ( ( m_X*p_Other.m_Y ) - ( m_Y*p_Other.m_X ) );
		}

		ZED_FLOAT32 PerpendicularDot( const Vector2 &p_Vec1,
			const Vector2 &p_Vec2 )
		{
			return ( ( p_Vec1.m_X*p_Vec2.m_Y ) - ( p_Vec1.m_Y*p_Vec2.m_X ) );
		}

		System::Writer &operator<<( System::Writer &p_Out,
			const Vector2 &p_Source )
		{
			p_Out << "< " << p_Source[ 0 ] << " " << p_Source[ 1 ] << " >" <<
				std::endl;

			return p_Out;
		}

		ZED_BOOL Vector2::operator==( const Vector2 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				Arithmetic::Equal( m_Y, p_Other[ 1 ] ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		ZED_BOOL Vector2::operator!=( const Vector2 &p_Other ) const
		{
			if( Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				Arithmetic::Equal( m_Y, p_Other[ 1 ] ) )
			{
				return ZED_FALSE;
			}

			return ZED_TRUE;
		}

		Vector2 Vector2::operator+( const Vector2 &p_Other ) const
		{
			return Vector2( m_X+p_Other[ 0 ], m_Y+p_Other[ 1 ] );
		}

		Vector2 Vector2::operator-( const Vector2 &p_Other ) const
		{
			return Vector2( m_X-p_Other[ 0 ], m_Y-p_Other[ 1 ] );
		}

		Vector2 Vector2::operator*( const Vector2 &p_Other ) const
		{
			return Vector2( m_X*p_Other[ 0 ], m_Y-p_Other[ 1 ] );
		}

		Vector2 Vector2::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector2( m_X*p_Scalar, m_Y*p_Scalar );
		}

		Vector2 operator*( const ZED_FLOAT32 p_Scalar, const Vector2 &p_Vec )
		{
			return Vector2( p_Scalar*p_Vec[ 0 ], p_Scalar*p_Vec[ 1 ] );
		}

		Vector2 Vector2::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			// No divide by zero check!
			return Vector2( m_X/p_Scalar, m_Y/p_Scalar );
		}

		Vector2 &Vector2::operator+=( const Vector2 &p_Other )
		{
			m_X += p_Other[ 0 ];
			m_Y += p_Other[ 1 ];

			return *this;
		}

		Vector2 &Vector2::operator-=( const Vector2 &p_Other )
		{
			m_X -= p_Other[ 0 ];
			m_Y -= p_Other[ 1 ];

			return *this;
		}

		Vector2 &Vector2::operator*=( const Vector2 &p_Other )
		{
			m_X *= p_Other[ 0 ];
			m_Y *= p_Other[ 1 ];

			return *this;
		}

		Vector2 &Vector2::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			m_X *= p_Scalar;
			m_Y *= p_Scalar;

			return *this;
		}

		Vector2 &operator*=( const ZED_FLOAT32 p_Scalar,
			Vector2 &p_Self )
		{
			p_Self[ 0 ] *= p_Scalar;
			p_Self[ 1 ] *= p_Scalar;

			return p_Self;
		}

		Vector2 &Vector2::operator/=( const ZED_FLOAT32 p_Scalar )
		{
			m_X /= p_Scalar;
			m_Y /= p_Scalar;

			return *this;
		}
	}
}

