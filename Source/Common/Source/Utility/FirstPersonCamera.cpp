#include <Utility/FirstPersonCamera.hpp>
#include <Arithmetic/Matrix3x3.hpp>

namespace ZED
{
	namespace Utility
	{
		FirstPersonCamera::FirstPersonCamera( ) :
			m_MaxPitch( ZED_Pi / 2.0f )
		{
			m_Direction.Set( 0.0f, 0.0f, -1.0f );
		}

		FirstPersonCamera::~FirstPersonCamera( )
		{
		}

		void FirstPersonCamera::SetMaxPitch( const ZED_FLOAT32 p_MaxPitch )
		{
			m_MaxPitch = p_MaxPitch;
		}

		void FirstPersonCamera::Move( const Arithmetic::Vector3 &p_Velocity )
		{
			ZED::Arithmetic::Vector3 Strafe, Thrust;

			Strafe = m_LocalRight * p_Velocity[ 0 ];
			Thrust = m_LocalDirection * p_Velocity[ 2 ];

			m_Position += Strafe + Thrust;
		}

		void FirstPersonCamera::Rotate( const ZED_FLOAT32 p_Angle,
			const Arithmetic::Vector3 &p_Axis )
		{
			ZED_FLOAT32 AngleSine, AngleCosine;

			ZED::Arithmetic::SinCos( p_Angle / 2.0f, AngleSine, AngleCosine );

			m_Rotation[ 0 ] = p_Axis[ 0 ] * AngleSine;
			m_Rotation[ 1 ] = p_Axis[ 1 ] * AngleSine;
			m_Rotation[ 2 ] = p_Axis[ 2 ] * AngleSine;
			m_Rotation[ 3 ] = AngleCosine;
		}

		void FirstPersonCamera::Update( const ZED_UINT64 p_ElapsedTime )
		{
			this->RecalculateAxes( );

			ZED::Arithmetic::Matrix3x3 Upper3x3;
			Upper3x3.SetColumns( m_LocalRight, m_LocalUp, -m_LocalDirection );

			Arithmetic::Vector3 Position = ( -m_Position * Upper3x3 );
			
			this->SetView3D( m_LocalRight, m_LocalUp, m_LocalDirection,
				Position );
		}
	}
}

