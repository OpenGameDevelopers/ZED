#include <Vector4.hpp>
#include <Arithmetic.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Vector4::Vector4( const Vector4 &p_Copy )
		{
			m_X = p_Copy.m_X;
			m_Y = p_Copy.m_Y;
			m_Z = p_Copy.m_Z;
			m_W = p_Copy.m_W;
		}

		void Vector4::Normalise( )
		{
			ZED_FLOAT32 LengthSq = m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W;

			// Check to make sure the value is workable
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
				m_Z *= Factor;
				m_W *= Factor;
			}
		}

		ZED_FLOAT32 Vector4::Magnitude( ) const
		{
			return( ZED::Arithmetic::SquareRoot(
				m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W ) );
		}

		ZED_FLOAT32 Vector4::MagnitudeSq( ) const
		{
			return( m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W );
		}

		ZED_FLOAT32 Vector4::Distance( const Vector4 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;
			ZED_FLOAT32 W = m_W - p_Other.m_W;

			return ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z + W*W );
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
			if( ZED::Arithmetic::IsZero( m_W ) )
			{
				m_W = 0.0f;
			}
		}

		ZED_FLOAT32 Vector4::Dot( const Vector4 &p_Other ) const
		{
			return ( m_X*p_Other.m_X + m_Y*p_Other.m_Y +
				m_Z*p_Other.m_Z + m_W*p_Other.m_W );
		}

		System::Writer &operator<<( System::Writer p_Out,
			const Vector4 &p_Source )
		{
			p_Out << "Vector4: [ " << p_Source[ 0 ] << ", "
				<< p_Source[ 1 ] << ", "
				<< p_Source[ 2 ] << ", "
				<< p_Source[ 3 ] << " ]" << std::endl;

			return p_Out;
		}

		ZED_BOOL Vector4::operator==( const Vector4 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other.m_X ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other.m_Z ) &&
				ZED::Arithmetic::Equal( m_W, p_Other.m_W ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector4::operator!=( const Vector4 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other.m_X ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other.m_Z ) &&
				ZED::Arithmetic::Equal( m_W, p_Other.m_W ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
		}

		Vector4 &Vector4::operator=( const Vector4 &p_Copy )
		{
			m_X = p_Copy.m_X;
			m_Y = p_Copy.m_Y;
			m_Z = p_Copy.m_Z;
			m_W = p_Copy.m_W;

			return *this;
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

		Vector4 operator*( const ZED_FLOAT32 p_Scalar, const Vector4 &p_Self )
		{
			return Vector4( p_Self[ 0 ]*p_Scalar, p_Self[ 1 ]*p_Scalar,
				p_Self[ 2 ]*p_Scalar, p_Self[ 3 ]*p_Scalar );
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

		Vector4 &operator*=( const ZED_FLOAT32 p_Scalar,
			Vector4 &p_Self )
		{
			p_Self[ 0 ] *= p_Scalar;
			p_Self[ 1 ] *= p_Scalar;
			p_Self[ 2 ] *= p_Scalar;
			p_Self[ 3 ] *= p_Scalar;

			return p_Self;
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
