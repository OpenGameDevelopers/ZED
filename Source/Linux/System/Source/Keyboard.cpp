#include <Keyboard.hpp>

namespace ZED
{
	namespace System
	{
		Keyboard::Keyboard( )
		{
			for( ZED_MEMSIZE i = 0; i < 128; ++i )
			{
				m_Keys[ i ] = 0;
			}
		}

		ZED_UINT32 Keyboard::State( void *p_pState ) const
		{
			return ZED_OK;
		}
	}
}

