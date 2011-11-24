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

		private:
			Vector3 m_Min;
			Vector3 m_Max;
		};
	}
}

#endif
