#ifndef __ZED_RENDERER_LINUXRENDEREROGL3_HPP__
#define __ZED_RENDERER_LINUXRENDEREROGL3_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <Renderer/Renderer.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Renderer/OGL/GLVertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		class LinuxRendererOGL3 : public Renderer
		{
		public:
			LinuxRendererOGL3( );
			virtual ~LinuxRendererOGL3( );

			virtual ZED_UINT32 Create(// GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window );

			virtual ZED_INLINE ZED_RENDERER_BACKEND BackEnd( )
				{ return ZED_RENDERER_BACKEND_OPENGL; }

			virtual void ForceClear( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual void ClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue );

			virtual ZED_UINT32 BeginScene( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );
			virtual void EndScene( );

			virtual ZED_BOOL ToggleFullscreen( );

			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual void Release( );

			virtual void SetView3D(
				const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Direction,
				const Arithmetic::Vector3 &p_Position );

			virtual void SetViewLookAt(
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

			virtual ZED_UINT32 PerspectiveProjectionMatrix(
				const ZED_FLOAT32 p_FOV, const ZED_FLOAT32 p_AspectRatio );

			virtual void PerspectiveProjectionMatrix(
				ZED::Arithmetic::Matrix4x4 *p_pMatrix ) const;

			virtual ZED_UINT32 SetMode( const ZED_UINT32 p_Stage,
				const ZED_VIEWMODE p_Mode );

			virtual ZED_INLINE ZED_BOOL ShaderSupport( )
				{ return m_ShaderSupport; }

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

/*			virtual ZED_UINT32 Create( GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas,
				Display *p_pDisplay );*/

			virtual void SetRenderState( const ZED_RENDERSTATE p_State,
				const ZED_MEMSIZE p_Value );

			// TEMP!
			// For after Create and after window creation
			virtual void CreateGLContext( );

			virtual void GetWVP( Arithmetic::Matrix4x4 *p_pMatrix );
			virtual void GetVP( Arithmetic::Matrix4x4 *p_pMatrix );
			// !TEMP

		private:
			GLExtender					m_GLExt;
			ZED::System::ZED_WINDOWDATA	m_WindowData;
			GLXContext					m_GLContext;

			CanvasDescription m_Canvas;
			GLVertexCacheManager *m_pVertexCacheManager;

			ZED_BOOL	m_ShaderSupport;
			
			// Near and far planes
			ZED_FLOAT32 m_Near;
			ZED_FLOAT32 m_Far;

			ZED_VIEWMODE	m_ViewMode;

			Arithmetic::Matrix4x4 m_View3D;

			Arithmetic::Matrix4x4 m_ProjectionPerspective;
			Arithmetic::Matrix4x4 m_World;
			Arithmetic::Matrix4x4 m_ViewScreen;
			Arithmetic::Matrix4x4 m_ProjectionScreen;
			Arithmetic::Matrix4x4 m_ViewProjection;
			Arithmetic::Matrix4x4 m_WVP;
			Arithmetic::Matrix4x4 m_PerspectiveProjection;
		};
	}
}

#endif

