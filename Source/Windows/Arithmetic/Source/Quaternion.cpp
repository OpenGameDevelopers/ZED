#include <Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		void Quaternion::Copy( const Quaternion &p_Original )
		{
			m_X = p_Original.m_X;
			m_Y = p_Original.m_Y;
			m_Z = p_Original.m_Z;
			m_W = p_Original.m_W;
		}

		Quaternion &Quaternion::Clone( ) const
		{
			Quaternion *pClone = new Quaternion( );

			pClone->Copy( *this );

			return *pClone;
		}
	}
}
