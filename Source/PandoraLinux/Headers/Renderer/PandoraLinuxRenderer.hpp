#ifndef __ZED_RENDERER_PANDORALINUXRENDERER_HPP__
#define __ZED_RENDERER_PANDORALINUXRENDERER_HPP__

#include <Renderer/Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		class PandoraLinuxRenderer : public Renderer
		{
		public:
			PandoraLinuxRenderer( );
			virtual ~PandoraLinuxRenderer( );

			virtual ZED_UINT32 Create( const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window );

			ZED_INLINE virtual ZED_RENDERER_BACKEND BackEnd( )
				{ return ZED_RENDERER_BACKEND_OPENGLES2; }

			virtual void ForceClear( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual void ClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue );

			virtual ZED_UINT32 BeginScene( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual void EndScene( );

			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual ZED_BOOL ShaderSupport( );

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

			virtual void RenderState( const ZED_RENDERSTATE p_State,
				const ZED_UINT32 p_Value );

		private:
		};
	}
}

#endif

