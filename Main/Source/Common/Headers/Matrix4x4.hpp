#ifndef __ZEDMATRIX4X4_HPP__
#define __ZEDMATRIX4X4_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Quaternion;
		class Matrix4x4
		{
		public:
			ZED_INLINE Matrix4x4( ){ Identity( ); }
			ZED_INLINE ~Matrix4x4( ){ }
			ZED_EXPLICIT Matrix4x4( const Quaternion &p_Quat );

			void Identity( );

			Matrix4x4 &Rotate( const Quaternion &p_Quat );

		private:
			ZED_FLOAT32 m_Element[ 16 ];
		};
	}
}

#endif
