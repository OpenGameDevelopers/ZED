#include <Utility/FreeCamera.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Arithmetic/Vector3.hpp>
#include <System/Debugger.hpp>
#include <Arithmetic/Quaternion.hpp>

namespace ZED
{
	namespace Utility
	{
		FreeCamera::FreeCamera( )
		{
		}

		FreeCamera::~FreeCamera( )
		{
		}

		void FreeCamera::Rotate( const ZED_FLOAT32 p_Angle,
			const ZED::Arithmetic::Vector3 &p_Axis )
		{
			ZED_FLOAT32 AngleSine, AngleCosine;

			ZED::Arithmetic::SinCos( p_Angle / 2.0f, AngleSine, AngleCosine );

			m_Rotation[ 0 ] = p_Axis[ 0 ] * AngleSine;
			m_Rotation[ 1 ] = p_Axis[ 1 ] * AngleSine;
			m_Rotation[ 2 ] = p_Axis[ 2 ] * AngleSine;
			m_Rotation[ 3 ] = AngleCosine;
		}

		void FreeCamera::Move( const ZED::Arithmetic::Vector3 &p_Velocity )
		{
			ZED::Arithmetic::Vector3 Strafe, Thrust, Elevation;

			Strafe = m_LocalRight * p_Velocity[ 0 ];
			Elevation = m_LocalUp * p_Velocity[ 1 ];
			Thrust = m_LocalDirection * p_Velocity[ 2 ];

			m_Position += Strafe + Elevation + Thrust;
		}

		void FreeCamera::Update( const ZED_UINT64 p_ElapsedTime )
		{
			this->RecalculateAxes( );

			ZED::Arithmetic::Matrix3x3 Upper3x3;
			Upper3x3.SetColumns( m_LocalRight, m_LocalUp, -m_LocalDirection );

			Arithmetic::Vector3 Position = ( -m_Position*Upper3x3 );

			this->SetView3D( m_LocalRight, m_LocalUp, m_LocalDirection,
				Position );
		}
	}
}

