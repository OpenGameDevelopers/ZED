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
			m_EGLDisplay = ( EGLDisplay )0;

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

			// The OGLES 2.0 driver on the Pandora seems to no longer works
			// with X11, only the framebuffer is supported (Rebirth and 1GHz)
			// TODO
			// Verify the version of the Pandora to decide to use the
			// framebuffer or not

			m_EGLDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );

			EGLint EGLMajor;
			EGLint EGLMinor;

			eglInitialize( m_EGLDisplay, &EGLMajor, &EGLMinor );

			// This is pretty bad
			EGLint EGLAttributes [ ] =
			{
				EGL_RED_SIZE,	8,
				EGL_GREEN_SIZE,	8,
				EGL_BLUE_SIZE,	8,
				EGL_ALPHA_SIZE,	8,
				EGL_DEPTH_SIZE,	24,
				EGL_STENCIL_SIZE,	8,
				EGL_CONFORMANT,	EGL_OPENGL_ES2_BIT,
				EGL_NONE
			};

			EGLint EGLConfigs;
			EGLConfig Configuration;

			eglChooseConfig( m_EGLDisplay, EGLAttributes, &Configuration, 1,
				&EGLConfigs );

			m_EGLSurface = eglCreateWindowSurface( m_EGLDisplay, Configuration,
				( NativeWindowType )0, NULL );

			m_EGLContext = eglCreateContext( m_EGLDisplay, Configuration,
				NULL, NULL );

			eglMakeCurrent( m_EGLDisplay, m_EGLSurface, m_EGLSurface,
				m_EGLContext );

			m_Running = ZED_TRUE;

			m_WindowData.X11Window = m_Window;
			m_WindowData.pX11Display = m_pDisplay;
			m_WindowData.eglDisplay = m_EGLDisplay;
			m_WindowData.eglSurface = m_EGLSurface;

			return ZED_OK;
		}

		void PandoraLinuxWindow::Destroy( )
		{
			if( m_EGLSurface )
			{
				eglDestroySurface( m_EGLDisplay, m_EGLSurface );
			}

			if( m_EGLDisplay )
			{
				eglMakeCurrent( m_EGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
					EGL_NO_CONTEXT );
				eglTerminate( m_EGLDisplay );
			}

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

