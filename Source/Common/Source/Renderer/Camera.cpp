#include <Renderer/Camera.hpp>
#include <Arithmetic/Plane.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <Arithmetic/Matrix4x4.hpp>

namespace ZED
{
	namespace Renderer
	{
		Camera::Camera( )
		{
			m_ViewMode = ZED_VIEWMODE_INVALID;
			m_Active = ZED_FALSE;
			m_View.Identity( );
			m_Projection.Identity( );
			m_ViewProjection.Identity( );
			m_Position.Zero( );
			m_Direction.Zero( );
			m_pRenderer = ZED_NULL;
			m_Near = 0.0f;
			m_Far = 0.0f;
		}

		Camera::~Camera( )
		{
		}

		void Camera::ClippingPlanes( const ZED_FLOAT32 p_Near,
			const ZED_FLOAT32 p_Far )
		{
			m_Near = p_Near;
			m_Far = p_Far;
			if( m_Near < ZED_Epsilon )
			{
				m_Near = ZED_Epsilon;
			}

			if( m_Far <= 1.0f )
			{
				m_Far = 1.0f;
			}
			
			if( m_Near > m_Far )
			{
				m_Near = m_Far;
				m_Far += 1.0f;
			}

			if( m_ViewMode == ZED_VIEWMODE_PERSPECTIVE )
			{
				ZED_FLOAT32 FarFactor = ( 1.0f / ( m_Near - m_Far ) ) * m_Far;
				ZED_FLOAT32 NearFactor = -FarFactor * m_Near;
				m_Projection( 2, 2 ) = FarFactor;
				m_Projection( 3, 2 ) = NearFactor;
			}
		}

		void Camera::ViewMode( const ZED_VIEWMODE p_ViewMode )
		{
			m_ViewMode = p_ViewMode;

			if( m_ViewMode == ZED_VIEWMODE_INVALID )
			{
				zedTrace( "[ZED::Renderer::Camera::ViewMode] <WARN> "
					"Invalid view mode set\n" );

				return;
			}
		}

		void Camera::View3D( const Arithmetic::Vector3 &p_Right,
			const Arithmetic::Vector3 &p_Up,
			const Arithmetic::Vector3 &p_Direction,
			const Arithmetic::Vector3 &p_Position )
		{
			// Right-handed matrix for the camera:
			// R = Right, U = Up, D = Direction, P = Position
			// R R R P
			// U U U P
			// D D D P
			// 0 0 0 1
			m_View( 3, 0 ) = m_View( 3, 1 ) = m_View( 3, 2 ) = 0.0f;
			m_View( 3, 3 ) = 1.0f;

			m_View( 0, 0 ) = p_Right[ 0 ];
			m_View( 0, 1 ) = p_Right[ 1 ];
			m_View( 0, 2 ) = p_Right[ 2 ];

			m_View( 1, 0 ) = p_Up[ 0 ];
			m_View( 1, 1 ) = p_Up[ 1 ];
			m_View( 1, 2 ) = p_Up[ 2 ];

			m_View( 2, 0 ) = p_Direction[ 0 ];
			m_View( 2, 1 ) = p_Direction[ 1 ];
			m_View( 2, 2 ) = p_Direction[ 2 ];

			m_View( 0, 3 ) = p_Position[ 0 ];
			m_View( 1, 3 ) = p_Position[ 1 ];
			m_View( 2, 3 ) = p_Position[ 2 ];
		}

		void Camera::ViewLookAt(
			const Arithmetic::Vector3 &p_Position,
			const Arithmetic::Vector3 &p_Point,
			const Arithmetic::Vector3 &p_WorldUp )
		{
			Arithmetic::Vector3 Direction, Right, Up;

			Direction = ( p_Point - p_Position );
			Direction.Normalise( );

			Right = Direction.Cross( p_WorldUp );
			Right.Normalise( );

			Up = Right.Cross( Direction );
			Up.Normalise( );

			Arithmetic::Matrix3x3 Upper3x3;
			// OpenGL needs -Direction, though D3D does not use -Direction
			// Maybe there should be a member variable for the renderer?
			Upper3x3.SetRows( Right, Up, -Direction );

			Arithmetic::Vector3 Position = -( Upper3x3*p_Position );

			this->View3D( Right, Up, -Direction, Position );
		}

		ZED_UINT32 Camera::PerspectiveProjection( const ZED_FLOAT32 p_FOV,
			const ZED_FLOAT32 p_AspectRatio,
			Arithmetic::Matrix4x4 *p_pMatrix )
		{
			if( Arithmetic::Absolute( m_Far - m_Near ) < ZED_Epsilon )
			{
				zedTrace( "[ZED::Renderer::Camera::PerspectiveProjection] "
					"<ERROR> Far plane - near plane less than epsilon\n" );

				return ZED_FAIL;
			}

			ZED_FLOAT32 D = 1.0f/tan( p_FOV / 180.0f ) * ZED_Pi * 0.5f;
			ZED_FLOAT32 Recip = 1.0f/( m_Near - m_Far );

			( *p_pMatrix )( 0, 0 ) = D / p_AspectRatio;
			( *p_pMatrix )( 1, 1 ) = D;
			( *p_pMatrix )( 2, 2 ) = ( m_Near + m_Far ) * Recip;
			( *p_pMatrix )( 2, 3 ) = 2 * m_Near * m_Far * Recip;
			( *p_pMatrix )( 3, 2 ) = -1.0f;
			( *p_pMatrix )( 3, 3 ) = 0.0f;

			m_Projection = ( *p_pMatrix );

			return ZED_OK;
		}
	}
}

