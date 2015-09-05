#ifndef __ZED_RENDERER_LINUXRENDEREROPENGL_HPP__
#define __ZED_RENDERER_LINUXRENDEREROPENGL_HPP__

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
		// OpenGLVertexCacheManager
		class GLVertexCacheManager;
		class MaterialManager;

		class LinuxRendererOpenGL : public Renderer
		{
		public:
			ZED_EXPLICIT LinuxRendererOpenGL( const ZED_SINT32 p_MajorVersion,
				const ZED_SINT32 p_MinorVersion );
			virtual ~LinuxRendererOpenGL( );

			virtual ZED_UINT32 Create(
				const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window );

			virtual void Clear( );

			virtual void SwapBuffers( );

			virtual void SetClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue );

			virtual void SetClearFlags( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual ZED_UINT32 CreateMesh( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices,
				const ZED_UINT64 p_VertexAttributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType );

			virtual ZED_UINT32 Screenshot( const ZED_CHAR8 *p_pFileName,
				const ZED_BOOL p_RelativeToExecutable );

			virtual void SetRenderState( const ZED_RENDERSTATE p_State,
				const ZED_UINT32 p_Value );

			virtual ZED_UINT32 AddMaterial( Material * const &p_pMaterial );

			virtual ZED_UINT32 GetMaterial( const ZED_UINT32 p_MaterialID,
				Material *p_pMaterial ) const;

			virtual ZED_UINT32 GetMaterial( ZED_CHAR8 * const &p_pMaterialName,
				Material *p_pMaterial ) const;

		private:
			LinuxRendererOpenGL( );
			LinuxRendererOpenGL( const LinuxRendererOpenGL &p_Other );
			LinuxRendererOpenGL &operator=(
				const LinuxRendererOpenGL &p_Other );

			LinuxGLExtender					*m_pGLExtender;
			ZED::System::LinuxWindowData	*m_pWindowData;
			GLXContext						m_GLXContext;
			GLbitfield						m_ClearFlags;
			CanvasDescription				m_Canvas;
			GLVertexCacheManager			*m_pVertexCacheManager;
			MaterialManager					*m_pMaterialManager;
			GLuint							m_BackBufferID;
			ZED_BOOL						m_TakeScreenshot;
			ZED_CHAR8						*m_pScreenshotFileName;
			ZED_SINT32						m_MajorVersion;
			ZED_SINT32						m_MinorVersion;
		};
	}
}

#endif // __ZED_RENDERER_LINUXRENDEREROPENGL_HPP__

