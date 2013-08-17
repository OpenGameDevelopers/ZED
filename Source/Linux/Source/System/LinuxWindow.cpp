#include <System/LinuxWindow.hpp>
#include <X11/extensions/Xrandr.h>
#include <Renderer/Renderer.hpp>
#include <System/Debugger.hpp>
#include <cstring>

#define MWM_HINTS_FUNCTIONS		( 1L << 0 )
#define MWM_HINTS_DECORATIONS	( 1L << 1 )
#define MWM_HINTS_INPUT_MODE	( 1L << 2 )
#define MWM_HINTS_STATUS		( 1L << 3 )

#define MWM_FUNC_NONE		0L
#define MWM_FUNC_ALL		( 1L << 0 )
#define MWM_FUNC_RESIZE		( 1L << 1 )
#define MWM_FUNC_MOVE		( 1L << 2 )
#define MWM_FUNC_MINIMISE	( 1L << 3 )
#define MWM_FUNC_MAXIMISE	( 1L << 4 )
#define MWM_FUNC_CLOSE		( 1L << 5 )

#define MWM_DECOR_NONE		0L
#define MWM_DECOR_ALL		( 1L << 0 )
#define MWM_DECOR_BORDER	( 1L << 1 )
#define MWM_DECOR_RESIZEH	( 1L << 2 )
#define MWM_DECOR_TITLE		( 1L << 3 )
#define MWM_DECOR_MENU		( 1L << 4 )
#define MWM_DECOR_MINIMISE	( 1L << 5 )
#define MWM_DECOR_MAXIMISE	( 1L << 6 )

namespace ZED
{
	namespace System
	{
		ZED_UINT32 GetNativeScreenSize( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber,
			ZED_SCREENSIZE &p_ScreenSize )
		{
			ZED_UINT32 ScreenCount = 0;
			GetScreenCount( p_DisplayNumber, &ScreenCount );

			if( ( p_ScreenNumber < 0 ) ||
				( p_ScreenNumber > ( ScreenCount-1 ) ) )
			{
				zedTrace( "[ZED::Renderer::GetNativeScreenSize] <ERROR> "
					"Screen count %s than what is available\n",
					( p_ScreenNumber < 0 ) ? "less" : "more" );
			}

			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

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

		ZED_UINT32 GetScreenCount( const ZED_UINT32 p_DisplayNumber,
			ZED_UINT32 *p_pScreenCount )
		{
			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

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
			const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber )
		{
			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

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
			ZED_MEMSIZE *p_pCount, const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber )
		{
			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

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
			const ZED_UINT32 ScreenNumber = GetCurrentScreenNumber( );
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetScreenOrientation] <ERROR> "
					"Could not open display\n" );

				return ZED_SCREEN_ORIENTATION_0;
			}

			ZED_SCREEN_ORIENTATION Orientation = ZED_SCREEN_ORIENTATION_0;
			
			Rotation CurrentRotation;
			XRRRotations( pDisplay, ScreenNumber, &CurrentRotation );

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

