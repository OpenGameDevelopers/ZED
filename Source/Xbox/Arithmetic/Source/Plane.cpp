#include <Plane.hpp>


namespace ZED
{
	namespace Arithmetic
	{
		void Plane::Set( const Vector3 &p_Normal,
			const ZED_FLOAT32 p_Distance )
		{
			m_Normal = p_Normal;
			m_Distance = p_Distance;
		}
	}
}
