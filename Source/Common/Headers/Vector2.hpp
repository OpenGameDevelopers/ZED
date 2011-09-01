#ifndef __ZED_ARITHMETIC_VECTOR2_HPP__
#define __ZED_ARITHMETIC_VECTOR2_HPP__

#include <DataTypes.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Vector2
		{
		public:
			ZED_INLINE Vector2( ){ m_X = m_Y = 0.0f; }
			Vector2( const Vector2 &p_Copy );

			// Contructor for X, Y
			ZED_INLINE Vector2( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y ):
				m_X( p_X ), m_Y( p_Y ) { }
			ZED_INLINE ~Vector2( ) { }

			ZED_FLOAT32 Dot( const Vector2 &p_Other ) const;
			void Normalise( );
			
			ZED_INLINE void Set( ZED_FLOAT32 p_X, ZED_FLOAT32 p_Y )
				{ m_X = p_X; m_Y = p_Y; }
			ZED_INLINE void SetX( ZED_FLOAT32 p_X ){ m_X = p_X; }
			ZED_INLINE void SetY( ZED_FLOAT32 p_Y ){ m_Y = p_Y; }

			ZED_INLINE ZED_FLOAT32 GetX( ) const { return m_X; }
			ZED_INLINE ZED_FLOAT32 GetY( ) const { return m_Y; }

			void Clean( );
			ZED_INLINE void Zero( ) { m_X = m_Y = 0.0f; }
			ZED_BOOL IsZero( ) const;

			ZED_FLOAT32 Magnitude( ) const;
			ZED_FLOAT32 MagnitudeSq( ) const;

			ZED_FLOAT32 Distance( const Vector2 &p_Other ) const;
			ZED_FLOAT32 DistanceSq( const Vector2 &p_Other ) const;

			ZED_INLINE Vector2 Perpendicular( ) const
				{ return Vector2( -m_Y, m_X ); }
			ZED_FLOAT32 PerpendicularDot( const Vector2 &p_Other ) const;
			friend ZED_FLOAT32 PerpendicularDot( const Vector2 &p_Vec1,
				const Vector2 &p_Vec2 );


			// Operators
			// -Debugging-
			friend ZED::System::Writer &operator<<( ZED::System::Writer p_Out,
				const Vector2 p_Source );

			// -Returning elements-
			// -Modify-
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const ZED_UINT32 p_Index )
				{ return ( &m_X )[ p_Index ]; }
			// -Access-
			ZED_INLINE ZED_FLOAT32 operator[ ]( const ZED_UINT32 p_Index )const
				{ return ( &m_X )[ p_Index ]; }

			// -Equality-
			ZED_BOOL operator==( const Vector2 &p_Other ) const;
			ZED_BOOL operator!=( const Vector2 &p_Other ) const;

			// -Unary negation-
			ZED_INLINE Vector2 operator-( ) const
				{ return Vector2( -m_X, -m_Y ); }

			// -Addition/subtraction-
			Vector2 operator+( const Vector2 &p_Other ) const;
			Vector2 operator-( const Vector2 &p_Other ) const;

			// -Multiply/Divide-
			Vector2 operator*( const Vector2 &p_Other ) const;
			Vector2 operator*( const ZED_FLOAT32 p_Scalar ) const;
			friend Vector2 operator*( const ZED_FLOAT32 p_Scalar,
				const Vector2 &p_Self );
			Vector2 operator/( const ZED_FLOAT32 p_Scalar ) const;

			// -Assignment-
			Vector2 &operator+=( const Vector2 &p_Other );
			Vector2 &operator-=( const Vector2 &p_Other );
			Vector2 &operator*=( const Vector2 &p_Other );
			Vector2 &operator*=( const ZED_FLOAT32 p_Scalar );
			friend Vector2 &operator*=( const ZED_FLOAT32 p_Scalar,
				Vector2 &p_Self );
			Vector2 &operator/=( const ZED_FLOAT32 p_Scalar );

		private:
			ZED_FLOAT32 m_X, m_Y;
		};
	}
}

#endif
