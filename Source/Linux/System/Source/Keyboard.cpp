#include <Keyboard.hpp>
#include <cstring>

namespace ZED
{
	namespace System
	{
		Keyboard::Keyboard( )
		{
			memset( m_Keys, 0, sizeof( char )*sizeof( m_Keys ) );
		}

		ZED_UINT32 Keyboard::State( void *p_pState ) const
		{
			return ZED_OK;
		}
	}
}

