#include <System/Keyboard.hpp>
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
			if( p_pState )
			{
				// p_pState is assumed to be a char[ 256 ]
				memcpy( p_pState, m_Keys, sizeof( m_Keys ) );
			}

			return ZED_OK;
		}
	}
}

