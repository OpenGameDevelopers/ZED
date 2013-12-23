#ifndef __ZED_RENDERER_WINDOWSRENDEREROGL3_HPP__
#define __ZED_RENDERER_WINDOWSRENDEREROGL3_HPP__

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
		class WindowsRendererOGL3 : public Renderer
		{
		public:
			WindowsRendererOGL3( );
			virtual ~WindowsRendererOGL3( );

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
			virtual ZED_BOOL ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual void Release( );

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID );

			virtual void RenderState( const ZED_RENDERSTATE p_State,
				const ZED_MEMSIZE p_Value );

			// Return the version of OpenGL currently in use
			ZED_GLVERSION GetGLVersion( ) const { return m_GLVersion; }

		private:
			PIXELFORMATDESCRIPTOR	m_PixelFormat;
			HGLRC					m_GLRenderContext;
			HDC						m_DeviceContext;
			GLExtender				m_GLExtender;
			CanvasDescription		m_Canvas;
			ZED_GLVERSION			m_GLVersion;

			// If the graphics engine winds down, this will be set to false so
			// further sub-systems can clean up
			ZED_BOOL m_Running;
			
			// Vertex Cache Manager
			GLVertexCacheManager *m_pVertexCacheManager;
		};
	}
}

#endif // __ZED_RENDERER_WINDOWSRENDEREROGL3_HPP__

