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

			virtual void SetClippingPlanes( const ZED_FLOAT32 p_Near,
				const ZED_FLOAT32 p_Far );

			virtual void Prepare2D( );

			virtual ZED_UINT32 CalcPerspProjMatrix( const ZED_FLOAT32 p_FOV,
				const ZED_FLOAT32 p_AspectRatio,
				Arithmetic::Matrix4x4 *p_pMatrix );

			virtual ZED_UINT32 PerspectiveProjectionMatrix(
				const ZED_FLOAT32 p_FOV, const ZED_FLOAT32 p_AspectRatio );

			virtual void PerspectiveProjectionMatrix(
				Arithmetic::Matrix4x4 *p_pMatrix );

			virtual ZED_BOOL ShaderSupport( );

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

			virtual void SetRenderState( const ZED_RENDERSTATE p_State,
				const ZED_MEMSIZE p_Value );

			virtual void GetWVP( Arithmetic::Matrix4x4 *p_pMatrix );
			virtual void GetVP( Arithmetic::Matrix4x4 *p_pMatrix );
		private:
		};
	}
}

#endif

