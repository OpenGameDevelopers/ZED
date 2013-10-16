#include <Renderer/PandoraLinuxRenderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		PandoraLinuxRenderer::PandoraLinuxRenderer( )
		{
		}

		PandoraLinuxRenderer::~PandoraLinuxRenderer( )
		{
		}

		ZED_UINT32 PandoraLinuxRenderer::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			return ZED_OK;
		}

		void PandoraLinuxRenderer::ForceClear( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
		}

		void PandoraLinuxRenderer::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
		}

		ZED_UINT32 PandoraLinuxRenderer::BeginScene( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
		}

		void PandoraLinuxRenderer::EndScene( )
		{
		}
		
		ZED_UINT32 PandoraLinuxRenderer::ResizeCanvas(
			const ZED_UINT32 p_Width, const ZED_UINT32 p_Height )
		{
			return ZED_OK;
		}

	}
}

