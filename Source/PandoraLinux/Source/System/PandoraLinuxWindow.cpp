#include <System/PandoraLinuxWindow.hpp>
#include <System/Debugger.hpp>
#include <cstdio>
#include <cstring>

namespace ZED
{
	namespace System
	{
		PandoraLinuxWindow::PandoraLinuxWindow( )
		{
			m_pDisplay = ZED_NULL;
			m_Window = 0;

			memset( &m_WindowData, 0, sizeof( m_WindowData ) );
			
			m_X = m_Y = m_Width = m_Height = 0;
		}

		PandoraLinuxWindow::~PandoraLinuxWindow( )
		{
			this->Destroy( );
		}

		ZED_UINT32 PandoraLinuxWindow::Create( const ZED_UINT32 p_X,
			const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height, const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber, const ZED_UINT32 p_Style )
		{
			m_Running = ZED_FALSE;

			char DisplayScreen[ 16 ];
			memset( DisplayScreen, '\0', sizeof( DisplayScreen ) );
			sprintf( DisplayScreen, ":%d.%d", p_DisplayNumber,
				p_ScreenNumber );

			m_pDisplay = XOpenDisplay( DisplayScreen );

			if( m_pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::System::PandoraLinuxWindow] <ERROR> "
					"Failed to open window: :%d.%d\n", p_DisplayNumber,
					p_ScreenNumber );

				return ZED_FAIL;
			}

			XSetWindowAttributes WindowAttributes;

			WindowAttributes.event_mask = StructureNotifyMask | ExposureMask |
				KeyPressMask | KeyReleaseMask |
				ButtonPressMask | ButtonReleaseMask |
				ResizeRedirectMask | PointerMotionMask |
				FocusChangeMask | EnterWindowMask | LeaveWindowMask;
			WindowAttributes.border_pixel = 0;
			WindowAttributes.background_pixmap = None;

			if( p_Style == ZED_WINDOW_STYLE_FULLSCREEN )
			{
			}
			else
			{
				m_X = p_X;
				m_Y = p_Y;
				m_Width = p_Width;
				m_Height = p_Height;
			}

			m_Window = XCreateWindow( m_pDisplay,
				DefaultRootWindow( m_pDisplay ),
				m_X, m_Y, m_Width, m_Height, 0, CopyFromParent, InputOutput,
				CopyFromParent, CWEventMask | CWBorderPixel,
				&WindowAttributes );

			XMapWindow( m_pDisplay, m_Window );
			XMapRaised( m_pDisplay, m_Window );
			XMoveWindow( m_pDisplay, m_Window,
				m_X, m_Y );
			XRaiseWindow( m_pDisplay, m_Window );

			m_Running = ZED_TRUE;

			m_WindowData.X11Window = m_Window;
			m_WindowData.pX11Display = m_pDisplay;
			m_WindowData.eglDisplay = m_EGLDisplay;
			m_WindowData.eglSurface = m_EGLSurface;

			return ZED_OK;
		}

		void PandoraLinuxWindow::Destroy( )
		{
			if( m_Window )
			{
				XDestroyWindow( m_pDisplay, m_Window );
				m_Window = 0;
			}

			if( m_pDisplay )
			{
				XCloseDisplay( m_pDisplay );
				m_pDisplay = ZED_NULL;
			}
		}

		ZED_UINT32 PandoraLinuxWindow::Update( )
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

		void PandoraLinuxWindow::Title( const char *p_pTitle )
		{
			XStoreName( m_pDisplay, m_Window, p_pTitle );
		}

		void PandoraLinuxWindow::HideCursor( )
		{
			if( m_pDisplay )
			{
				XDefineCursor( m_pDisplay, m_Window, this->NullCursor( ) );
			}
		}

		void PandoraLinuxWindow::ShowCursor( )
		{
			if( m_pDisplay )
			{
				XUndefineCursor( m_pDisplay, m_Window );
			}
		}

		ZED_BOOL PandoraLinuxWindow::ToggleCursor( )
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

		void PandoraLinuxWindow::SetWindowed( )
		{
		}

		void PandoraLinuxWindow::SetFullScreen( )
		{
		}

		ZED_BOOL PandoraLinuxWindow::ToggleFullScreen( )
		{
			return ZED_TRUE;
		}

		Cursor PandoraLinuxWindow::NullCursor( )
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

