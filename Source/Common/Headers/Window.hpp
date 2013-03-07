#ifndef __ZED_RENDERER_WINDOW_HPP__
#define __ZED_RENDERER_WINDOW_HPP__

#include <DataTypes.hpp>
#ifdef ZED_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#endif

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

#ifdef ZED_PLATFORM_LINUX
		typedef struct __ZED_WINDOWDATA
		{
			XVisualInfo	*pX11VisualInfo;
			Display		*pX11Display;
			GLXFBConfig	X11GLXFBConfig;
			::Window	X11Window;
		}ZED_WINDOWDATA;
#endif

		class Window
		{
		public:
			virtual ~Window( ){ }

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height ) = 0;
			
			virtual void Destroy( ) = 0;

			virtual ZED_WINDOWDATA WindowData( ) const = 0;
		};
	}
}

#endif

