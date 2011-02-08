#ifndef __ZEDVECTOR3_GENERIC_HPP__
#define __ZEDVECTOR3_GENERIC_HPP__

#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Vector3Generic : public Vector3
		{
		public:
			void Normalise( );
			ZED_FLOAT32 Magnitude( ) const;
			ZED_FLOAT32 MagnitudeSq( ) const;

			ZED_FLOAT32 Distance( const Vector3 &p_Other ) const;
			ZED_FLOAT32 DistanceSq( const Vector3 &p_Other ) const;

			void Clean( );

			ZED_FLOAT32 Dot( const Vector3 &p_Other ) const;
			Vector3 Cross( const Vector3 &p_Other ) const;

			ZED_BOOL operator==( const Vector3 &p_Other ) const;
			ZED_BOOL operator!=( const Vector3 &p_Other ) const;

			Vector3 operator+( const Vector3 &p_Other ) const;
			Vector3 operator-( const Vector3 &p_Other ) const;

			Vector3 operator*( const Vector3 &p_Other ) const;
			Vector3 operator*( const ZED_FLOAT32 p_Scalar ) const;
			friend Vector3 operator*( const ZED_FLOAT32 p_Scalar,
				const Vector3 &p_Self );
			Vector3 operator/( const ZED_FLOAT32 p_Scalar ) const;

			Vector3 &operator+=( const Vector3 &p_Other );
			Vector3 &operator-=( const Vector3 &p_Other );

			Vector3 &operator*=( const Vector3 &p_Other );
			Vector3 &operator*=( const ZED_FLOAT32 p_Scalar );
			friend Vector3 &operator*=( const ZED_FLOAT32 p_Scalar,
				const Vector3 &p_Self );
			Vector3 &operator/=( const ZED_FLOAT32 p_Scalar );
		};
	}
}

#endif
