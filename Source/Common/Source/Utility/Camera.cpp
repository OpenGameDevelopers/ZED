#include <Utility/Camera.hpp>
#include <Arithmetic/Plane.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Arithmetic/Quaternion.hpp>

namespace ZED
{
	namespace Utility
	{
		Camera::Camera( ) :
			m_ViewMode( ZED_VIEWMODE_INVALID ),
			m_Active( ZED_FALSE ),

#if defined ZED_BUILD_DEBUG
			m_pRenderer( ZED_NULL ),
#endif
			m_Near( 0.0f ),
			m_Far( 0.0f )
		{
			m_View.Identity( ),
			m_Projection.Identity( );
			m_ViewProjection.Identity( );
			m_Position.Zero( );
			m_Direction.Zero( );
		}

		Camera::~Camera( )
		{
		}

		void Camera::SetPosition( const ZED::Arithmetic::Vector3 &p_Position )
		{
			m_Position = p_Position;
		}

		void Camera::SetPosition( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
			const ZED_FLOAT32 p_Z )
		{
			m_Position.Set( p_X, p_Y, p_Z );
			m_Direction += m_Position;
		}

		void Camera::SetDirection(
			const ZED::Arithmetic::Vector3 &p_Direction )
		{
			m_Direction = p_Direction;
		}

		void Camera::GetPosition( ZED::Arithmetic::Vector3 *p_pPosition ) const
		{
			( *p_pPosition ) = m_Position;
		}

		void Camera::GetDirection(
			ZED::Arithmetic::Vector3 *p_pDirection ) const
		{
			( *p_pDirection ) = m_Direction;
		}

