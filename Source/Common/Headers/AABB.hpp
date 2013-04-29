#ifndef __ZED_ARITHMETIC_AABB_HPP__
#define __ZED_ARITHMETIC_AABB_HPP__

#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class AABB
		{
		public:
			ZED_BOOL Intersects( const AABB &p_Other ) const;
			ZED_INLINE void Min( const Vector3 &p_Min )
				{ m_Min = p_Min; }
			ZED_INLINE void Max( const Vector3 &p_Max )
				{ m_Max = p_Max; }

		private:
			Vector3 m_Min;
			Vector3 m_Max;
		};
	}
}

#endif
