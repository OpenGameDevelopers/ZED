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

		void Mouse::SetPosition( const ZED_SINT32 p_X, const ZED_SINT32 p_Y )
		{
			m_X = p_X;
			m_Y = p_Y;
		}

		void Mouse::GetPosition( ZED_SINT32 &p_X, ZED_SINT32 &p_Y ) const
		{
			p_X = m_X;
			p_Y = m_Y;
		}
	}
}


