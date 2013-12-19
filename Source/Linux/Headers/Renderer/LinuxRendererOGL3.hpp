#ifndef __ZED_RENDERER_LINUXRENDEREROGL3_HPP__
#define __ZED_RENDERER_LINUXRENDEREROGL3_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <Renderer/Renderer.hpp>
#include <Arithmetic/Vector3.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLExtender;
		class GLVertexCacheManager;

		class LinuxRendererOGL3 : public Renderer
		{
		public:
			LinuxRendererOGL3( );
			virtual ~LinuxRendererOGL3( );

			virtual ZED_UINT32 Create(
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

			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual void Release( );

			virtual ZED_INLINE ZED_BOOL ShaderSupport( )
				{ return m_ShaderSupport; }

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_pIndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

			virtual void RenderState( const ZED_RENDERSTATE p_State,
				const ZED_UINT32 p_Value );

		private:
			GLExtender					*m_pGLExtender;
			ZED::System::WINDOWDATA		m_WindowData;
			GLXContext					m_GLContext;

			CanvasDescription m_Canvas;
			GLVertexCacheManager *m_pVertexCacheManager;

			ZED_BOOL	m_ShaderSupport;
		};
	}
}

#endif

