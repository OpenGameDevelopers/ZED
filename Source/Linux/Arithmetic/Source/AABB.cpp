#include <AABB.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		ZED_BOOL AABB::Intersects( const AABB &p_Other ) const
		{
			if( m_Min[ 0 ] < p_Other.m_Min[ 0 ] ||
				m_Max[ 0 ] > p_Other.m_Max[ 0 ] )
			{
				return ZED_FALSE;
			}
			if( m_Min[ 1 ] < p_Other.m_Min[ 1 ] ||
				m_Max[ 1 ] > p_Other.m_Max[ 1 ] )
			{
				return ZED_FALSE;
			}
			if( m_Min[ 2 ] < p_Other.m_Min[ 2 ] ||
				m_Max[ 2 ] > p_Other.m_Max[ 2 ] )
			{
				return ZED_FALSE;
			}

			return ZED_TRUE;
		}
	}
}

