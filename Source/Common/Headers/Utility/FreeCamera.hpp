#ifndef __ZED_UTILITY_FREECAMERA_HPP__
#define __ZED_UTILITY_FREECAMERA_HPP__

#include <Arithmetic/Quaternion.hpp>
#include <Utility/Camera.hpp>

namespace ZED
{
	namespace Utility
	{
		class FreeCamera : public Camera
		{
		public:
			FreeCamera( );
			virtual ~FreeCamera( );

			virtual void Rotate( const ZED_FLOAT32 p_Angle,
				const ZED::Arithmetic::Vector3 &p_Axis );
			void Move( const ZED::Arithmetic::Vector3 &p_Velocity );

			virtual void Update( const ZED_UINT64 p_ElapsedTime );

		private:
		};
	}
}

#endif // __ZED_RENDERER_FREECAMERA_HPP__

