#ifndef __ZED_ARITHMETIC_QUATERNION_HPP__
#define __ZED_ARITHMETIC_QUATERNION_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Vector3;

		class Quaternion
		{
		public:
			Quaternion( );

			Quaternion( const Quaternion &p_Other );
			Quaternion &operator=( const Quaternion &p_Other );

			// Component constructor
			Quaternion( const ZED_FLOAT32 p_W, const ZED_FLOAT32 p_X,
				const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z );

			// Compound constructor
			Quaternion( const ZED_FLOAT32 p_W, const Vector3 &p_Vec );

			~Quaternion( );

			// Set the quaternion component-wise
			void Set( const ZED_FLOAT32 p_W, const ZED_FLOAT32 p_X,
				const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z );

			void SetIdentity( );

			ZED_BOOL IsIdentity( ) const;

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
			Vector3 operator*( const Vector3 &p_Vector ) const;
			
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
