#ifndef __ZEDXBOXRENDERER_HPP__
#define __ZEDXBOXRENDERER_HPP__

#include <DataTypes.hpp>
#include <CanvasDescription.hpp>
#include <Renderer.hpp>
#include <Matrix4x4.hpp>

namespace ZED
{
	namespace Renderer
	{
		class XboxRenderer : public Renderer
		{
		public:
			XboxRenderer( );
			virtual ~XboxRenderer( );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_Adapter,
				const CanvasDescription &p_Canvas );

			virtual void ForceClear( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil );

			virtual void ClearColour( ZED_FLOAT32 p_Red, ZED_FLOAT32 p_Green,
				ZED_FLOAT32 p_Blue );

			virtual ZED_UINT32 BeginScene( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil );

			virtual void EndScene( );

			virtual ZED_BOOL ToggleFullscreen( );

			virtual ZED_UINT32 ResizeCanvas( ZED_UINT32 p_Width,
				ZED_UINT32 p_Height );

			// Clean up
			virtual void Release( );

			virtual ZED_UINT32 SetView3D( const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Dir,
				const Arithmetic::Vector3 &p_Position );

			virtual ZED_UINT32 SetViewLookAt(
				const Arithmetic::Vector3 &p_Position,
				const Arithmetic::Vector3 &p_Point,
				const Arithmetic::Vector3 &p_WorldUp );
			
			virtual void CalcViewProjMatrix( );
			virtual void CalcWorldViewProjMatrix( );

			virtual void SetClippingPlanes( const ZED_FLOAT32 p_Near,
				const ZED_FLOAT32 p_Far );

			virtual void Prepare2D( );

			virtual ZED_UINT32 CalcPerspProjMatrix( const ZED_FLOAT32 p_FOV,
				const ZED_FLOAT32 p_AspectRatio,
				Arithmetic::Matrix4x4 *p_pMatrix );

			virtual ZED_UINT32 SetMode( const ZED_UINT32 p_Stage,
				const ZED_VIEWMODE p_Mode );

			virtual ZED_UINT32 InitStage( const ZED_FLOAT32 p_FOV,
				const ZED_VIEWPORT &p_Viewport, ZED_UINT32 p_Stage );

			virtual void GetFrustum( Arithmetic::Plane *p_Planes );

			virtual void SetWorldTransform(
				Arithmetic::Matrix4x4 *p_pWorld );

		private:
			CanvasDescription		m_Canvas;
			LPDIRECT3DDEVICE8		m_pDevice;
			LPDIRECT3D8				m_pD3D;
			D3DPRESENT_PARAMETERS	m_PresentParams;
			D3DCOLOR				m_Colour;

			// Set to false to stop all further operations from taking place
			ZED_BOOL	m_Running;

			// Width, height and aspect ratio are always handy to have around
			ZED_FLOAT32 m_AspectRatio;
			ZED_UINT32 m_Width;
			ZED_UINT32 m_Height;

			// The matrices to use for viewing
			Arithmetic::Matrix4x4 m_View2D;
			Arithmetic::Matrix4x4 m_View3D;

			// Defines the current view [Screen, Persp., Ortho.]
			ZED_VIEWMODE m_Mode;

			// The near and far clip planes
			ZED_FLOAT32 m_Near;
			ZED_FLOAT32 m_Far;

			// Stages are used to render with different camera modes
			ZED_UINT32		m_Stage;
			// The viewports are used in conjunction with the stages to determine
			// the camera mode
			ZED_VIEWPORT	m_Viewport[ 4 ];

			// For the different view and projection matrices
			Arithmetic::Matrix4x4 m_ProjectionOrthogonal[ 4 ];
			Arithmetic::Matrix4x4 m_ProjectionPerspective[ 4 ];
			Arithmetic::Matrix4x4 m_ProjectionScreen;
			Arithmetic::Matrix4x4 m_ViewScreen;
			Arithmetic::Matrix4x4 m_World;
			Arithmetic::Matrix4x4 m_ViewProjection;
			Arithmetic::Matrix4x4 m_WorldViewProjection;
		};
	}
}

#endif
