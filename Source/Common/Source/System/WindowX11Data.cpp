#include <System/WindowX11Data.hpp>
#include <System/Debugger.hpp>
#include <string>
#include <cstdlib>

namespace ZED
{
	namespace System
	{
		LinuxWindowData::LinuxWindowData( ) :
			m_pDisplay( ZED_NULL ),
			m_Window( 0 ),
			m_pScreen( ZED_NULL )
		{
			m_DisplayNumber = 0;
			m_ScreenNumber = 0;
		}

		LinuxWindowData::LinuxWindowData( Display *p_pDisplay,
			const ::Window &p_Window, Screen *p_pScreen ) :
			m_pDisplay( p_pDisplay ),
			m_Window( p_Window ),
			m_pScreen( p_pScreen )
		{
			m_ScreenNumber = XScreenNumberOfScreen(
				const_cast< Screen * >( p_pScreen ) );

			std::string DisplayString = XDisplayString( p_pDisplay );

			size_t Deliminator = DisplayString.find_first_of( '.' );
			std::string DisplayNumber =
				DisplayString.substr( 1, Deliminator - 1 );
			m_DisplayNumber = atoi( DisplayNumber.c_str( ) );
		}

		LinuxWindowData::LinuxWindowData(
			const LinuxWindowData &p_WindowData ) :
			m_pDisplay( p_WindowData.m_pDisplay ),
			m_Window( p_WindowData.m_Window ),
			m_pScreen( p_WindowData.m_pScreen )
		{
			m_DisplayNumber = p_WindowData.m_DisplayNumber;
			m_ScreenNumber = p_WindowData.m_ScreenNumber;
		}

		LinuxWindowData::~LinuxWindowData( )
		{
		}

		LinuxWindowData &LinuxWindowData::operator=(
			const LinuxWindowData &p_Other )
		{
			m_pDisplay = p_Other.m_pDisplay;
			m_Window = p_Other.m_Window;
			m_pScreen = p_Other.m_pScreen;
		}

		Display *LinuxWindowData::GetDisplay( ) const
		{
			return m_pDisplay;
		}

		::Window LinuxWindowData::GetWindow( ) const
		{
			return m_Window;
		}

		Screen *LinuxWindowData::GetScreen( ) const
		{
			return m_pScreen;
		}
	}
}

