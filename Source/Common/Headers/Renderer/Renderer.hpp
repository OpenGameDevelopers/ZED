#ifndef __ZED_RENDERER_RENDERER_HPP__
#define __ZED_RENDERER_RENDERER_HPP__

#include <System/DataTypes.hpp>
#include <System/GraphicsAdapter.hpp>
#include <Arithmetic/Vector3.hpp>
#if defined ZED_PLATFORM_XBOX
	#include <xtl.h>
#elif defined ZED_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <GL/gl.h>
	#include <GL/wglext.h>
	#include <GL/glext.h>
#elif defined ZED_WINDOWSYSTEM_X11
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
#if defined ZED_PLATFORM_LINUX
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glxext.h>
	#include <GL/glext.h>
#elif defined ZED_PLATFORM_PANDORA_LINUX
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES2/gl2extimg.h>
	#include <EGL/egl.h>
#else
	#error Unknown platform
#endif // ZED_PLATFORM_LINUX
#endif // ZED_PLATFORM_XBOX
#include <Renderer/RenderTypes.hpp>
#include <Arithmetic/Plane.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <System/Window.hpp>

/**
	\namespace ZED
	\brief The root namespace
*/
namespace ZED
{
	/**
		\namespace ZED::Renderer
		\brief Contains the rendering related classes, structures, functions,
		and constants
	*/
	namespace Renderer
	{
		ZED_MEMSIZE GetBPP( const ZED_FORMAT p_Format );

		// Forward-declarations
		class CanvasDescription;
		class Material;

		class Renderer
		{
		public:
			virtual ~Renderer( ){ }

			// Return information about the graphics device (name, etc.)
			// Pass in how the device should behave
			virtual ZED_UINT32 Create(
				const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window ) = 0;

			// Clear the back buffer
			virtual void Clear( ) = 0;

			// Swap the front and back buffers
			virtual void SwapBuffers( ) = 0;

			// Set the clear colour (alpha is 1.0f)
			virtual void SetClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue ) = 0;

			// Set the flags to be used when calling Clear( )
			virtual void SetClearFlags( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil ) = 0;

			// If the application needs to switch to a larger canvas, check if
			// it's feasible, then perform the operation
			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height ) = 0;

			// When rendering polygons, call this to render them in an
			// efficient manner
			virtual ZED_UINT32 CreateMesh( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const /* VertexAttributes */ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID /* This should not exist when creating a mesh */,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType
				/*, Mesh **p_ppMesh */ ) = 0;

			virtual ZED_UINT32 Screenshot( const ZED_CHAR8 *p_pFileName,
				const ZED_BOOL p_RelativeToExecutable ) = 0;

			virtual void SetRenderState( const ZED_RENDERSTATE p_State,
				const ZED_UINT32 p_Value ) = 0;

			/* The renderer should provide the ability to create materials, not
			 * add them */
			virtual ZED_UINT32 AddMaterial(
				ZED::Renderer::Material * const &p_pMaterial ) = 0;

			virtual ZED_UINT32 GetMaterial( const ZED_UINT32 p_MaterialID,
				ZED::Renderer::Material *p_pMaterial ) const = 0;

			virtual ZED_UINT32 GetMaterial( ZED_CHAR8 * const &p_pMaterialName,
				ZED::Renderer::Material *p_pMaterial ) const = 0;

			// Return the specific API used for graphics acceleration (i.e.
			// OpenGL, Direct3D, Vulkan)
			ZED_RENDERER_BACKEND GetBackEnd( ) const;

			// Returns whether the renderer supports shaders (can be either
			// assembly or some form of high-level language)
			ZED_BOOL GetShaderSupport( ) const;

		protected:
			ZED_BOOL				m_ShaderSupport;
			ZED_RENDERER_BACKEND	m_BackEnd;
		};
	}
}

#endif // __ZED_RENDERER_RENDERER_HPP__

