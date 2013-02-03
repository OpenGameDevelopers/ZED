#ifndef __ZED_ARITHMETIC_VECTOR4_HPP__
#define __ZED_ARITHMETIC_VECTOR4_HPP__

#include <DataTypes.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Vector4
		{
		public:
			ZED_INLINE Vector4( ){ m_X = m_Y = m_Z = m_W = 0.0f; }
			
			Vector4( const Vector4 &p_Other );
			Vector4 &operator=( const Vector4 &p_Other );

			// Constructor for setting X, Y, Z, W
			ZED_INLINE Vector4( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Z, const ZED_FLOAT32 p_W );

			ZED_INLINE ~Vector4( ){ }

			// Normalise, magnitude (squared) and distance (squared)
			void Normalise( );
			ZED_FLOAT32 Magnitude( ) const;
			ZED_FLOAT32 MagnitudeSq( ) const;
			ZED_FLOAT32 Distance( const Vector4 &p_Other ) const;
			ZED_FLOAT32 DistanceSq( const Vector4 &p_Other ) const;

			// Set all elements
			ZED_INLINE void Set( const ZED_FLOAT32 p_X,
				const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z,
				const ZED_FLOAT32 p_W );

			// Set individual elements
			ZED_INLINE void SetX( const ZED_FLOAT32 p_X ){ m_X = p_X; }
			ZED_INLINE void SetY( const ZED_FLOAT32 p_Y ){ m_Y = p_Y; }
			ZED_INLINE void SetZ( const ZED_FLOAT32 p_Z ){ m_Z = p_Z; }
			ZED_INLINE void SetW( const ZED_FLOAT32 p_W ){ m_W = p_W; }

			// Accessors
			ZED_INLINE ZED_FLOAT32 GetX( ) const { return m_X; }
			ZED_INLINE ZED_FLOAT32 GetY( ) const { return m_Y; }
			ZED_INLINE ZED_FLOAT32 GetZ( ) const { return m_Z; }
			ZED_INLINE ZED_FLOAT32 GetW( ) const { return m_W; }

			// Is the vector zero?
			ZED_INLINE ZED_BOOL IsZero( ) const;
			// Zero-out the vector
			ZED_INLINE void Zero( ) { m_X = m_Y = m_Z = m_W = 0.0f; }

			// Set near-zero elements to zero
			void Clean( );

			// Dot with another vector
			ZED_FLOAT32 Dot( const Vector4 &p_Other ) const;

			// Operator overloading
			// -Debugging-
			friend System::Writer &operator<<( System::Writer &p_Out,
				const Vector4 &p_Source );

			// -Returning elements-
			// -Modify-
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const ZED_INT32 p_Index )
				{ return ( &m_X )[ p_Index ]; }
			// -Access-
			ZED_INLINE ZED_FLOAT32 operator[ ](
				const ZED_INT32 p_Index ) const
				{ return( &m_X )[ p_Index ]; }

			// -Equality-
			ZED_BOOL operator==( const Vector4 &p_Other ) const;
			ZED_BOOL operator!=( const Vector4 &p_Other ) const;

			// -Unary negation-
			ZED_INLINE Vector4 operator-( ) const;

			// -Addition/Subtraction-
			Vector4 operator+( const Vector4 &p_Other ) const;
			Vector4 operator-( const Vector4 &p_Other ) const;

			// -Multiply/Divide-
			Vector4 operator*( const Vector4 &p_Other ) const;
			Vector4 operator*( const ZED_FLOAT32 p_Scalar ) const;
			Vector4 operator/( const ZED_FLOAT32 p_Scalar ) const;

			// -Assignment-
			Vector4 &operator+=( const Vector4 &p_Other );
			Vector4 &operator-=( const Vector4 &p_Other );
			Vector4 &operator*=( const Vector4 &p_Other );
			Vector4 &operator*=( const ZED_FLOAT32 p_Scalar );
			Vector4 &operator/=( const ZED_FLOAT32 p_Scalar );

		private:
			ZED_FLOAT32 m_X, m_Y, m_Z, m_W;
		};

		ZED_INLINE Vector4::Vector4( const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z,
			const ZED_FLOAT32 p_W ) :
			m_X( p_X ), m_Y( p_Y ), m_Z( p_Z ), m_W( p_W )
		{
		}

		ZED_INLINE void Vector4::Set( const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z,
			const ZED_FLOAT32 p_W )
		{
			m_X = p_X;
			m_Y = p_Y;
			m_Z = p_Z;
			m_W = p_W;
		}
	}
}

#endif

