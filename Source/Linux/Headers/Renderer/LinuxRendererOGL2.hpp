#ifndef __ZED_RENDERER_LINUXRENDEREROGL2_HPP__
#define __ZED_RENDERER_LINUXRENDEREROGL2_HPP__

#include <System/DataTypes.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/CanvasDescription.hpp>

namespace ZED
{
	namespace System
	{
		class LinuxWindowData;
	}

	namespace Renderer
	{
		class LinuxGLExtender;
		class GLVertexCacheManager;
		class MaterialManager;

		class LinuxRendererOGL2 : public Renderer
		{
		public:
			LinuxRendererOGL2( );
			virtual ~LinuxRendererOGL2( );

			virtual ZED_UINT32 Create(
				const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window );

			virtual ZED_INLINE ZED_RENDERER_BACKEND GetBackEnd( ) const
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

			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

			virtual ZED_UINT32 Screenshot( const ZED_CHAR8 *p_pFileName,
				const ZED_BOOL p_RelativeToExecutable );

			virtual void RenderState( const ZED_RENDERSTATE p_State,
				const ZED_UINT32 p_Value );

			virtual ZED_UINT32 AddMaterial(
				ZED::Renderer::Material * const &p_pMaterial );

			virtual ZED_UINT32 GetMaterial( const ZED_UINT32 p_MaterialID,
				ZED::Renderer::Material *p_pMaterial ) const;

			virtual ZED_UINT32 GetMaterial( ZED_CHAR8 * const &p_pMaterialName,
				ZED::Renderer::Material *p_pMaterial ) const;

		private:
			LinuxGLExtender					*m_pGLExtender;
			ZED::System::LinuxWindowData	*m_pWindowData;
			GLXContext						m_GLContext;

			CanvasDescription		m_Canvas;
			GLVertexCacheManager	*m_pVertexCacheManager;

			MaterialManager		*m_pMaterialManager;

			GLuint		m_BackbufferID;
			ZED_BOOL	m_TakeScreenshot;
			ZED_CHAR8	*m_pScreenshotFileName;
		};
	}
}

#endif // __ZED_RENDERER_LINUXRENDEREROGL2_HPP__

