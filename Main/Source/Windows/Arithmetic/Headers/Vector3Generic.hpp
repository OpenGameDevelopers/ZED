#ifndef __ZEDVECTOR3_GENERIC_HPP__
#define __ZEDVECTOR3_GENERIC_HPP__

#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Vector3Generic : public Vector3
		{
			virtual ZED_FLOAT32 Magnitude( ) const;
		};
	}
}

#endif
