/*#ifndef __ZED_RENDERER_WINDOWSRENDERERD3D10_HPP__
#define __ZED_RENDERER_WINDOWSRENDERERD3D10_HPP__

#include <Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		class WindowsRendererD3D10 : public Renderer
		{
		public:
			WindowsRendererD3D10( );
			virtual ~WindowsRendererD3D10( );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas );

			// As this is Windows, a device context handle is required
			// for setup
			ZED_UINT32 Create( GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas, const HDC &p_HDC );

			virtual void ForceClear( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual ZED_UINT32 BeginScene( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual void EndScene( );

			virtual void ClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue );

			virtual ZED_BOOL ToggleFullscreen( );

			virtual ZED_BOOL ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual void Release( );

			virtual void SetView3D( const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Direction,
				const Arithmetic::Vector3 &p_Position );

			// Windows-specific functions
			ZED_UINT32 SetHDC( const HDC &p_HDC );

		private:
		};
	}
}

#endif
*/