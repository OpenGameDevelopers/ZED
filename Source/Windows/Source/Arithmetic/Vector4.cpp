#include <Arithmetic/Arithmetic.hpp>
#include <Arithmetic/Vector4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		// Create the jump tables for the various operations.
		typedef void ( *Void_VEC4_Handle )( Vector4 & );
		typedef ZED_FLOAT32 ( *Float32_VEC4_Handle )( const Vector4 & );
		typedef ZED_FLOAT32 ( *Float32_VEC4_VEC4_Handle )( const Vector4 &,
			const Vector4 & );

		// Normalise functions for various SIMD types.
		void NormaliseC( Vector4 &p_Vec )
		{
			ZED_FLOAT32 LengthSq = p_Vec[ 0 ]*p_Vec[ 0 ] +
				p_Vec[ 1 ]*p_Vec[ 1 ] + p_Vec[ 2 ]*p_Vec[ 2 ] +
				p_Vec[ 3 ]*p_Vec[ 3 ];

			// Check to make sure the value is workable
			if( ZED::Arithmetic::IsZero( LengthSq ) )
			{
				p_Vec.Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor =
					ZED::Arithmetic::InvSquareRoot(
					LengthSq );

				p_Vec[ 0 ] *= Factor;
				p_Vec[ 1 ] *= Factor;
				p_Vec[ 2 ] *= Factor;
				p_Vec[ 3 ] *= Factor;
			}
		}
		void NormaliseMMX( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseMMXEX( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void Normalise3DNOW( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void Normalise3DNOWPLUS( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void Normalise3DNOWGEO( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE2( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE3( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSSE3( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE41( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE42( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}
		void NormaliseSSE4a( Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
		}

		// Normalise jump table.
		Void_VEC4_Handle Normalise_JT[ 13 ] =
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

		void Vector4::Normalise( )
		{
			return Normalise_JT[ SIMDType ]( *this );
		}

		// Magnitude functions for various SIMD types.
		ZED_FLOAT32 MagnitudeC( const Vector4 &p_Vec )
		{
			return( ZED::Arithmetic::SquareRoot( p_Vec[ 0 ]*p_Vec[ 0 ] +
				p_Vec[ 1 ]*p_Vec[ 1 ] + p_Vec[ 2 ]*p_Vec[ 2 ] +
				p_Vec[ 3 ]*p_Vec[ 3 ] ) );
		}
		ZED_FLOAT32 MagnitudeMMX( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeMMXEX( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Magnitude3DNOW( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Magnitude3DNOWPLUS( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Magnitude3DNOWGEO( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE2( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE3( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSSE3( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE41( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE42( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSSE4a( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}

		// Magnitude jump table.
		Float32_VEC4_Handle Magnitude_JT[ 13 ] =
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

		ZED_FLOAT32 Vector4::Magnitude( ) const
		{
			return Magnitude_JT[ SIMDType ]( *this );
		}

		// Magnitude Squared functions for various SIMD types.
		ZED_FLOAT32 MagnitudeSqC( const Vector4 &p_Vec )
		{
			return ( p_Vec[ 0 ]*p_Vec[ 0 ] + p_Vec[ 1 ]*p_Vec[ 1 ] +
				p_Vec[ 2 ]*p_Vec[ 2 ] + p_Vec[ 3 ]*p_Vec[ 3 ] );
		}
		ZED_FLOAT32 MagnitudeSqMMX( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqMMXEX( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSq3DNOW( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSq3DNOWPLUS( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSq3DNOWGEO( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE2( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE3( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSSE3( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE41( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE42( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 MagnitudeSqSSE4a( const Vector4 &p_Vec )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}

		Float32_VEC4_Handle MagnitudeSq_JT[ 13 ] =
		{
			MagnitudeSqC,
			MagnitudeSqMMX,
			MagnitudeSqMMXEX,
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

		ZED_FLOAT32 Vector4::MagnitudeSq( ) const
		{
			return MagnitudeSq_JT[ SIMDType ]( *this );
		}

		// Distance functions for various SIMD types.
		ZED_FLOAT32 DistanceC( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			ZED_FLOAT32 X = p_Vec1[ 0 ] - p_Vec2[ 0 ];
			ZED_FLOAT32 Y = p_Vec1[ 1 ] - p_Vec2[ 1 ];
			ZED_FLOAT32 Z = p_Vec1[ 2 ] - p_Vec2[ 2 ];
			ZED_FLOAT32 W = p_Vec1[ 3 ] - p_Vec2[ 3 ];

			return ZED::Arithmetic::SquareRoot( X*X + Y*Y + Z*Z + W*W );
		}
		ZED_FLOAT32 DistanceMMX( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceMMXEX( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Distance3DNOW( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Distance3DNOWPLUS( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 Distance3DNOWGEO( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSE( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSE2( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSE3( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSSE3( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSE41( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSE42( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSSE4a( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}

		Float32_VEC4_VEC4_Handle Distance_JT[ 13 ] =
		{
			DistanceC,
			DistanceMMX,
			DistanceMMXEX,
			Distance3DNOW,
			Distance3DNOWPLUS,
			Distance3DNOWGEO,
			DistanceSSE,
			DistanceSSE2,
			DistanceSSE3,
			DistanceSSSE3,
			DistanceSSE41,
			DistanceSSE42,
			DistanceSSE4a
		};

		ZED_FLOAT32 Vector4::Distance( const Vector4 &p_Other ) const
		{
			return Distance_JT[ SIMDType ]( *this, p_Other );
		}

		// Distance Squared functions for various SIMD types.
		ZED_FLOAT32 DistanceSqC( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			ZED_FLOAT32 X = p_Vec1[ 0 ] - p_Vec2[ 0 ];
			ZED_FLOAT32 Y = p_Vec1[ 1 ] - p_Vec2[ 1 ];
			ZED_FLOAT32 Z = p_Vec1[ 2 ] - p_Vec2[ 2 ];
			ZED_FLOAT32 W = p_Vec1[ 3 ] - p_Vec2[ 3 ];

			return ( X*X + Y*Y + Z*Z + W*W );
		}
		ZED_FLOAT32 DistanceSqMMX( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqMMXEX( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSq3DNOW( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSq3DNOWPLUS( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSq3DNOWGEO( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSE( const Vector4 &p_Vec1, const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSE2( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSE3( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSSE3( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSE41( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSE42( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}
		ZED_FLOAT32 DistanceSqSSE4a( const Vector4 &p_Vec1,
			const Vector4 &p_Vec2 )
		{
			zedTrace( "You shouldn't have called this function\n\n" );
			zedAssert( false );
			return 2.0f;
		}

		// Distance Squared Jump Table
		Float32_VEC4_VEC4_Handle DistanceSq_JT[ 13 ] =
		{
			DistanceSqC,
			DistanceSqMMX,
			DistanceSqMMXEX,
			DistanceSq3DNOW,
			DistanceSq3DNOWPLUS,
			DistanceSq3DNOWGEO,
			DistanceSqSSE,
			DistanceSqSSE2,
			DistanceSqSSE3,
			DistanceSqSSSE3,
			DistanceSqSSE41,
			DistanceSqSSE42,
			DistanceSqSSE4a
		};

		ZED_FLOAT32 Vector4::DistanceSq( const Vector4 &p_Other ) const
		{
			return DistanceSq_JT[ SIMDType ]( *this, p_Other );
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
			return( m_X*p_Other.m_X + m_Y+p_Other.m_Y + m_Z*p_Other.m_Z +
				m_W*p_Other.m_W );
		}

		System::Writer &operator<<( System::Writer &p_Out,
			const Vector4 &p_Source )
		{
			p_Out << "< " << p_Source[ 0 ] << " " << p_Source[ 1 ] <<
				" " << p_Source[ 2 ] << " >" << std::endl;

			return p_Out;
		}

		ZED_BOOL Vector4::operator==( const Vector4 &p_Other ) const
		{
			if( !ZED::Arithmetic::Equal( m_X, p_Other.m_X ) )
			{
				return ZED_FALSE;
			}
			if( !ZED::Arithmetic::Equal( m_Y, p_Other.m_Y ) )
			{
				return ZED_FALSE;
			}
			if( !ZED::Arithmetic::Equal( m_Z, p_Other.m_Z ) )
			{
				return ZED_FALSE;
			}
			if( !ZED::Arithmetic::Equal( m_W, p_Other.m_W ) )
			{
				return ZED_FALSE;
			}

			return ZED_TRUE;
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

		Vector4 Vector4::operator+( const Vector4 &p_Other ) const
		{
			return Vector4( m_X+p_Other.m_X, m_Y+p_Other.m_Y,
				m_Z+p_Other.m_Z, m_W+p_Other.m_W );
		}

		Vector4 Vector4::operator-( const Vector4 &p_Other ) const
		{
			return Vector4( m_X-p_Other.m_X, m_Y-p_Other.m_Y,
				m_Z-p_Other.m_Z, m_W-p_Other.m_W );
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

		Vector4 operator*( const ZED_FLOAT32 p_Scalar,
			const Vector4 &p_Self )
		{
			return Vector4( p_Scalar*p_Self[ 0 ], p_Scalar*p_Self[ 1 ],
				p_Scalar*p_Self[ 2 ], p_Scalar*p_Self[ 3 ] );
		}

		Vector4 Vector4::operator/( const ZED_FLOAT32 p_Scalar ) const
		{
			// Just check if the scalar is zero...
			if( ZED::Arithmetic::IsZero( p_Scalar ) )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[ERROR | Vector4::operator/] Scalar is zero\n" );
				return *this;
			}

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
			if( ZED::Arithmetic::IsZero( p_Scalar ) )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[ERROR | Vector4::operator/=] Scalar is zero\n" );
				return *this;
			}

			m_X /= p_Scalar;
			m_Y /= p_Scalar;
			m_Z /= p_Scalar;
			m_W /= p_Scalar;

			return *this;
		}
	}
}
