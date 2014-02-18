#include <Utility/FirstPersonCamera.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <Arithmetic/Vector4.hpp>

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
			ZED::Arithmetic::Matrix4x4 InverseView;
			m_View.AffineInverse( InverseView );

			ZED::Arithmetic::Vector4 Right;
			InverseView.GetColumn( 0, Right );

			m_Position[ 0 ] += Right[ 0 ] * p_Velocity[ 0 ];
			m_Position[ 2 ] += Right[ 2 ] * p_Velocity[ 0 ];

			ZED::Arithmetic::Vector4 Forward;
			InverseView.GetColumn( 2, Forward );

			m_Position[ 0 ] += Forward[ 0 ] * p_Velocity[ 2 ];
			m_Position[ 2 ] += Forward[ 2 ] * p_Velocity[ 2 ];
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
		}

		void FirstPersonCamera::Update( const ZED_UINT64 p_ElapsedTime )
		{
			ZED::Arithmetic::Vector3 LookAt( 0.0f, 0.0f, 1.0f );
			ZED::Arithmetic::Vector3 Up( 0.0f, 1.0f, 0.0f );
			LookAt = m_Rotation * LookAt;
			Up = m_Rotation * Up;
			
			this->SetViewLookAt( m_Position, LookAt + m_Position,
				Up );
		}
	}
}

