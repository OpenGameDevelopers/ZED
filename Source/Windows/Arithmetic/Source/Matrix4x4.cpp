#include <Matrix4x4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		void Matrix4x4::Identity( )
		{
			m_Element[ 0 ] = m_Element[ 5 ] = m_Element[ 10 ] =
				m_Element[ 15 ] = 1.0f;
			m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] = m_Element[ 4 ] =
				m_Element[ 6 ] = m_Element[ 7 ] = m_Element[ 8 ] =
				m_Element[ 9 ] = m_Element[ 11 ] = m_Element[ 12 ] =
				m_Element[ 13 ] = m_Element[ 14 ] = 0.0f;
		}
	}
}
