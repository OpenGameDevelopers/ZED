#include <System/WindowData.hpp>

namespace ZED
{
	namespace System
	{
		WindowData::~WindowData( )
		{
		}

		ZED_SINT32 WindowData::GetDisplayNumber( ) const
		{
			return m_DisplayNumber;
		}

		ZED_SINT32 WindowData::GetScreenNumber( ) const
		{
			return m_ScreenNumber;
		}
	}
}

