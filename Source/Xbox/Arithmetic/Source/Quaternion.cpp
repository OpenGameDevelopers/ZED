#include <Quaternion.hpp>


namespace ZED
{
	namespace Arithmetic
	{
		ZED_BOOL Quaternion::IsUnit( ) const
		{
			return Arithmetic::Equal(
				m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W,
				ZED_Epsilon );
		}
	}
}
