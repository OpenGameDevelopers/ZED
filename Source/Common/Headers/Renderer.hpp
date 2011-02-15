#ifndef __ZEDRENDERER_HPP__
#define __ZEDRENDERER_HPP__

#include <DataTypes.hpp>
#include <GraphicsAdapter.hpp>
#include <Vector3.hpp>
#if ZED_PLATFORM_XBOX
	#include <xtl.h>
#elif( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
	#include <windows.h>
	#include <GL/GL.h>
	#include <wglext.h>
	#include <glext.h>
#elif( ZED_PLATFORM_LINUX32_X86 || ZED_PLATFORM_LINUX64_X86 )
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <glxext.h>
	#include <glext.h>
#endif
#include <RenderTypes.hpp>
#include <Plane.hpp>

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

			// Set the world's right, up and direction vectors
			// For example:
			// [ 1 0 0 ] == X Right
			// [ 0 1 0 ] == Y Up
			/*virtual ZED_UINT32 SetView3D(
				const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Direction,
				const Arithmetic::Vector3 &p_Position ) = 0;

			virtual ZED_UINT32 SetViewLookAt(
				const Arithmetic::Vector3 &p_Position,
				const Arithmetic::Vector3 &p_Point,
				const Arithmetic::Vector3 &p_WorldUp )=0;

			// Necessary for the shader pipeline to know what it's using for
			// rendering in VP and WVP
			virtual void CalcViewProjMatrix( )=0;
			virtual void CalcWorldViewProjMatrix( )=0;

			// Set the near and far clipping planes
			virtual void SetClippingPlanes( ZED_FLOAT32 p_Near,
				ZED_FLOAT32 p_Far )=0;

			// Set the mode for the stage
			virtual void SetMode( ZED_UINT32 p_Stage, ZED_UINT32 p_Mode )=0;

			// Set the FOV and viewport for stages
			virtual void InitStage( ZED_FLOAT32 p_FOV,
				const ZED_VIEWPORT &p_Viewport, ZED_UINT32 p_Stage )=0;

			// Get the view frustum (return six planes)
			virtual void GetFrustum( Arithmetic::Plane *p_pPlanes )=0;

			// Transform from 2D to 3D and 3D to 2D
			virtual void Transform2DTo3D( const ZED_POINT &p_Point,
				Arithmetic::Vector3 *p_pOrigin,
				Arithmetic::Vector3 *p_pDirection )=0;
			virtual ZED_POINT Transform3DTo2D(
				const Arithmetic::Vector3 &p_Point )=0;*/

			// Windows-specific functions
#if ( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
			virtual ZED_UINT32 SetHDC( const HDC &p_HDC ) = 0;
#endif
		};
	}
}

#endif