		LinuxWindow::LinuxWindow( )
		{
			m_pDisplay = ZED_NULL;
			m_pVisualInfo = ZED_NULL;
			m_CursorHidden = ZED_FALSE;
			m_FullScreen = ZED_FALSE;
			m_X = m_Y = m_Width = m_Height = 0;
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
			const ZED_UINT32 p_Height, const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber, const ZED_UINT32 p_Style )
		{
			m_Running = ZED_FALSE;

/*			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			m_pDisplay = XOpenDisplay( pDisplayNumber );
*/
			m_pDisplay = XOpenDisplay( ZED_NULL );

			if( m_pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxWindow::Create] <ERROR> "
					"Could not open display\n" );
				return ZED_FAIL;
			}

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

			if( p_Style == ZED_WINDOW_STYLE_FULLSCREEN )
			{
				Atom State = XInternAtom( m_pDisplay, "_NET_WM_STATE", False );
				Atom Fullscreen = XInternAtom( m_pDisplay,
					"_NET_WM_STATE_FULLSCREEN", False );

				XEvent Event;
				memset( &Event, 0, sizeof( Event ) );

				Event.type = ClientMessage;
				Event.xclient.window		= m_Window;
				Event.xclient.message_type	= State;
				Event.xclient.format		= 32;
				Event.xclient.data.l[ 0 ]	= 1;
				Event.xclient.data.l[ 1 ]	= Fullscreen;

				XSendEvent( m_pDisplay, DefaultRootWindow( m_pDisplay ),
					False, SubstructureNotifyMask, &Event );

				ZED_SCREENSIZE NativeSize;
				GetNativeScreenSize( p_DisplayNumber, p_ScreenNumber,
					NativeSize );
				m_X = 0;
				m_Y = 0;
				m_Width = NativeSize.Width;
				m_Height = NativeSize.Height;
			}
			else
			{
				m_X = p_X;
				m_Y = p_Y;
				m_Width = p_Width;
				m_Height = p_Height;
			}

			m_Window = XCreateWindow( m_pDisplay,
				RootWindow( m_pDisplay, m_pVisualInfo->screen ),
				m_X, m_Y, m_Width, m_Height, 0,
				m_pVisualInfo->depth, InputOutput,
				m_pVisualInfo->visual,
				CWEventMask | CWColormap | CWBorderPixel, &WinAttribs );
		
			m_WindowData.X11Window = m_Window;
			m_WindowData.pX11Display = m_pDisplay;
			m_WindowData.pX11VisualInfo = m_pVisualInfo;
			m_WindowData.X11GLXFBConfig = GLFBConfig;

			Atom DeleteMessage = XInternAtom( m_pDisplay, "WM_DELETE_WINDOW",
				False );
			XSetWMProtocols( m_pDisplay, m_Window, &DeleteMessage, 1 );

			Atom Property = XInternAtom( m_pDisplay, "_MOTIF_WM_HINTS",
				False );

			if( Property )
			{
				typedef struct __tagHints
				{
					unsigned long	Flags;
					unsigned long	Functions;
					unsigned long	Decorations;
					long			InputMode;
					unsigned long	State;
				} Hints;

				Hints WindowDecor;
				WindowDecor.Flags = MWM_HINTS_FUNCTIONS |
					MWM_HINTS_DECORATIONS;
				WindowDecor.Functions = 0L;
				WindowDecor.Decorations = 0L;

				zedTrace( "Style: %d\n", p_Style );

				if( ( p_Style & ZED_WINDOW_STYLE_FULLSCREEN ) ||
					( p_Style & ZED_WINDOW_STYLE_NONE ) )
				{
					WindowDecor.Functions |= MWM_FUNC_NONE;
					WindowDecor.Decorations |= MWM_DECOR_NONE;
				}
				else if( p_Style == ZED_WINDOW_STYLE_ALL )
				{
					WindowDecor.Functions |= MWM_FUNC_ALL;
					WindowDecor.Decorations |= MWM_DECOR_ALL;
				}
				else
				{
					if( p_Style & ZED_WINDOW_STYLE_CLOSE )
					{
						WindowDecor.Functions |= MWM_FUNC_CLOSE;
					}

					if( p_Style & ZED_WINDOW_STYLE_MINIMISE )
					{
						WindowDecor.Functions |= MWM_FUNC_MINIMISE;
						WindowDecor.Decorations |= MWM_DECOR_MINIMISE;
					}

					if( p_Style & ZED_WINDOW_STYLE_MAXIMISE )
					{
						WindowDecor.Functions |= MWM_FUNC_MAXIMISE;
						WindowDecor.Decorations |= MWM_DECOR_MAXIMISE;
					}

					if( p_Style & ZED_WINDOW_STYLE_RESIZE )
					{
						WindowDecor.Functions |= MWM_FUNC_RESIZE;
						WindowDecor.Decorations |= MWM_DECOR_RESIZEH;
					}

					if( p_Style & ZED_WINDOW_STYLE_TITLEBAR )
					{
						WindowDecor.Decorations |= MWM_DECOR_TITLE;
					}

					if( p_Style & ZED_WINDOW_STYLE_MENU )
					{
						WindowDecor.Decorations |= MWM_DECOR_MENU;
					}
					
					if( p_Style & ZED_WINDOW_STYLE_BORDER )
					{
						WindowDecor.Decorations |= MWM_DECOR_BORDER;
					}

					if( p_Style & ZED_WINDOW_STYLE_MOVE )
					{
						WindowDecor.Functions |= MWM_FUNC_MOVE;
					}
				}
				
				XChangeProperty( m_pDisplay, m_Window, Property, Property, 32,
					PropModeReplace, ( unsigned char *)&WindowDecor, 5 );

				// Kind of an inelegant way to disable window re-sizing
				if( !( p_Style & ZED_WINDOW_STYLE_RESIZE ) )
				{
					XSizeHints *pSizeHints = XAllocSizeHints( );
					pSizeHints->flags = PMinSize | PMaxSize;
					pSizeHints->min_width = m_Width;
					pSizeHints->min_height = m_Height;
					pSizeHints->max_width = m_Width;
					pSizeHints->max_height = m_Height;

					XSetWMNormalHints( m_pDisplay, m_Window, pSizeHints );

					XFree( pSizeHints );
				}
			}
			else
			{
				zedTrace( "[ZED::LinuxWindow::Create] <WARNING> "
					"Could not acquire the property: \"_MOTIF_WM_HINTS\"\n" );
			}

			XMapWindow( m_pDisplay, m_Window );
			XMapRaised( m_pDisplay, m_Window );
			XMoveWindow( m_pDisplay, m_Window, p_X, p_Y );
			XRaiseWindow( m_pDisplay, m_Window );

			m_Running = ZED_TRUE;

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
			XEvent Event;

			int Pending = XPending( m_pDisplay );

			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_pDisplay, &Event );
				switch( Event.type )
				{
					case ClientMessage:
					{
						if( *XGetAtomName( m_pDisplay,
							Event.xclient.message_type ) == *"WM_PROTOCOLS" )
						{
							m_Running = ZED_FALSE;
						}
						break;
					}
					default:
					{
						XPutBackEvent( m_pDisplay, &Event );
						break;
					}
				}
			}
			return ZED_OK;
		}

		void LinuxWindow::Title( const char *p_pTitle )
		{
			XStoreName( m_pDisplay, m_Window, p_pTitle );
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

		void LinuxWindow::SetWindowed( )
		{
		}

		void LinuxWindow::SetFullScreen( )
		{
		}

		ZED_BOOL LinuxWindow::ToggleFullScreen( )
		{
			if( m_FullScreen )
			{
				this->SetWindowed( );
			}
			else
			{
				this->SetFullScreen( );
			}

			m_FullScreen = !m_FullScreen;

			return m_FullScreen;
		}

		ZED_BOOL LinuxWindow::Closed( )
		{
			return !m_Running;
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

