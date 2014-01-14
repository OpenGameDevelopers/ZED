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
			m_Direction.Set( 0.0f, -1.0f, 0.0f );
			m_Orbit.Set( 0.0f, 0.0f, 0.0f );
		}

		FreeCamera::~FreeCamera( )
		{
		}

		void FreeCamera::Rotate( const ZED::Arithmetic::Vector3 &p_Rotation )
		{
			/*m_Direction += p_Rotation;
			m_Orientation.Rotate( m_Direction[ 2 ], m_Direction[ 0 ],
				m_Direction[ 1 ] );
			ZED::Arithmetic::Vector3 CameraReference( 0.0f, 0.0f, -1.0f );
			ZED::Arithmetic::Vector3 LookAt =
				m_Orientation.Transform( CameraReference );

			ZED::Arithmetic::Vector3 CameraLookAt = m_Position + LookAt;

			this->SetViewLookAt( m_Position, CameraLookAt,
				ZED::Arithmetic::Vector3( 0.0f, 1.0f, 0.0f ) );*/
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

		void FreeCamera::UpdateOrbitLocation( )
		{
			m_Orbit = m_Position+m_Direction;
			-m_Orbit;
			zedTrace( "Orbit: %f %f %f\n\n", m_Orbit[ 0 ], m_Orbit[ 1 ], m_Orbit[ 2 ] );
		}

		void FreeCamera::Update( const ZED_UINT64 p_ElapsedTime )
		{
			this->RecalculateAxes( );

			zedTrace( "m_Position: < %f %f %f >\n", m_Position[ 0 ],
				m_Position[ 1 ], m_Position[ 2 ] );

			ZED::Arithmetic::Matrix3x3 Upper3x3;
			Upper3x3.SetColumns( m_LocalRight, m_LocalUp, m_LocalDirection );

			Arithmetic::Vector3 Position = ( -m_Position*Upper3x3 );
			this->SetView3D( m_LocalRight, m_LocalUp, m_LocalDirection,
				Position );
				/*

			ZED::Arithmetic::Vector3 Position( 0.0f, 170.0f, 0.0f );
			ZED::Arithmetic::Vector3 Point( 0.0f, 0.0f, 0.0f );
			this->SetViewLookAt( Position, Point, ZED::Arithmetic::Vector3( 0.0f, 1.0f, 0.0f ) );*/
		}
	}
}

