#ifndef __ZEDXBOXRENDERER_HPP__
#define __ZEDXBOXRENDERER_HPP__

#include <DataTypes.hpp>
#include <CanvasDescription.hpp>
#include <Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		class XboxRenderer : public Renderer
		{
		public:
			XboxRenderer( );
			~XboxRenderer( );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_Adapter,
				const CanvasDescription &p_Canvas );

			virtual void ForceClear( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil );

			virtual void ClearColour( ZED_FLOAT32 p_Red, ZED_FLOAT32 p_Green,
				ZED_FLOAT32 p_Blue );

			virtual ZED_UINT32 BeginScene( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
				ZED_BOOL p_Stencil );

			virtual void EndScene( );

			virtual ZED_BOOL ToggleFullscreen( );

			virtual ZED_UINT32 ResizeCanvas( ZED_UINT32 p_Width,
				ZED_UINT32 p_Height );

			// Clean up
			virtual void Release( );

		private:
			CanvasDescription		m_Canvas;
			LPDIRECT3DDEVICE8		m_pDevice;
			LPDIRECT3D8				m_pD3D;
			D3DPRESENT_PARAMETERS	m_PresentParams;
			D3DCOLOR				m_Colour;

			// Width, height and aspect ratio are always handy to have around
			ZED_FLOAT32 m_AspectRatio;
			ZED_UINT32 m_Width;
			ZED_UINT32 m_Height;
		};
	}
}

#endif
