#ifndef __ZED_RENDERER__LINUXRENDEREROGL3_HPP__
#define __ZED_RENDERER__LINUXRENDEREROGL3_HPP__

#include <DataTypes.hpp>
#include <GLXExtender.hpp>
#include <CanvasDescription.hpp>
#include <Renderer.hpp>
#include <Vector3.hpp>

namespace ZED
{
	namespace Renderer
	{
		class LinuxRendererOGL3 : public Renderer
		{
		public:
			LinuxRendererOGL3( );
			virtual ~LinuxRendererOGL3( );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas );
			virtual void ForceClear( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

		private:
		};
	}
}

#endif

