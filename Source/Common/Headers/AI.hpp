#ifndef __ZED_AI_AI_HPP__
#define __ZED_AI_AI_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>

namespace ZED
{
	namespace AI
	{
		// Kinematic structure
		typedef struct __KINEMATIC
		{
			// Static
			Arithmetic::Vector3	Postion;
			ZED_FLOAT32			Orientation;

			// Dynamic
			Arithmetic::Vector3	Velocity;
			ZED_FLOAT32			Rotation;
		}KINEMATIC, *LPKINEMATIC;
	}
}

#endif
