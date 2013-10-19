#include <System/Mouse.hpp>

namespace ZED
{
	namespace System
	{
		Mouse::Mouse( )
		{
			m_Buttons = 0x00;
			m_X = 0;
			m_Y = 0;
		}

		Mouse::~Mouse( )
		{
		}

		ZED_UINT32 Mouse::State( void *p_pState ) const
		{
			return ZED_OK;
		}
	}
}


