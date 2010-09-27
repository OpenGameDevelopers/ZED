#include <Vector3Generic.hpp>

namespace ZED
{
	namespace Arithmetic
	{/*
		Vector3::Vector3( const Vector3 &p_Vec3 )
		{
			m_X = p_Vec3.m_X;
			m_Y = p_Vec3.m_Y;
			m_Z = p_Vec3.m_Z;
		}*/

		void Vector3Generic::Normalise( )
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
		}

		ZED_FLOAT32 Vector3Generic::Magnitude( ) const
		{
			return ZED::Arithmetic::SquareRoot( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3Generic::MagnitudeSq( ) const
		{
			return ( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_FLOAT32 Vector3Generic::Distance( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];

			return ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z );
		}
		
		ZED_FLOAT32 Vector3Generic::DistanceSq( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];

			return ( X*X + Y*Y + Z*Z );
		}

		// Clean up the elements
		void Vector3Generic::Clean( )
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

		ZED_FLOAT32 Vector3Generic::Dot( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 ReturnValue = 0.0f;
			// Add a fourth element for doing the dot product
			ZED_FLOAT32 TempVector[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };


			return ( m_X*p_Other.GetX( ) + m_Y*p_Other.GetY( ) +
				m_Z*p_Other.GetZ( ) );
			//return ReturnValue;
		}

		Vector3 Vector3Generic::Cross( const Vector3 &p_Other ) const
		{
			return Vector3( ( m_Y*p_Other[ 2 ] ) - ( m_Z*p_Other[ 1 ] ),
							( m_Z*p_Other[ 0 ] ) - ( m_X*p_Other[ 2 ] ),
							( m_X*p_Other[ 1 ] ) - ( m_Y*p_Other[ 0 ] ) );
		}

		ZED_BOOL Vector3Generic::operator==( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other[ 2 ] ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector3Generic::operator!=( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other[ 2 ] ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
		}

		/*Vector3 &Vector3::operator=( const Vector3 &p_Vec3 )
		{
			return Vector3( p_Vec3.m_X, p_Vec3.m_Y, p_Vec3.m_Z );
		}*/
		
		Vector3 Vector3Generic::operator+( const Vector3 &p_Other ) const
		{
			return Vector3( m_X+p_Other[ 0 ], m_Y+p_Other[ 1 ],
				m_Z+p_Other[ 2 ] );
		}

		Vector3 Vector3Generic::operator-( const Vector3 &p_Other ) const
		{
			return Vector3( m_X-p_Other[ 0 ], m_Y-p_Other[ 1 ],
				m_Z-p_Other[ 2 ] );
		}

		Vector3 Vector3Generic::operator*( const Vector3 &p_Other ) const
		{
			return Vector3( m_X*p_Other[ 0 ], m_Y*p_Other[ 1 ],
				m_Z*p_Other[ 2 ] );

		}

		Vector3 Vector3Generic::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X*p_Scalar, m_Y*p_Scalar, m_Z*p_Scalar );
		}

		Vector3 operator*( const ZED_FLOAT32 p_Scalar, const Vector3 &p_Self )
		{
			return Vector3( p_Scalar*p_Self[ 0 ], p_Scalar*p_Self[ 1 ],
				p_Scalar*p_Self[ 2 ] );
		}

		Vector3 Vector3Generic::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X/p_Scalar, m_Y/p_Scalar, m_Z/p_Scalar );
		}

		Vector3 &Vector3Generic::operator+=( const Vector3 &p_Other )
		{
			m_X += p_Other[ 0 ];
			m_Y += p_Other[ 1 ];
			m_Z += p_Other[ 2 ];

			return *this;
		}

		Vector3 &Vector3Generic::operator-=( const Vector3 &p_Other )
		{
			m_X -= p_Other[ 0 ];
			m_Y -= p_Other[ 1 ];
			m_Z -= p_Other[ 2 ];

			return *this;
		}

		Vector3 &Vector3Generic::operator*=( const Vector3 &p_Other )
		{
			m_X *= p_Other[ 0 ];
			m_Y *= p_Other[ 1 ];
			m_Z *= p_Other[ 2 ];

			return *this;
		}

		Vector3 &Vector3Generic::operator*=( const ZED_FLOAT32 p_Scalar )
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

		Vector3 &Vector3Generic::operator/=( const ZED_FLOAT32 p_Scalar )
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
