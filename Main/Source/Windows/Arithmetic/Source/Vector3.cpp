#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		/*Vector3::Vector3( const Vector3 &p_Vec3 )
		{
			m_X = p_Vec3.m_X;
			m_Y = p_Vec3.m_Y;
			m_Z = p_Vec3.m_Z;
		}*/

		void Vector3::Normalise( )
		{			
			ZED_FLOAT32 LengthSq = m_X*m_X + m_Y*m_Y + m_Z*m_Z;

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
			}
			
			// Uncomment the above!!!
			/*__asm
			{
				MOV		ESI,	this		; // Vector U
				MOVUPS	XMM0,	[ ESI ]		; // First vector in XMM0
				// XMM2 = m_X, m_Y, m_Z, [undefined]
				MOVAPS	XMM2,	XMM0		; // Copy the original vector
				// m_X*m_X, m_Y*m_Y, m_Z*m_Z, [undefined]
				MULPS	XMM0,	XMM0		; // Multiply with second vector
				// XMM1 = Squared Vector3
				MOVAPS	XMM1,	XMM0		; // Copy result
				// m_Y^2, m_X^2, [undefined], m_Z^2
				SHUFPS	XMM1,	XMM1,	4Eh	; // Shuffle: 1, 0, 3, 2
				// m_
				ADDPS	XMM0,	XMM1		; // Add: 3+1, 2+0, 1+3, 0+2
				// 
				MOVAPS	XMM1,	XMM0		; // Copy results
				SHUFPS	XMM1,	XMM1,	11h	; // Shuffle: 0+2, 1+3, 0+2, 1+3
				ADDPS	XMM0,	XMM1		; // Add: 0+1+2+3, 0+1+2+3

				RSQRTPS	XMM0,	XMM0		; // Reciprocal square root
				MULPS	XMM2,	XMM0		; // Multiply by reciprocal
				MOVUPS	[ ESI ],XMM2		; // Bring back result
			}*/
		}

		ZED_FLOAT32 Vector3::Magnitude( ) const
		{
			return ZED::Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3::MagnitudeSq( ) const
		{
			return ( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3::Distance( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;

			return ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z );
		}

		ZED_FLOAT32 Vector3::DistanceSq( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other.m_X;
			ZED_FLOAT32 Y = m_Y - p_Other.m_Y;
			ZED_FLOAT32 Z = m_Z - p_Other.m_Z;

			return ( X*X + Y*Y + Z*Z );
		}

		// Clean up the elements
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
			return ( m_X*p_Other.m_X + m_Y*p_Other.m_Y + m_Z*p_Other.m_Z );
		}

		Vector3 Vector3::Cross( const Vector3 &p_Other ) const
		{
			return Vector3( ( m_Y*p_Other.m_Z ) - ( m_Z*p_Other.m_Y ),
							( m_Z*p_Other.m_X ) - ( m_X*p_Other.m_Z ),
							( m_X*p_Other.m_Y ) - ( m_Y*p_Other.m_X ) );
		}

		ZED_BOOL Vector3::operator==( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other.m_X ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other.m_Z ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector3::operator!=( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other.m_X ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other.m_Z ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
		}

		/*Vector3 &Vector3::operator=( const Vector3 &p_Vec3 )
		{
			return Vector3( p_Vec3.m_X, p_Vec3.m_Y, p_Vec3.m_Z );
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
			return Vector3( p_Scalar*p_Self.m_X, p_Scalar*p_Self.m_Y,
				p_Scalar*p_Self.m_Z );
		}

		Vector3 Vector3::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X/p_Scalar, m_Y/p_Scalar, m_Z/p_Scalar );
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
			return Vector3( p_Vec1.GetX( )+p_Vec2.GetX( ),
				p_Vec1.GetY( )+p_Vec2.GetY( ),
				p_Vec1.GetZ( )+p_Vec2.GetZ( ) );
		}

		Vector3 Sub( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			return Vector3 ( p_Vec1.GetX( ) - p_Vec2.GetX( ),
				p_Vec1.GetY( ) - p_Vec2.GetY( ),
				p_Vec1.GetZ( ) - p_Vec2.GetZ( ) );
		}

		Vector3 Mul( const Vector3 &p_Vec1, const Vector3 &p_Vec2 )
		{
			return Vector3( p_Vec1.GetX( )*p_Vec2.GetX( ),
							p_Vec1.GetY( )*p_Vec2.GetY( ),
							p_Vec1.GetZ( )*p_Vec2.GetZ( ) );
		}

		Vector3 Mul( const Vector3 &p_Vec, ZED_FLOAT32 p_Scalar )
		{
			return Vector3( p_Vec.GetX( )*p_Scalar,
							p_Vec.GetY( )*p_Scalar,
							p_Vec.GetZ( )*p_Scalar );
		}
	}
}
