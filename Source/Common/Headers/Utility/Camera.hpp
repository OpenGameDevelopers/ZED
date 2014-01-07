#ifndef __ZED_UTILITY_CAMERA_HPP__
#define __ZED_UTILITY_CAMERA_HPP__

#include <Renderer/Renderer.hpp>

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
			virtual ~Camera( );

			ZED_INLINE void Position(
				const ZED::Arithmetic::Vector3 &p_Position )
				{ m_Position = p_Position; }

			void Direction(
				const ZED::Arithmetic::Vector3 &p_Direction )
				{ m_Direction = p_Direction; }

			ZED_INLINE void Position(
				Arithmetic::Vector3 *p_pPosition ) const
				{ ( *p_pPosition ) = m_Position; }

			void SetPosition( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Z );

			ZED_INLINE void Direction(
				Arithmetic::Vector3 *p_pDirection ) const
				{ ( *p_pDirection ) = m_Direction; }

			void ClippingPlanes( const ZED_FLOAT32 p_Near,
				const ZED_FLOAT32 p_Far );

			void ViewMode( const ZED_VIEWMODE p_ViewMode );
			ZED_INLINE ZED_VIEWMODE ViewMode( ) const { return m_ViewMode; }

			// p_pFrustum must be an array of six planes
			void Frustum( Arithmetic::Plane *p_pFrustum ) const;
			void RenderFrustum( const ZED_COLOUR p_Colour ) const;

			void View3D(
				const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Direction,
				const Arithmetic::Vector3 &p_Position );

			void ViewLookAt(
				const Arithmetic::Vector3 &p_Position,
				const Arithmetic::Vector3 &p_Point,
				const Arithmetic::Vector3 &p_WorldUp );

			ZED_UINT32 PerspectiveProjection( const ZED_FLOAT32 p_FOV,
				const ZED_FLOAT32 p_AspectRatio,
				Arithmetic::Matrix4x4 *p_pMatrix );

			ZED_BOOL Active( ) const { return m_Active; }
			void Activate( ) { m_Active = ZED_TRUE; }
			void Deactivate( ) { m_Active = ZED_FALSE; }

			void Projection( Arithmetic::Matrix4x4 *p_pMatrix ) const
				{ ( *p_pMatrix ) = m_Projection; }
			void View( Arithmetic::Matrix4x4 *p_pMatrix ) const
				{ ( *p_pMatrix ) = m_View; }

			void GetProjectionViewMatrix(
				Arithmetic::Matrix4x4 *p_pProjectionViewMatrix ) const;

			void Renderer( ZED::Renderer::Renderer *p_pRenderer )
				{ m_pRenderer = p_pRenderer; }

		protected:
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

