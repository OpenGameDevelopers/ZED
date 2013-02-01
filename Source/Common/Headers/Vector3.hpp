#ifndef __ZEDVECTOR3_HPP__
#define __ZEDVECTOR3_HPP__

#include <DataTypes.hpp>
#include <Arithmetic.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Matrix3x3;

		class Vector3
		{
			friend class Matrix3x3;
		public:
			ZED_INLINE Vector3( ) : m_X( 0.0f ), m_Y( 0.0f ), m_Z( 0.0f ){ }

			// Constructor for setting X, Y, and Z
			ZED_INLINE Vector3( const ZED_FLOAT32 p_X, ZED_FLOAT32 const p_Y,
								   const ZED_FLOAT32 p_Z );

			// Allow for explicit copy and clone
			Vector3 &Clone( ) const;
			void Copy( const Vector3 &p_Original );

			// Normalise, magnitude (squared) and distance (squared)
			void Normalise( );
			ZED_FLOAT32 Magnitude( ) const;
			ZED_FLOAT32 MagnitudeSq( ) const;
			
			// Return the distance between this vector and another (see the
			// procedural interface for the all-round function)
			ZED_FLOAT32 Distance( const Vector3 &p_Other ) const;
			ZED_FLOAT32 DistanceSq( const Vector3 &p_Other ) const;
								  
			ZED_INLINE void Set( const ZED_FLOAT32 p_X,
								 const ZED_FLOAT32 p_Y,
								 const ZED_FLOAT32 p_Z );
								 
			// Set the individual elements
			ZED_INLINE void SetX( const ZED_FLOAT32 p_X ){ m_X = p_X; }
			ZED_INLINE void SetY( const ZED_FLOAT32 p_Y ){ m_Y = p_Y; }
			ZED_INLINE void SetZ( const ZED_FLOAT32 p_Z ){ m_Z = p_Z; }
			
			// Accessors
			ZED_INLINE ZED_FLOAT32 GetX( ) const { return m_X; }
			ZED_INLINE ZED_FLOAT32 GetY( ) const { return m_Y; }
			ZED_INLINE ZED_FLOAT32 GetZ( ) const { return m_Z; }

			// Determine if the vector is close enough to zero (account for
			// floating point innacuracies)
			ZED_INLINE ZED_BOOL	IsZero( ) const;
			ZED_INLINE void Zero( ){ m_X = m_Y = m_Z = 0.0f; }

			// Set near-zero elements to zero
			void Clean( );

			// Dot/Cross procuct
			ZED_FLOAT32 Dot( const Vector3 &p_Other ) const;
			Vector3 Cross( const Vector3 &p_Other ) const;

			// Operator overloading time
			// -Debugging-
			friend ZED::System::Writer &operator<<(
				ZED::System::Writer &p_Out,
				const Vector3 p_Source );

			// - Returning elements-
			// -Modify-
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const ZED_UINT32 p_Index )
				{ return ( &m_X )[ p_Index ]; }
			// -Access-
			ZED_INLINE ZED_FLOAT32 operator[ ](
				const ZED_UINT32 p_Index ) const
				{ return ( &m_X )[ p_Index ]; }

			// -Equality-
			ZED_BOOL operator==( const Vector3 &p_Other ) const;
			ZED_BOOL operator!=( const Vector3 &p_Other ) const;

			// -Unary negation-
			ZED_INLINE Vector3 operator-( ) const
				{ return Vector3( -m_X, -m_Y, -m_Z ); }

			// -Addition/Subtraction-
			Vector3 operator+( const Vector3 &p_Other ) const;
			Vector3 operator-( const Vector3 &p_other ) const;

			// -Multiplication/Division-
			Vector3 operator*( const Vector3 &p_Other ) const;
			Vector3 operator*( const ZED_FLOAT32 p_Scalar ) const;
			friend Vector3 operator*( const ZED_FLOAT32 p_Scalar,
				const Vector3 &p_Self );
			Vector3 operator*( const Matrix3x3 &p_Matrix ) const;
			Vector3 operator/( const ZED_FLOAT32 p_Scalar ) const;

			// -Assignment-
			Vector3 &operator+=( const Vector3 &p_Other );
			Vector3 &operator-=( const Vector3 &p_Other );
			Vector3 &operator*=( const Vector3 &p_Other );
			Vector3 &operator*=( const ZED_FLOAT32 p_Scalar );
			friend Vector3 &operator*=( const ZED_FLOAT32 p_Scalar,
				Vector3 &p_Self );
			Vector3 &operator/=( const ZED_FLOAT32 p_Scalar );

		private:
			ZED_FLOAT32 m_X, m_Y, m_Z;

			// Disable indirect copy and clone
#ifdef ZED_CPPVER_11
			Vector3( const Vector3 &p_Copy ) = delete;
			Vector3 &operator=( const Vector3 &p_Clone ) = delete;
#endif
#ifdef ZED_CPPVER_03
			Vector3( const Vector3 &p_Copy );
			Vector3 &operator=( const Vector3 &p_Clone );
#endif
		};
		
		ZED_INLINE ZED_BOOL	Vector3::IsZero( ) const
		{
			return ZED::Arithmetic::IsZero( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		ZED_INLINE Vector3::Vector3( const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z ) :
			m_X( p_X ), m_Y( p_Y ), m_Z( p_Z )
		{
		}
		
		ZED_INLINE void Vector3::Set( const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z )
		{
			m_X = p_X;
			m_Y = p_Y;
			m_Z = p_Z;
		}
	}
}

#endif
