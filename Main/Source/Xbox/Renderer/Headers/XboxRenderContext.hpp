#ifndef __ZEDXBOXRENDERCONTEXT_HPP__
#define __ZEDXBOXRENDERCONTEXT_HPP__

#include <RenderContext.hpp>
#include <xtl.h>

namespace ZED
{
	namespace Renderer
	{
		class XboxRenderContext : public RenderContext
		{
		public:
			ZED_UINT32 CreateDevice( const GraphicsAdapter &Adapter,
				const CanvasDescription &Canvas, RenderDevice *pDevice );

		private:
		};
	}
}

#endif
