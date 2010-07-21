#ifndef __ZEDWINDOWSRENDERER_HPP__
#define __ZEDWINDOWSRENDERER_HPP__

#include <DataTypes.hpp>
#include <GLExtender.hpp>
#include <Renderer.hpp>
#include <CanvasDescription.hpp>

namespace ZED
{
	namespace Renderer
	{
		class WindowsRenderer : public Renderer
		{
		public:
			WindowsRenderer( );
			~WindowsRenderer( );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_Adapter,
				const CanvasDescription &p_Canvas );

			// Provide an alternative to allow the HDC to be set in Create
			ZED_UINT32 Create( GraphicsAdapter *p_Adapter,
				const CanvasDescription &p_Canvas, const HDC &p_HDC );

			virtual void ForceClear( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil );
			virtual ZED_UINT32 BeginScene( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil );
			virtual void EndScene( );

			virtual void ClearColour( ZED_FLOAT32 p_Red, ZED_FLOAT32 p_Green,
				ZED_FLOAT32 p_Blue );

			virtual ZED_BOOL ToggleFullscreen( );
			virtual ZED_BOOL ResizeCanvas( ZED_UINT32 p_Width,
				ZED_UINT32 p_Height );

			virtual void Release( );

			// Windows-specific functions
			ZED_UINT32 SetHDC( const HDC &p_HDC );

		private:
			PIXELFORMATDESCRIPTOR m_PixelFormat;
			HGLRC m_HGLRC;
			HDC m_HDC;
			GLExtender m_Ext;
			CanvasDescription m_Canvas;
			ZED_FLOAT32 m_AspectRatio;
		};
	}
}

#endif
