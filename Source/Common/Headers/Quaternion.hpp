#ifndef __ZEDQUATERNION_HPP__
#define __ZEDQUATERNION_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Quaternion
		{
		public:
			ZED_INLINE Quaternion( ) : m_X( 0.0f ),
				m_Y( 0.0f ), m_Z( 0.0f ), m_W( 0.0f ){ }
			ZED_INLINE ~Quaternion( );

			// Component constructor
			ZED_INLINE Quaternion( ZED_FLOAT32 p_X, ZED_FLOAT32 p_Y,
				ZED_FLOAT32 p_Z, ZED_FLOAT32 p_W ) : m_X( p_X ), m_Y( p_Y ),
				m_Z( p_Z ), m_W( p_W ){ }
			// Compound constructor
			ZED_INLINE Quaternion( ZED_FLOAT32 p_W, const Vector3 &p_Vec ) :
				m_W( p_W ), m_X( p_Vec.GetX( ) ), m_Y( p_Vec.GetY( ) ),
				m_Z( p_Vec.GetZ( ) ){ }

			// Returns the Quaternion as a Vector3
			ZED::Arithmetic::Vector3 Vector3( );

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
