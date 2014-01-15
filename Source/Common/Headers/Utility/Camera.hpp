#ifndef __ZED_UTILITY_CAMERA_HPP__
#define __ZED_UTILITY_CAMERA_HPP__

#include <Renderer/Renderer.hpp>
#include <Arithmetic/Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		// Forward declarations
		class Vector3;
		class Plane;
		class Quaternion;
		class Matrix4x4;
	}

	namespace Utility
	{
		class Camera
		{
		public:
			Camera( );
			Camera( const Camera &p_OtherCamera );
			virtual ~Camera( );

			Camera &operator=( const Camera &p_OtherCamera );

			void SetPosition( const ZED::Arithmetic::Vector3 &p_Position );
			void SetPosition( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Z );
			void SetDirection( const ZED::Arithmetic::Vector3 &p_Direction );
			void SetOrientation(
				const ZED::Arithmetic::Quaternion &p_Orientation );

			void GetPosition( Arithmetic::Vector3 *p_pPosition ) const;
			void GetDirection( Arithmetic::Vector3 *p_pDirection ) const;
			void GetOrientation(
				ZED::Arithmetic::Quaternion *p_pQuaternion ) const;

			void SetClippingPlanes( const ZED_FLOAT32 p_Near,
				const ZED_FLOAT32 p_Far );
			void GetClippingPlanes( ZED_FLOAT32 &p_Near,
				ZED_FLOAT32 &p_Far ) const;

			void SetViewMode( const ZED_VIEWMODE p_ViewMode );
			ZED_INLINE ZED_VIEWMODE GetViewMode( ) const;

			// p_pFrustum must be an array of six planes
			void GetFrustum( Arithmetic::Plane *p_pFrustum ) const;
			void RenderFrustum( const ZED_COLOUR p_Colour ) const;

			void SetView3D(
				const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Direction,
				const Arithmetic::Vector3 &p_Position );

			void SetViewLookAt(
				const Arithmetic::Vector3 &p_Position,
				const Arithmetic::Vector3 &p_Point,
				const Arithmetic::Vector3 &p_WorldUp );

			ZED_UINT32 SetPerspectiveProjection( const ZED_FLOAT32 p_FOV,
				const ZED_FLOAT32 p_AspectRatio,
				Arithmetic::Matrix4x4 *p_pMatrix );

			ZED_BOOL IsActive( ) const;
			void Activate( );
			void Deactivate( );

			void GetProjectionMatrix( Arithmetic::Matrix4x4 *p_pMatrix ) const;
			void GetViewMatrix( Arithmetic::Matrix4x4 *p_pMatrix ) const;
			void GetProjectionViewMatrix(
				Arithmetic::Matrix4x4 *p_pProjectionViewMatrix ) const;

			void SetRenderer( ZED::Renderer::Renderer *p_pRenderer );

			virtual void Update( const ZED_UINT64 p_ElapsedTime ) = 0;

			virtual void Rotate( const ZED_FLOAT32 p_Angle,
				const ZED::Arithmetic::Vector3 &p_Axis ) = 0;

		protected:
			void RecalculateAxes( );

			ZED::Arithmetic::Vector3	m_Position;
			ZED::Arithmetic::Vector3	m_Direction;

			// View can be in 2D (screen-space) or in 3D (camera space)
			Arithmetic::Matrix4x4 m_View;
			// Projection can be perspective or orthogonal
			Arithmetic::Matrix4x4 m_Projection;
			Arithmetic::Matrix4x4 m_ViewProjection;
			
			ZED_BOOL		m_Active;
			ZED_VIEWMODE	m_ViewMode;

			// Clip plane limits
			ZED_FLOAT32		m_Near;
			ZED_FLOAT32		m_Far;

			// All quaternion-based camera rotation
			ZED::Arithmetic::Quaternion	m_Orientation;
			ZED::Arithmetic::Quaternion	m_Rotation;
			ZED::Arithmetic::Vector3	m_LocalRight;
			ZED::Arithmetic::Vector3	m_LocalUp;
			ZED::Arithmetic::Vector3	m_LocalDirection;

#if defined ZED_BUILD_DEBUG
			// The renderer is needed for drawing the debug visualisation of
			// the camera's view frustum
			ZED::Renderer::Renderer		*m_pRenderer;

			ZED_FLOAT32		m_FrustumVertices[ 8 ];
			ZED_UINT16		m_FrustumIndices[ 24 ];
#endif
		};
	}
}

#endif

