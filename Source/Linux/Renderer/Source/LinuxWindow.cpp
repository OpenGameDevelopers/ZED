#include <LinuxWindow.hpp>
#include <X11/extensions/Xrandr.h>
#include <Renderer.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_UINT32 GetNativeScreenSize( const ZED_UINT32 p_ScreenNumber,
			ZED_SCREENSIZE &p_ScreenSize )
		{
			ZED_UINT32 ScreenCount = 0;
			GetScreenCount( &ScreenCount );

			if( ( p_ScreenNumber < 0 ) ||
				( p_ScreenNumber > ( ScreenCount-1 ) ) )
			{
				zedTrace( "[ZED::Renderer::GetNativeScreenSize] <ERROR> "
					"Screen count %s than what is available\n",
					( p_ScreenNumber < 0 ) ? "less" : "more" );
			}

			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetNativeScreenSize] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			p_ScreenSize.Width = DisplayWidth( pDisplay, p_ScreenNumber );
			p_ScreenSize.Height = DisplayHeight( pDisplay, p_ScreenNumber );

			XCloseDisplay( pDisplay );

			return ZED_OK;
		}

		ZED_UINT32 GetScreenCount( ZED_UINT32 *p_pScreenCount )
		{
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				( *p_pScreenCount ) = 0;
				
				zedTrace( "[ZED::Renderer::GetScreenCount] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			ZED_UINT32 ScreenCount = ScreenCount( pDisplay );
			XCloseDisplay( pDisplay );

			( *p_pScreenCount ) = ScreenCount;

			return ZED_OK;
		}

		ZED_SCREEN_ORIENTATION GetScreenOrientation(
			const ZED_UINT32 p_ScreenNumber )
		{
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetScreenOrientation] <ERROR> "
					"Could not open display\n" );

				return ZED_SCREEN_ORIENTATION_0;
			}

			ZED_SCREEN_ORIENTATION Orientation = ZED_SCREEN_ORIENTATION_0;
			
			Rotation CurrentRotation;
			XRRRotations( pDisplay, p_ScreenNumber, &CurrentRotation );

			if( CurrentRotation == RR_Rotate_0 )
			{
				Orientation = ZED_SCREEN_ORIENTATION_0;
			}
			if( CurrentRotation == RR_Rotate_90 )
			{
				Orientation = ZED_SCREEN_ORIENTATION_90;
			}
			if( CurrentRotation == RR_Rotate_180 )
			{
				Orientation = ZED_SCREEN_ORIENTATION_180;
			}
			if( CurrentRotation == RR_Rotate_270 )
			{
				Orientation = ZED_SCREEN_ORIENTATION_270;
			}

			return Orientation;
		}

		ZED_UINT32 EnumerateScreenSizes( ZED_SCREENSIZE **p_ppSizes,
			ZED_MEMSIZE *p_pCount, const ZED_UINT32 p_ScreenNumber )
		{
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::EnumerateScreenSizes] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			ZED_SINT32 TotalSizes = 0;
			XRRScreenSize *pScreenSize = XRRSizes( pDisplay, p_ScreenNumber,
				&TotalSizes );
			XCloseDisplay( pDisplay );

			zedTrace( "[ZED::Renderer::EnumerateScreenSizes] <INFO> "
				"Found %d resolutions\n", TotalSizes );

			( *p_ppSizes ) = new ZED_SCREENSIZE[ TotalSizes ];

			for( ZED_SINT32 i = 0; i < TotalSizes; ++i )
			{
				zedTrace( "\t%d: %dx%d\n",
					i, pScreenSize[ i ].width, pScreenSize[ i ].height );

				( *p_ppSizes )[ i ].Width = pScreenSize[ i ].width;
				( *p_ppSizes )[ i ].Height = pScreenSize[ i ].height;
			}

			( *p_pCount ) = TotalSizes;
			
			return ZED_OK;
		}

		ZED_UINT32 GetCurrentScreenNumber( )
		{
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetCurrentScreenNumber] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			ZED_UINT32 ScreenNumber = DefaultScreen( pDisplay );

			XCloseDisplay( pDisplay );

			return ScreenNumber;
		}

		ZED_SCREEN_ORIENTATION GetCurrentScreenOrientation( )
		{
			return GetScreenOrientation( GetCurrentScreenNumber( ) );
		}

		LinuxWindow::LinuxWindow( )
		{
			m_pDisplay = ZED_NULL;
			m_pVisualInfo = ZED_NULL;
			m_CursorHidden = ZED_TRUE;
		}

		LinuxWindow::~LinuxWindow( )
		{
			if( m_CursorHidden )
			{
				this->ShowCursor( );
			}

			this->Destroy( );
		}

		ZED_UINT32 LinuxWindow::Create( const ZED_UINT32 p_X,
			const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			m_pDisplay = XOpenDisplay( 0 );

			// Is there a way to detach this?
			int VisualAttribs[ ] =
			{
				GLX_X_RENDERABLE,	True,
				GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
				GLX_RENDER_TYPE,	GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
				GLX_RED_SIZE,		8,
				GLX_GREEN_SIZE,		8,
				GLX_BLUE_SIZE,		8,
				GLX_ALPHA_SIZE,		8,
				GLX_DEPTH_SIZE,		24,
				GLX_STENCIL_SIZE,	8,
				GLX_DOUBLEBUFFER,	True,
				None
			};

			ZED_SINT32	GLXMajor = 0, GLXMinor = 0;

			if( !glXQueryVersion( m_pDisplay, &GLXMajor, &GLXMinor ) ||
				( ( GLXMajor == 1 ) && ( GLXMinor < 3 ) ) || GLXMajor < 1 )
			{
				return ZED_FAIL;
			}

			ZED_SINT32 FBCount = 0;
			GLXFBConfig *pFBC = glXChooseFBConfig( m_pDisplay,
				DefaultScreen( m_pDisplay ), VisualAttribs, &FBCount );

			if( !pFBC )
			{
				return ZED_FAIL;
			}

			// For now, just use the first one found
			GLXFBConfig GLFBConfig = pFBC[ 0 ];

			XFree( pFBC );
			pFBC = ZED_NULL;

			m_pVisualInfo = glXGetVisualFromFBConfig( m_pDisplay,
				GLFBConfig );

			XSetWindowAttributes WinAttribs;

			WinAttribs.colormap = XCreateColormap( m_pDisplay,
				RootWindow( m_pDisplay, m_pVisualInfo->screen ),
				m_pVisualInfo->visual, AllocNone );
			WinAttribs.background_pixmap = None;
			WinAttribs.border_pixel = 0;
			WinAttribs.event_mask = StructureNotifyMask | ExposureMask |
				KeyPressMask | KeyPressMask |
				ButtonPressMask | ButtonReleaseMask |
				ResizeRedirectMask | PointerMotionMask |
				FocusChangeMask | EnterWindowMask | LeaveWindowMask;
			WinAttribs.override_redirect = True;

			m_Window = XCreateWindow( m_pDisplay,
				RootWindow( m_pDisplay, m_pVisualInfo->screen ),
				p_X, p_Y, p_Width, p_Height, 0,
				m_pVisualInfo->depth, InputOutput,
				m_pVisualInfo->visual,
				CWEventMask | CWColormap | CWBorderPixel |
				CWOverrideRedirect, &WinAttribs );
			
			XMapWindow( m_pDisplay, m_Window );
			XMapRaised( m_pDisplay, m_Window );
			XMoveWindow( m_pDisplay, m_Window, p_X, p_Y );
			XRaiseWindow( m_pDisplay, m_Window );

			m_WindowData.X11Window = m_Window;
			m_WindowData.pX11Display = m_pDisplay;
			m_WindowData.pX11VisualInfo = m_pVisualInfo;
			m_WindowData.X11GLXFBConfig = GLFBConfig;

			return ZED_OK;
		}

		void LinuxWindow::Destroy( )
		{
			if( m_pVisualInfo )
			{
				XFree( m_pVisualInfo );
			}
			if( m_Window )
			{
				XDestroyWindow( m_pDisplay, m_Window );
			}

			if( m_pDisplay )
			{
				XCloseDisplay( m_pDisplay );
				m_pDisplay = ZED_NULL;
			}
		}

		ZED_UINT32 LinuxWindow::Update( )
		{
			return ZED_OK;
		}

		void LinuxWindow::HideCursor( )
		{
			if( m_pDisplay == ZED_NULL )
			{
				return;
			}

			XDefineCursor( m_pDisplay, m_Window, this->NullCursor( ) );
		}

		void LinuxWindow::ShowCursor( )
		{
			if( m_pDisplay == ZED_NULL )
			{
				return;
			}

			XUndefineCursor( m_pDisplay, m_Window );
		}

		ZED_BOOL LinuxWindow::ToggleCursor( )
		{
			if( m_CursorHidden )
			{
				this->ShowCursor( );
			}
			else
			{
				this->HideCursor( );
			}

			m_CursorHidden = !m_CursorHidden;

			return ( m_CursorHidden != ZED_TRUE );
		}

		Cursor LinuxWindow::NullCursor( )
		{
			Pixmap CursorMask;
			XGCValues XGC;
			GC GraphicsContext;
			XColor DummyColour;
			Cursor ReturnCursor;

			CursorMask = XCreatePixmap( m_pDisplay, m_Window, 1, 1, 1 );
			XGC.function = GXclear;
			GraphicsContext = XCreateGC( m_pDisplay, CursorMask, GCFunction,
				&XGC );
			XFillRectangle( m_pDisplay, CursorMask, GraphicsContext, 0, 0, 1,
				1 );
			DummyColour.pixel = 0;
			DummyColour.red = 0;
			DummyColour.flags = 4;
			ReturnCursor = XCreatePixmapCursor( m_pDisplay, CursorMask,
				CursorMask, &DummyColour, &DummyColour, 0, 0 );
			XFreePixmap( m_pDisplay, CursorMask );
			XFreeGC( m_pDisplay, GraphicsContext );

			m_CursorHidden = ZED_TRUE;

			return ReturnCursor;
		}
	}
}

