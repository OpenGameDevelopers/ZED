#include <Arithmetic/Arithmetic.hpp>
#include <Arithmetic/Vector2.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Vector2::Vector2( const Vector2 &p_Copy ) :
			m_X( p_Copy.m_X ), m_Y( p_Copy.m_Y )
		{
		}

		Vector2 Vector2::operator=( const Vector2 &p_Clone )
		{
			m_X = p_Clone.m_X;
			m_Y = p_Clone.m_Y;
			
			return *this;
		}

		ZED_FLOAT32 Vector2::Dot( const Vector2 &p_Other ) const
		{
			return ( ( m_X*p_Other.m_X )+( m_Y*p_Other.m_Y ) );
		}

		void Vector2::Normalise( )
		{
			ZED_FLOAT32 Mag = ( m_X*m_X )+( m_Y*m_Y );

			// Continue if magnitude is not zero
			if( ZED::Arithmetic::IsZero( Mag ) )
			{
				zedTrace( "[ZED::Arithmetic::Vector2::Normalise] <WARN> "
					"Magnitude is zero\n" );
			}
			else
			{
				ZED_FLOAT32 Factor = 1.0f / Mag;

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

			return ZED_FALSE;
		}

		ZED_FLOAT32 Vector2::Magnitude( ) const
		{
			return ZED::Arithmetic::SquareRoot( ( m_X*m_X )+( m_Y*m_Y ) );
		}

		ZED_FLOAT32 Vector2::MagnitudeSq( ) const
		{
			return ( m_X*m_X )+( m_Y*m_Y );
		}

		ZED_FLOAT32 Vector2::Distance( const Vector2 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;

			return ZED::Arithmetic::InvSquareRoot( ( X*X )+( Y*Y ) );
		}

		ZED_FLOAT32 Vector2::DistanceSq( const Vector2 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;

			return ( X*X )+( Y*Y );
		}

		ZED_FLOAT32 Vector2::PerpendicularDot( const Vector2 &p_Other ) const
		{
			return ( ( m_X*p_Other.m_Y )-( m_Y*p_Other.m_X ) );
		}

		ZED::System::Writer &operator<<( ZED::System::Writer p_Out,
			const Vector2 p_Source )
		{
			p_Out << "< " << p_Source.m_X << " " << p_Source.m_Y << " >" <<
				std::endl;

			return p_Out;
		}

		ZED_BOOL Vector2::operator==( const Vector2 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other.m_X ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector2::operator!=( const Vector2 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other.m_X ) ||
				ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
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
			// No divide-by-zero check!
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

