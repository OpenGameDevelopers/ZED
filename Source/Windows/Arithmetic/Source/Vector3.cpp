#include <Vector3.hpp>
#include <Matrix3x3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		// Create the jump tables for the various operations.
		typedef void ( *Void_VEC3_Handle )( Vector3 & );
		typedef ZED_FLOAT32 ( *Float32_VEC3_Handle )( const Vector3 & );
		typedef ZED_FLOAT32 ( *Float32_VEC3_VEC3_Handle )( const Vector3 &,
			const Vector3 & );

		// Normalise implementations
		void NormaliseC( Vector3 &p_Vec )
		{
			ZED_FLOAT32 LengthSq =	p_Vec[ 0 ]*p_Vec[ 0 ] +
									p_Vec[ 1 ]*p_Vec[ 1 ] +
									p_Vec[ 2 ]*p_Vec[ 2 ];

			if( ZED::Arithmetic::IsZero( LengthSq ) )
			{
				p_Vec.Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor =
					ZED::Arithmetic::InvSquareRoot( LengthSq );

				p_Vec[ 0 ] *= Factor;
				p_Vec[ 1 ] *= Factor;
				p_Vec[ 2 ] *= Factor;
			}
		}
#ifdef ZED_PLATFORM_WIN64_X86
		extern void NormaliseMMX( Vector3 &p_Vec );
