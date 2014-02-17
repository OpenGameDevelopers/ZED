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
			ZED::Arithmetic::Vector3 Strafe( 1.0f, 0.0f, 0.0f );
			ZED::Arithmetic::Vector3 Thrust( 0.0f, 0.0f, 1.0f );

			Strafe *= p_Velocity[ 0 ];
			Thrust *= p_Velocity[ 2 ];

			ZED::Arithmetic::Quaternion Velocity( 0.0f, p_Velocity );

			Velocity = m_Rotation * Velocity * m_Rotation.Conjugate( );


			//m_Position += m_Rotation * Strafe;
			//m_Position += m_Rotation * Thrust;
			m_Position += Velocity.AsVector( );
		}

		void FirstPersonCamera::Rotate( const ZED_FLOAT32 p_Angle,
			const Arithmetic::Vector3 &p_Axis )
		{
			ZED_FLOAT32 AngleSine, AngleCosine;

			ZED::Arithmetic::SinCos( p_Angle / 2.0f, AngleSine, AngleCosine );

			ZED::Arithmetic::Quaternion RotationPitch;
			RotationPitch[ 0 ] = p_Axis[ 0 ] * AngleSine;
			RotationPitch[ 1 ] = 0.0f;
			RotationPitch[ 2 ] = 0.0f;
			RotationPitch[ 3 ] = AngleCosine;


			ZED::Arithmetic::Quaternion RotationYaw;
			RotationYaw[ 0 ] = 0.0f;
			RotationYaw[ 1 ] = p_Axis[ 1 ] * AngleSine;
			RotationYaw[ 2 ] = 0.0f;
			RotationYaw[ 3 ] = AngleCosine;

			m_Rotation = RotationPitch * m_Rotation * RotationYaw;
/*
			zedTrace( "m_Rotation: %f %f %f [%f]\n", m_Rotation[ 0 ],
				m_Rotation[ 1 ], m_Rotation[ 2 ], m_Rotation[ 3 ] );*/

		}

		void FirstPersonCamera::Update( const ZED_UINT64 p_ElapsedTime )
		{
			ZED::Arithmetic::Quaternion LookAt( 0.0f, 0.0f, 0.0f, 1.0f );
			ZED::Arithmetic::Quaternion Up( 0.0f, 0.0f, 1.0f, 0.0f );

			LookAt = m_Rotation * LookAt * m_Rotation.Conjugate( );
			Up = m_Rotation * Up * m_Rotation.Conjugate( );

		/*	zedTrace( "LookAt: %f %f %f\n",
				( LookAt.AsVector( ) + m_Position )[ 0 ],
				( LookAt.AsVector( ) + m_Position )[ 1 ],
				( LookAt.AsVector( ) + m_Position )[ 2 ] );
			zedTrace( "Position: %f %f %f\n", m_Position[ 0 ], m_Position[ 1 ],
				m_Position[ 2 ] );*/
			
			this->SetViewLookAt( m_Position, LookAt.AsVector( ) + m_Position,
				Up.AsVector( ) );
		}
	}
}

