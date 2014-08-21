#ifndef __ZED_UTILTIY_FIRSTPERSONCAMERA_HPP__
#define	__ZED_UTILTIY_FIRSTPERSONCAMERA_HPP__

#include <Utility/Camera.hpp>
#include <Arithmetic/Vector3.hpp>

namespace ZED
{
	namespace Utility
	{
		class FirstPersonCamera : public Camera
		{
		public:
			FirstPersonCamera( );
			~FirstPersonCamera( );

			void SetMaxPitch( const ZED_FLOAT32 p_MaxPitch );

			void Move( const Arithmetic::Vector3 &p_Velocity );
			virtual void Rotate( const ZED_FLOAT32 p_Angle,
				const Arithmetic::Vector3 &p_Axis );

			virtual void Update( const ZED_UINT64 p_ElapsedTime );

		private:
			ZED_FLOAT32 m_MaxPitch;
		};
	}
}

#endif

