#include <Matrix4x4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		void Matrix4x4::Identity( )
		{
			m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] =
				m_M[ 15 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] =
				m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] =
				m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] =
				m_M[ 13 ] = m_M[ 14 ] = 0.0f;
		}
	}
}
