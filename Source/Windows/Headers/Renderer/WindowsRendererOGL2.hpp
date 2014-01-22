#ifndef __ZED_RENDERER_WINDOWSRENDEREROGL2_HPP__
#define __ZED_RENDERER_WINDOWSRENDEREROGL2_HPP__

#include <Renderer/Renderer.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <Arithmetic/Vector3.hpp>
#include <System/Window.hpp>

namespace ZED
{
	namespace Renderer
	{
		class WindowsRendererOGL2 : public Renderer
		{
		public:
			WindowsRendererOGL2( );
			virtual ~WindowsRendererOGL2( );

			virtual ZED_UINT32 Create( const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window );

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

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID );

			virtual void RenderState( const ZED_RENDERSTATE p_State,
				const ZED_MEMSIZE p_Value );

		private:
			PIXELFORMATDESCRIPTOR	m_PixelFormat;
			HGLRC					m_HGLRC;
			HDC						m_HDC;
			GLExtender				m_Ext;
			CanvasDescription		m_Canvas;
			ZED_GLVERSION			m_GLVersion;

			// Near and far clipping planes
			ZED_FLOAT32 m_Near;
			ZED_FLOAT32 m_Far;

			// View mode in use
			ZED_VIEWMODE m_ViewMode;

			// Up to four viewports can be rendered
			ZED_VIEWPORT m_Viewport[ 4 ];

			// Which viewport is being rendered into?
			ZED_UINT32 m_Stage;

			// If the graphics engine winds down, this will be set to false so
			// further sub-systems can clean up
			ZED_BOOL m_Running;

			// View matrices
			Arithmetic::Matrix4x4 m_View2D;
			Arithmetic::Matrix4x4 m_View3D;

			// For the different view and projection matrices
			Arithmetic::Matrix4x4 m_ProjectionOrthogonal[ 4 ];
			Arithmetic::Matrix4x4 m_ProjectionPerspective[ 4 ];
			Arithmetic::Matrix4x4 m_World;
			Arithmetic::Matrix4x4 m_ViewScreen;
			Arithmetic::Matrix4x4 m_ProjectionScreen;
			Arithmetic::Matrix4x4 m_ViewProjection;
			Arithmetic::Matrix4x4 m_WorldViewProjection;
		};
	}
}

#endif
