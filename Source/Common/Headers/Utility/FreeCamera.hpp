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

			void Rotate( const ZED::Arithmetic::Vector3 &p_Rotation );
			void Rotate( const ZED_FLOAT32 p_Angle,
				const ZED::Arithmetic::Vector3 &p_Axis );
			void Move( const ZED::Arithmetic::Vector3 &p_Velocity );

			virtual void Update( const ZED_UINT64 p_ElapsedTime );

		private:
			void UpdateOrbitLocation( );

		//	ZED::Arithmetic::Matrix4x4	m_Orientation;
			ZED::Arithmetic::Vector3	m_Orbit;
			ZED::Arithmetic::Quaternion	m_CurrentDirection;
			ZED::Arithmetic::Vector3	m_TempPosition;
		};
	}
}

#endif // __ZED_RENDERER_FREECAMERA_HPP__

