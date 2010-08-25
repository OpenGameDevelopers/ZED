#ifndef __ZEDRENDERER_HPP__
#define __ZEDRENDERER_HPP__

#include <DataTypes.hpp>
#include <GraphicsAdapter.hpp>
#if ZED_PLATFORM_XBOX
	#include <xtl.h>
#elif( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
	#include <windows.h>
	#include <GL/GL.h>
	#include <wglext.h>
	#include <glext.h>
#endif
#include <RenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		// Forward-declarations
		class CanvasDescription;

		class Renderer
		{
		public:
			// A good ol' superfluous destructor
			virtual ~Renderer( ){ }

			// Return information about the graphics device (name, etc.)
			// Pass in how the device should behave
			virtual ZED_UINT32 Create( GraphicsAdapter *p_Adapter,
				const CanvasDescription &p_Canvas ) = 0;

			// Just in case a clear is needed before BeginScene is called
			virtual void ForceClear( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil ) = 0;

			// Set the clear colour (alpha is 1.0f)
			virtual void ClearColour( ZED_FLOAT32 p_Red, ZED_FLOAT32 p_Green,
				ZED_FLOAT32 p_Blue ) = 0;

			// Optionally clear the colour, depth and stencil when rendering
			virtual ZED_UINT32 BeginScene( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil ) = 0;
			// Really, just call D3DDevice8->Present( NULL, NULL, NULL, NULL )
			// or SwapBuffers( HDC )
			virtual void EndScene( ) = 0;

			// Return true if it's possible to switch, this could prove to be
			// pretty fatal if it messes up
			virtual ZED_BOOL ToggleFullscreen( ) = 0;

			// If the application needs to switch to a larger canvas, check if
			// it's feasible, then perform the operation
			virtual ZED_UINT32 ResizeCanvas( ZED_UINT32 p_Width,
				ZED_UINT32 p_Height ) = 0;

			// Clean up
			virtual void Release( ) = 0;
		};
	}
}

#endif
