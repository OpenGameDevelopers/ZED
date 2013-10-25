#include <Renderer/FreeCamera.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Arithmetic/Vector3.hpp>
#include <System/Debugger.hpp>

namespace ZED
{
	namespace Renderer
	{
		FreeCamera::FreeCamera( )
		{
		}

		FreeCamera::~FreeCamera( )
		{
		}

		void FreeCamera::Rotate( const ZED::Arithmetic::Vector3 &p_Rotation )
		{
			m_Direction += p_Rotation;
			m_Orientation.Rotate( m_Direction[ 2 ], m_Direction[ 0 ],
				m_Direction[ 1 ] );
			ZED::Arithmetic::Vector3 CameraReference( 0.0f, 0.0f, -1.0f );
			ZED::Arithmetic::Vector3 LookAt =
				m_Orientation.Transform( CameraReference );

			ZED::Arithmetic::Vector3 CameraLookAt = m_Position + LookAt;

			zedTrace( "Look at | < %f %f %f >\n", CameraLookAt[ 0 ],
				CameraLookAt[ 1 ], CameraLookAt[ 2 ] );
			
			this->ViewLookAt( m_Position, CameraLookAt,
				ZED::Arithmetic::Vector3( 0.0f, 1.0f, 0.0f ) );
		}

		void FreeCamera::Move( const ZED::Arithmetic::Vector3 &p_Velocity )
		{
			m_Position += m_Orientation.Transform( p_Velocity );
			zedTrace( "Position | < %f %f %f >\n", m_Position[ 0 ],
				m_Position[ 1 ], m_Position[ 2 ] );
		}
	}
}

