#include <Matrix3x3.hpp>
//#include <Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		/*Matrix3x3::Matrix3x3( const Quaternion &p_Quat )
		{
			Rotate( p_Quat );
		}*/

		void Matrix3x3::Identity( )
		{
			m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] = m_Element[ 5 ] =
				m_Element[ 6 ] = m_Element[ 7 ] = 0.0f;
			m_Element[ 0 ] = m_Element[ 4 ] = m_Element[ 8 ] = 1.0f;
		}

		/*void Rotate( const Quaternion &p_Quat )
		{
		}*/

		void Matrix3x3::Rotate( const ZED_FLOAT32 p_Angle, const Vector3 &p_Axis )
		{
			ZED_FLOAT32 Cos = 0.0f, Sin = 0.0f;
			SinCos( p_Angle, Sin, Cos );

			ZED_FLOAT32 Tan = 1.0f - Cos;

			Vector3 nAxis = p_Axis;

			// Intermediate values
			ZED_FLOAT32 TanX = Tan*nAxis[ 0 ];
			ZED_FLOAT32 TanY = Tan*nAxis[ 1 ];
			ZED_FLOAT32 TanZ = Tan*nAxis[ 2 ];

			ZED_FLOAT32 SinX = Sin*nAxis[ 0 ];
			ZED_FLOAT32 SinY = Sin*nAxis[ 1 ];
			ZED_FLOAT32 SinZ = Sin*nAxis[ 2 ];
		}
	}
}
