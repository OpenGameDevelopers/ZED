#include <Utility/FirstPersonCamera.hpp>

namespace ZED
{
	namespace Utility
	{
		FirstPersonCamera::FirstPersonCamera( )
		{
		}

		FirstPersonCamera::~FirstPersonCamera( )
		{
		}

		void FirstPersonCamera::Move( const Arithmetic::Vector3 &p_Velocity )
		{
			ZED_FLOAT32 Sin = 0.0f, Cos = 0.0f;
			ZED::Arithmetic::SinCos( m_Direction[ 1 ], Sin, Cos );

			// Moving along X
			if( p_Velocity[ 0 ] > ZED_Epsilon )
			{
				m_Position[ 0 ] += Cos * p_Velocity[ 0 ];
				m_Position[ 2 ] -= Sin * p_Velocity[ 0 ];
			}
			if( p_Velocity[ 0 ] < -ZED_Epsilon )
			{
				m_Position[ 0 ] -= Cos * p_Velocity[ 0 ];
				m_Position[ 2 ] += Sin * p_Velocity[ 0 ];
			}
			
			// Moving along Z
			if( p_Velocity[ 2 ] > ZED_Epsilon )
			{
				m_Position[ 0 ] -= Sin * p_Velocity[ 2 ];
				m_Position[ 2 ] -= Cos * p_Velocity[ 2 ];
			}
			if( p_Velocity[ 2 ] < -ZED_Epsilon )
			{
				m_Position[ 0 ] += Sin * p_Velocity[ 2 ];
				m_Position[ 2 ] += Cos * p_Velocity[ 2 ];
			}
		}

		void FirstPersonCamera::Look( const Arithmetic::Vector3 &p_Velocity )
		{
			m_Direction[ 0 ] -= p_Velocity[ 0 ];
			m_Direction[ 1 ] -= p_Velocity[ 1 ];

			if( m_Direction[ 1 ] > m_MaxPitch )
			{
				m_Direction[ 1 ] = m_MaxPitch;
			}
			if( m_Direction[ 1 ] < -m_MaxPitch )
			{
				m_Direction[ 1 ] = -m_MaxPitch;
			}
		}
	}
}

