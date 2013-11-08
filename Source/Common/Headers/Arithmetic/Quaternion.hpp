#ifndef __ZED_ARITHMETIC_QUATERNION_HPP__
#define __ZED_ARITHMETIC_QUATERNION_HPP__

#include <System/DataTypes.hpp>
#include <Arithmetic/Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Quaternion
		{
		public:
			ZED_INLINE Quaternion( ) : m_X( 0.0f ),
				m_Y( 0.0f ), m_Z( 0.0f ), m_W( 1.0f ){ }

			Quaternion( const Quaternion &p_Other );
			Quaternion &operator=( const Quaternion &p_Other );

			ZED_INLINE ~Quaternion( ){ }

			// Component constructor
			ZED_INLINE Quaternion( ZED_FLOAT32 p_W, ZED_FLOAT32 p_X,
				ZED_FLOAT32 p_Y, ZED_FLOAT32 p_Z ) : m_X( p_X ), m_Y( p_Y ),
				m_Z( p_Z ), m_W( p_W ){ }
			// Compound constructor
			ZED_INLINE Quaternion( ZED_FLOAT32 p_W, const Vector3 &p_Vec ) :
				m_W( p_W ), m_X( p_Vec.X( ) ), m_Y( p_Vec.Y( ) ),
				m_Z( p_Vec.Z( ) ){ }

			// Set the quaternion component-wise
			ZED_INLINE void Set( const ZED_FLOAT32 p_W, const ZED_FLOAT32 p_X,
				const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z )
				{ m_X = p_X; m_Y = p_Y; m_Z = p_Z; m_W = p_W; }

			// Returns the Quaternion as a Vector3
			ZED::Arithmetic::Vector3 AsVector( );

			void Normalise( );

			ZED_FLOAT32 Magnitude( ) const;
			ZED_FLOAT32 MagnitudeSq( ) const;

			Quaternion Conjugate( ) const;

			// Returns true if the quaternion is of unit length
			ZED_BOOL IsUnit( ) const;

			// Non-destructive operators
			Quaternion operator*( const Quaternion &p_Other ) const;
			
			// Destructive operators
			Quaternion &operator*=( const Quaternion &p_Other );

			// Easy access to the elements of the Quaternion
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const ZED_UINT32 p_Index )
				{ return ( &m_X )[ p_Index ]; }
			ZED_INLINE ZED_FLOAT32 operator[ ]( const ZED_UINT32 p_Index )
				const { return ( &m_X )[ p_Index ]; }

		private:
			ZED_FLOAT32 m_X, m_Y, m_Z, m_W;
		};
	}
}

#endif
