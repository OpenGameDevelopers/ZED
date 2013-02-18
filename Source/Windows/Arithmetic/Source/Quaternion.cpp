#include <Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Quaternion::Quaternion( const Quaternion &p_Original )
		{
			m_X = p_Original.m_X;
			m_Y = p_Original.m_Y;
			m_Z = p_Original.m_Z;
			m_W = p_Original.m_W;
		}

		Quaternion &Quaternion::operator=( const Quaternion &p_Other )
		{
			m_X = p_Other.m_X;
			m_Y = p_Other.m_Y;
			m_Z = p_Other.m_Z;
			m_W = p_Other.m_W;		

			return *this;
		}
	}
}
