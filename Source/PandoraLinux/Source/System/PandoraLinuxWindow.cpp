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
			m_WindowData.pX11Display = ZED_NULL;
			m_WindowData.X11Window = 0;

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

			m_WindowData.pX11Display = XOpenDisplay( DisplayScreen );

			if( m_WindowData.pX11Display == ZED_NULL )
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

			m_WindowData.X11Window = XCreateWindow( m_WindowData.pX11Display,
				DefaultRootWindow( m_WindowData.pX11Display ),
				m_X, m_Y, m_Width, m_Height, 0, CopyFromParent, InputOutput,
				CopyFromParent, CWEventMask | CWBorderPixel,
				&WindowAttributes );

			XMapWindow( m_WindowData.pX11Display, m_WindowData.X11Window );
			XMapRaised( m_WindowData.pX11Display, m_WindowData.X11Window );
			XMoveWindow( m_WindowData.pX11Display, m_WindowData.X11Window,
				m_X, m_Y );
			XRaiseWindow( m_WindowData.pX11Display, m_WindowData.X11Window );

			m_Running = ZED_TRUE;

			return ZED_OK;
		}

		void PandoraLinuxWindow::Destroy( )
		{
			if( m_WindowData.X11Window )
			{
				XDestroyWindow( m_WindowData.pX11Display,
					m_WindowData.X11Window );
				m_WindowData.X11Window = 0;
			}

			if( m_WindowData.pX11Display )
			{
				XCloseDisplay( m_WindowData.pX11Display );
				m_WindowData.pX11Display = ZED_NULL;
			}
		}

		ZED_UINT32 PandoraLinuxWindow::Update( )
		{
			XEvent Event;

			int Pending = XPending( m_WindowData.pX11Display );

			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_WindowData.pX11Display, &Event );

				switch( Event.type )
				{
					case ClientMessage:
					{
						if( *XGetAtomName( m_WindowData.pX11Display,
							Event.xclient.message_type ) == *"WM_PROTOCOLS" )
						{
							m_Running = ZED_FALSE;
						}
						break;
					}
					default:
					{
						XPutBackEvent( m_WindowData.pX11Display, &Event );
						break;
					}
				}
			}

			return ZED_OK;
		}
	}
}

