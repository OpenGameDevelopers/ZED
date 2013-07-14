#ifndef __ZED_RENDERER_FIRSTPERSONCAMERA_HPP__
#define	__ZED_RENDERER_FIRSTPERSONCAMERA_HPP__

#include <Renderer/Camera.hpp>
#include <Arithmetic/Vector3.hpp>

namespace ZED
{
	namespace Renderer
	{
		class FirstPersonCamera : public Camera
		{
		public:
			FirstPersonCamera( );
			~FirstPersonCamera( );

			ZED_INLINE void HeadOffset( const ZED_FLOAT32 p_HeadOffset )
				{ m_Position[ 1 ] = p_HeadOffset; }
			ZED_INLINE void MaxPitch( const ZED_FLOAT32 p_MaxPitch )
				{ m_MaxPitch = p_MaxPitch; }

			void Move( const Arithmetic::Vector3 &p_Velocity );
			void Look( const Arithmetic::Vector3 &p_Velocity );

		private:
			ZED_FLOAT32 m_MaxPitch;
		};
	}
}

#endif

