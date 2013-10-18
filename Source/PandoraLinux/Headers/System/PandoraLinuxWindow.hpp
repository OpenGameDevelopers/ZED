#ifndef __ZED_SYSTEM_PANDORALINUXWINDOW_HPP__
#define __ZED_SYSTEM_PANDORALINUXWINDOW_HPP__

#include <System/Window.hpp>

namespace ZED
{
	namespace System
	{	
		class PandoraLinuxWindow : public Window
		{
		public:
			PandoraLinuxWindow( );
			virtual ~PandoraLinuxWindow( );

			virtual ZED_UINT32 Create( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height, const ZED_UINT32 p_DisplayNumber,
				const ZED_UINT32 p_ScreenNumber, const ZED_UINT32 p_Style );

			virtual void Destroy( );

			virtual ZED_UINT32 Update( );

			ZED_INLINE virtual WINDOWDATA WindowData( ) const
				{ return m_WindowData; }

			virtual void Title( const char *p_pTitle );

			virtual void HideCursor( );
			virtual void ShowCursor( );
			virtual ZED_BOOL ToggleCursor( );

			virtual void SetWindowed( );
			virtual void SetFullScreen( );
			virtual ZED_BOOL ToggleFullScreen( );

			ZED_INLINE virtual ZED_BOOL Closed( )
				{ return !m_Running; }

		private:
			Cursor		NullCursor( );

			ZED_BOOL	m_CursorHidden;
			ZED_BOOL	m_Running;
			ZED_UINT32	m_X;
			ZED_UINT32	m_Y;
			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;

			::Window	m_Window;
			Display		*m_pDisplay;
			EGLDisplay	m_EGLDisplay;
			EGLSurface	m_EGLSurface;
			WINDOWDATA	m_WindowData;
		};

	}
}

#endif

