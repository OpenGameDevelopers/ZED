#ifndef __ZED_RENDERER_FREECAMERA_HPP__
#define __ZED_RENDERER_FREECAMERA_HPP__

#include <Renderer/Camera.hpp>

namespace ZED
{
	namespace Renderer
	{
		class FreeCamera : public Camera
		{
		public:
			FreeCamera( );
			virtual ~FreeCamera( );

			void Rotate( const ZED::Arithmetic::Vector3 &p_Rotation );
			void Move( const ZED::Arithmetic::Vector3 &p_Velocity );

		private:
			ZED::Arithmetic::Matrix4x4	m_Orientation;
		};
	}
}

#endif // __ZED_RENDERER_FREECAMERA_HPP__

