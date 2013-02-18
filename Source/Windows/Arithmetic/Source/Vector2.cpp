#include <Vector2.hpp>
#include <Arithmetic.hpp>

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
			ZED_FLOAT32 LengthSq = m_X*m_X + m_Y*m_Y;

			if( ZED::Arithmetic::IsZero( LengthSq ) )
			{
				Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor =
					ZED::Arithmetic::InvSquareRoot( LengthSq );
				
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
			return ZED::Arithmetic::IsZero( m_X*m_X + m_Y*m_Y );	
		}

		ZED_FLOAT32 Vector2::Magnitude( ) const
		{
			return ZED::Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y );
		}

		ZED_FLOAT32 Vector2::MagnitudeSq( ) const
		{
			return ( m_X*m_X + m_Y*m_Y );
		}

		ZED_FLOAT32 Vector2::Distance( const Vector2 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;

			return ZED::Arithmetic::InvSquareRoot( X*X + Y*Y );
		}

		ZED_FLOAT32 Vector2::DistanceSq( const Vector2 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;

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

		ZED_BOOL Vector2::operator==( const Vector2 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( p_Other.m_X, m_X ) &&
				ZED::Arithmetic::Equal( p_Other.m_Y, m_Y ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector2::operator!=( const Vector2 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( p_Other.m_X, m_X ) &&
				ZED::Arithmetic::Equal( p_Other.m_Y, m_Y ) )
			{
				return ZED_FALSE;
			}
			return ZED_FALSE;
		}

		Vector2 Vector2::operator+( const Vector2 &p_Other ) const
		{
			return Vector2( m_X+p_Other.m_X, m_Y+p_Other.m_Y );
		}

		Vector2 Vector2::operator-( const Vector2 &p_Other ) const
		{
			return Vector2( m_X-p_Other.m_X, m_Y-p_Other.m_Y );
		}

		Vector2 Vector2::operator*( const Vector2 &p_Other ) const
		{
			return Vector2( m_X*p_Other.m_X, m_Y*p_Other.m_Y );
		}

		Vector2 Vector2::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector2( m_X*p_Scalar, m_Y*p_Scalar );
		}

		Vector2 Vector2::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector2( m_X/p_Scalar, m_Y/p_Scalar );
		}

		Vector2 &Vector2::operator+=( const Vector2 &p_Other )
		{
			m_X += p_Other.m_X;
			m_Y += p_Other.m_Y;

			return *this;
		}

		Vector2 &Vector2::operator-=( const Vector2 &p_Other )
		{
			m_X -= p_Other.m_X;
			m_Y -= p_Other.m_Y;

			return *this;
		}

		Vector2 &Vector2::operator*=( const Vector2 &p_Other )
		{
			m_X *= p_Other.m_X;
			m_Y *= p_Other.m_Y;

			return *this;
		}

		Vector2 &Vector2::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			m_X *= p_Scalar;
			m_Y *= p_Scalar;

			return *this;
		}

		Vector2 &Vector2::operator/=( const ZED_FLOAT32 p_Scalar )
		{
			m_X /= p_Scalar;
			m_Y /= p_Scalar;

			return *this;
		}
	}
}
