#ifndef __ZEDCAMERA_HPP__
#define __ZEDCAMERA_HPP__

#include <Arithmetic/Vector3.hpp>
#include <Arithmetic/Quaternion.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Camera
		{
		public:
			void Position( const ZED::Arithmetic::Vector3 &p_Position );
			void Direction(
				const ZED::Arithmetic::Quaternion &p_Direction );

			ZED_INLINE void Position(
				ZED::Arithmetic::Vector3 *p_pPosition ) const
				{ ( *p_pPosition ) = m_Position; }
			ZED::Arithmetic::Vector3 Direction( ) const
				{ return m_Direction; }

		protected:
			ZED::Arithmetic::Vector3 m_Position;
			ZED::Arithmetic::Vector3 m_Direction;
		};
	}
}

#endif