		void Camera::SetClippingPlanes( const ZED_FLOAT32 p_Near,
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

		void Camera::GetClippingPlanes( ZED_FLOAT32 &p_Near,
			ZED_FLOAT32 &p_Far ) const
		{
			p_Near = m_Near;
			p_Far = m_Far;
		}

		void Camera::SetViewMode( const ZED_VIEWMODE p_ViewMode )
		{
			m_ViewMode = p_ViewMode;

			if( m_ViewMode == ZED_VIEWMODE_INVALID )
			{
				zedTrace( "[ZED::Renderer::Camera::ViewMode] <WARN> "
					"Invalid view mode set\n" );
				return;
			}
		}

		ZED_VIEWMODE Camera::GetViewMode( ) const
		{
			return m_ViewMode;
		}

		void Camera::SetView3D( const Arithmetic::Vector3 &p_Right,
			const Arithmetic::Vector3 &p_Up,
			const Arithmetic::Vector3 &p_Direction,
			const Arithmetic::Vector3 &p_Position )
		{
			// Right-handed matrix for the camera:
			// R = Right, U = Up, D = Direction, P = Position
			// R U D P
			// R U D P
			// R U D P
			// 0 0 0 1
			m_View( 3, 0 ) = m_View( 3, 1 ) = m_View( 3, 2 ) = 0.0f;
			m_View( 3, 3 ) = 1.0f;

			m_View( 0, 0 ) = p_Right[ 0 ];
			m_View( 1, 0 ) = p_Right[ 1 ];
			m_View( 2, 0 ) = p_Right[ 2 ];

			m_View( 0, 1 ) = p_Up[ 0 ];
			m_View( 1, 1 ) = p_Up[ 1 ];
			m_View( 2, 1 ) = p_Up[ 2 ];

			m_View( 0, 2 ) = p_Direction[ 0 ];
			m_View( 1, 2 ) = p_Direction[ 1 ];
			m_View( 2, 2 ) = p_Direction[ 2 ];

			m_View( 0, 3 ) = p_Position[ 0 ];
			m_View( 1, 3 ) = p_Position[ 1 ];
			m_View( 2, 3 ) = p_Position[ 2 ];

			zedTrace( "SetView3D-------------------------------------------\n" );

			zedTrace( "Right:     < %f %f %f >\n", p_Right[ 0 ], p_Right[ 1 ],
				p_Right[ 2 ] );
			zedTrace( "Up:        < %f %f %f >\n", p_Up[ 0 ], p_Up[ 1 ], p_Up[ 2 ] );
			zedTrace( "Direction: < %f %f %f >\n", p_Direction[ 0 ],
				p_Direction[ 1 ], p_Direction[ 2 ] );
			zedTrace( "------------------------------------------------------\n" );
		}

		void Camera::SetViewLookAt(
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
			Upper3x3.SetColumns( Right, Up, -Direction );

			Arithmetic::Vector3 Position = -( p_Position*Upper3x3 );

			zedTrace( "ViewLookAt:\n" );
			zedTrace( "Direction: %f %f %f\n",
				-Direction[ 0 ], -Direction[ 1 ], -Direction[ 2 ] );
			zedTrace( "Position:  %f %f %f\n",
				Position[ 0 ], Position[ 1 ], Position[ 2 ] );

			this->SetView3D( Right, Up, -Direction, Position );
		}

		ZED_UINT32 Camera::SetPerspectiveProjection( const ZED_FLOAT32 p_FOV,
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

			if( p_pMatrix )
			{
				( *p_pMatrix )( 0, 0 ) = D / p_AspectRatio;
				( *p_pMatrix )( 1, 1 ) = D;
				( *p_pMatrix )( 2, 2 ) = ( m_Near + m_Far ) * Recip;
				( *p_pMatrix )( 2, 3 ) = 2 * m_Near * m_Far * Recip;
				( *p_pMatrix )( 3, 2 ) = -1.0f;
				( *p_pMatrix )( 3, 3 ) = 0.0f;

				m_Projection = ( *p_pMatrix );
			}
			else
			{
				m_Projection( 0, 0 ) = D / p_AspectRatio;
				m_Projection( 1, 1 ) = D;
				m_Projection( 2, 2 ) = ( m_Near + m_Far ) * Recip;
				m_Projection( 2, 3 ) = 2 * m_Near * m_Far * Recip;
				m_Projection( 3, 2 ) = -1.0f;
				m_Projection( 3, 3 ) = 0.0f;
			}

			return ZED_OK;
		}

		ZED_BOOL Camera::IsActive( ) const
		{
			return m_Active;
		}

		void Camera::Activate( )
		{
			m_Active = ZED_TRUE;
		}

		void Camera::Deactivate( )
		{
			m_Active = ZED_FALSE;
		}

		void Camera::GetProjectionMatrix(
			Arithmetic::Matrix4x4 *p_pMatrix ) const
		{
			( *p_pMatrix ) = m_Projection;
		}

		void Camera::GetViewMatrix( Arithmetic::Matrix4x4 *p_pMatrix ) const
		{
			( *p_pMatrix ) = m_View;
		}

		void Camera::GetProjectionViewMatrix(
			ZED::Arithmetic::Matrix4x4 *p_pProjectionViewMatrix ) const
		{
			( *p_pProjectionViewMatrix ) = m_Projection*m_View;
		}

		void Camera::SetRenderer( ZED::Renderer::Renderer *p_pRenderer )
		{
#if defined ZED_BUILD_DEBUG
			m_pRenderer = p_pRenderer;
#endif
		}

		void Camera::RecalculateAxes( )
		{
			m_Orientation *= m_Rotation;

			ZED::Arithmetic::Matrix3x3 Axes;
			Axes.Rotate( m_Orientation );
			
			m_LocalRight[ 0 ] = Axes[ 0 ];
			m_LocalRight[ 1 ] = Axes[ 3 ];
			m_LocalRight[ 2 ] = Axes[ 6 ];

			m_LocalUp[ 0 ] = Axes[ 1 ];
			m_LocalUp[ 1 ] = Axes[ 4 ];
			m_LocalUp[ 2 ] = Axes[ 7 ];

			m_LocalDirection[ 0 ] = Axes[ 2 ];
			m_LocalDirection[ 1 ] = Axes[ 5 ];
			m_LocalDirection[ 2 ] = Axes[ 8 ];

			zedTrace( "Right:     < %f %f %f >\n", m_LocalRight[ 0 ],
				m_LocalRight[ 1 ], m_LocalRight[ 2 ] );
			zedTrace( "Up:        < %f %f %f >\n", m_LocalUp[ 0 ],
				m_LocalUp[ 1 ], m_LocalUp[ 2 ] );
			zedTrace( "Direction: < %f %f %f >\n", m_LocalDirection[ 0 ],
				m_LocalDirection[ 1 ], m_LocalDirection[ 2 ] );
		}
	}
}

