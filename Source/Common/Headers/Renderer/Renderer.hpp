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
		ZED_UCHAR8 *FormatToString( const ZED_FORMAT p_Format );

		// Forward-declarations
		class CanvasDescription;

		class Renderer
		{
		public:
			virtual ~Renderer( ){ }

			// Return information about the graphics device (name, etc.)
			// Pass in how the device should behave
			virtual ZED_UINT32 Create(// GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas,
				const ZED::System::Window &p_Window ) = 0;

			virtual ZED_RENDERER_BACKEND BackEnd( ) = 0;

			// Just in case a clear is needed before BeginScene is called
			virtual void ForceClear( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil ) = 0;

			// Set the clear colour (alpha is 1.0f)
			virtual void ClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue ) = 0;

			// Optionally clear the colour, depth and stencil when rendering
			virtual ZED_UINT32 BeginScene( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil ) = 0;

			// Swap the front and back buffers
			virtual void EndScene( ) = 0;

			// If the application needs to switch to a larger canvas, check if
			// it's feasible, then perform the operation
			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height ) = 0;

			virtual ZED_BOOL ShaderSupport( ) = 0;

			// When rendering polygons, call this to render them in an
			// efficient manner
			virtual ZED_UINT32 Render( const ZED_MEMSIZE p_VertexCount,
				const ZED_BYTE *p_pVertices, const ZED_MEMSIZE p_IndexCount,
				const ZED_UINT16 *p_pIndices, const ZED_UINT64 p_Attributes,
				const ZED_UINT32 p_MaterialID,
				const ZED_RENDERPRIMITIVETYPE p_PrimitiveType ) = 0;
			
			virtual void RenderState( const ZED_RENDERSTATE p_State,
				const ZED_UINT32 p_Value ) = 0;
		};
	}
}

#endif

