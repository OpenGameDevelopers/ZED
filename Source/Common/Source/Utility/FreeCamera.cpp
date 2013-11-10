#include <Utility/FreeCamera.hpp>
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
			m_Direction.Set( 0.0f, 0.0f, -1.0f );
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

			this->ViewLookAt( m_Position, CameraLookAt,
				ZED::Arithmetic::Vector3( 0.0f, 1.0f, 0.0f ) );
		}

		void FreeCamera::Rotate( const ZED_FLOAT32 p_Angle,
			const ZED::Arithmetic::Vector3 &p_Axis )
		{
			ZED::Arithmetic::Quaternion Temp, View, Res;
			ZED_FLOAT32 Cos = 0.0f, Sin = 0.0f;
			ZED::Arithmetic::SinCos( p_Angle / 2.0f, Sin, Cos );

			Temp[ 0 ] = p_Axis[ 0 ] * Sin;
			Temp[ 1 ] = p_Axis[ 1 ] * Sin;
			Temp[ 2 ] = p_Axis[ 2 ] * Sin;
			Temp[ 3 ] = Cos;

			zedTrace( "Temp: < %f %f %f %f >\n", Temp[ 0 ], Temp[ 1 ], Temp[ 2 ],
				Temp[ 3 ] );

			View[ 0 ] = m_Direction[ 0 ];
			View[ 1 ] = m_Direction[ 1 ];
			View[ 2 ] = m_Direction[ 2 ];
			View[ 3 ] = 0.0f;

			zedTrace( "View: < %f %f %f %f >\n", View[ 0 ], View[ 1 ], View[ 2 ],
				View[ 3 ] );

			Res = Temp * View * Temp.Conjugate( );
			m_Direction[ 0 ] = Res[ 0 ];
			m_Direction[ 1 ] = Res[ 1 ];
			m_Direction[ 2 ] = Res[ 2 ];

			ZED::Arithmetic::Vector3 CameraLookAt( Res[ 0 ], Res[ 1 ],
				Res[ 2 ] );

			zedTrace( "CameraLookAt: < %f %f %f >\n", CameraLookAt[ 0 ],
				CameraLookAt[ 1 ], CameraLookAt[ 2 ]  );
			
			this->ViewLookAt( m_Position, CameraLookAt,
				ZED::Arithmetic::Vector3( 0.0f, 1.0f, 0.0f ) );
		}

		void FreeCamera::Move( const ZED::Arithmetic::Vector3 &p_Velocity )
		{
			m_Position += m_Orientation.Transform( p_Velocity );
		}
	}
}

