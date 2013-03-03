#ifndef __ZED_RENDERER_WINDOW_HPP__
#define __ZED_RENDERER_WINDOW_HPP__

#include <DataTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		typedef struct __ZED_SCREENSIZE
		{
			ZED_UINT32 Width;
			ZED_UINT32 Height;
		}ZED_SCREENSIZE;

		ZED_SCREENSIZE GetNativeScreenSize( );
		ZED_UINT32 EnumerateScreenSizes( ZED_SCREENSIZE **p_ppSizes,
			ZED_MEMSIZE *p_pCount, const ZED_UINT32 p_ScreenNumber );

		class Window
		{
		public:
			virtual ~Window( );

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height ) = 0;
		};
	}
}

#endif

