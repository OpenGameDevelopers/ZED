#include <Vector4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Vector4::Vector4( const Vector4 &p_Copy )
		{
			m_X = p_Copy.m_X;
			m_Y = p_Copy.m_Y;
			m_Z = p_Copy.m_Z;
			m_W = p_Copy.m_W;
		}
	}
}