#else
		void NormaliseMMX( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
#endif
		void NormaliseMMXEX( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void Normalise3DNOW( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void Normalise3DNOWPLUS( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void Normalise3DNOWGEO( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE2( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE3( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSSE3( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE41( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE42( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE4a( Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}

		// Normalise jump table.
		Void_VEC3_Handle Normalise_JT[ 13 ] =
		{
			NormaliseC,
			NormaliseMMX,
			NormaliseMMXEX,
			Normalise3DNOW,
			Normalise3DNOWPLUS,
			Normalise3DNOWGEO,
			NormaliseSSE,
			NormaliseSSE2,
			NormaliseSSE3,
			NormaliseSSSE3,
			NormaliseSSE41,
			NormaliseSSE42,
			NormaliseSSE4a
		};

		void Vector3::Normalise( )
		{			
			return Normalise_JT[ SIMDType ]( *this );
		}

		ZED_FLOAT32 MagnitudeC( const Vector3 &p_Vec )
		{
			return ZED::Arithmetic::SquareRoot(
				p_Vec[ 0 ]*p_Vec[ 0 ] +
				p_Vec[ 1 ]*p_Vec[ 1 ] +
				p_Vec[ 2 ]*p_Vec[ 2 ] );
		}
		ZED_FLOAT32 MagnitudeMMX( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeMMXEX( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Magnitude3DNOW( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Magnitude3DNOWPLUS( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Magnitude3DNOWGEO( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE2( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE3( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSSE3( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE41( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE42( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE4a( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}

		// Magnitude Jump Table
		Float32_VEC3_Handle Magnitude_JT[ 13 ] =
		{
			MagnitudeC,
			MagnitudeMMX,
			MagnitudeMMXEX,
			Magnitude3DNOW,
			Magnitude3DNOWPLUS,
			Magnitude3DNOWGEO,
			MagnitudeSSE,
			MagnitudeSSE2,
			MagnitudeSSE3,
			MagnitudeSSSE3,
			MagnitudeSSE41,
			MagnitudeSSE42,
			MagnitudeSSE4a
		};

		ZED_FLOAT32 Vector3::Magnitude( ) const
		{
			return Magnitude_JT[ SIMDType ]( *this );
		}

		// Magnitude Squared functions for the Jump Table
		ZED_FLOAT32 MagnitudeSqC( const Vector3 &p_Vec )
		{
			return( p_Vec[ 0 ]*p_Vec[ 0 ] + p_Vec[ 1 ]*p_Vec[ 1 ] +
				p_Vec[ 2 ]*p_Vec[ 2 ] );
		}
		ZED_FLOAT32 MagnitudeSqMMX( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqMMXEX( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSq3DNOW( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSq3DNOWPLUS( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSq3DNOWGEO( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE2( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE3( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSSE3( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE41( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE42( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE4a( const Vector3 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}

		// Set up the Magnitude Squared Jump Table
		Float32_VEC3_Handle MagnitudeSq_JT[ 13 ] =
		{
			MagnitudeSqC,
			MagnitudeSqMMX,
			MagnitudeSqMMXEX,
			MagnitudeSq3DNOW,
			MagnitudeSq3DNOWPLUS,
			MagnitudeSq3DNOWGEO,
			MagnitudeSqSSE,
			MagnitudeSqSSE2,
			MagnitudeSqSSE3,
			MagnitudeSqSSSE3,
			MagnitudeSqSSE41,
			MagnitudeSqSSE42,
			MagnitudeSqSSE4a
		};

		ZED_FLOAT32 Vector3::MagnitudeSq( ) const
		{
			return MagnitudeSq_JT[ SIMDType ]( *this );
		}

		ZED_FLOAT32 Vector3::Distance( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];

			return ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z );
		}
		
		ZED_FLOAT32 Vector3::DistanceSq( const Vector3 &p_Other ) const
		{
			ZED_FLOAT32 X = m_X - p_Other[ 0 ];
			ZED_FLOAT32 Y = m_Y - p_Other[ 1 ];
			ZED_FLOAT32 Z = m_Z - p_Other[ 2 ];

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
			ZED_FLOAT32 ReturnValue = 0.0f;

			return ( m_X*p_Other[ 0 ] + m_Y*p_Other[ 1 ] + m_Z*p_Other[ 2 ] );
		}

		Vector3 Vector3::Cross( const Vector3 &p_Other ) const
		{
			return Vector3( ( m_Y*p_Other[ 2 ] ) - ( m_Z*p_Other[ 1 ] ),
							( m_Z*p_Other[ 0 ] ) - ( m_X*p_Other[ 2 ] ),
							( m_X*p_Other[ 1 ] ) - ( m_Y*p_Other[ 0 ] ) );
		}

		ZED_BOOL Vector3::operator==( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other[ 2 ] ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		ZED_BOOL Vector3::operator!=( const Vector3 &p_Other ) const
		{
			if( ZED::Arithmetic::Equal( m_X, p_Other[ 0 ] ) &&
				ZED::Arithmetic::Equal( m_Y, p_Other[ 1 ] ) &&
				ZED::Arithmetic::Equal( m_Z, p_Other[ 2 ] ) )
			{
				return ZED_FALSE;
			}
			return ZED_TRUE;
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
			Vector3 ReturnVec;

			ReturnVec[ 0 ] = ( p_Matrix[ 0 ]*m_X ) +
							 ( p_Matrix[ 1 ]*m_Y ) +
							 ( p_Matrix[ 2 ]*m_Z );
			ReturnVec[ 1 ] = ( p_Matrix[ 3 ]*m_X ) +
							 ( p_Matrix[ 4 ]*m_Y ) +
							 ( p_Matrix[ 5 ]*m_Z );
			ReturnVec[ 2 ] = ( p_Matrix[ 6 ]*m_X ) +
							 ( p_Matrix[ 7 ]*m_Y ) +
							 ( p_Matrix[ 8 ]*m_Z );

			return Vector3( ReturnVec.m_X, ReturnVec.m_Y, ReturnVec.m_Z );
		}

		Vector3 operator*( const ZED_FLOAT32 p_Scalar, const Vector3 &p_Self )
		{
			return Vector3( p_Scalar*p_Self[ 0 ], p_Scalar*p_Self[ 1 ],
				p_Scalar*p_Self[ 2 ] );
		}

		Vector3 Vector3::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			return Vector3( m_X/p_Scalar, m_Y/p_Scalar, m_Z/p_Scalar );
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
			m_X /= p_Scalar;
			m_Y /= p_Scalar;
			m_Z /= p_Scalar;

			return *this;
		}
	}
}
