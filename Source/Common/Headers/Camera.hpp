#ifndef __ZEDCAMERA_HPP__
#define __ZEDCAMERA_HPP__

#include <Vector3.hpp>
#include <Quaternion.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Camera
		{
		public:
			void SetPosition( const ZED::Arithmetic::Vector3 &p_Position );
			void SetDirection(
				const ZED::Arithmetic::Quaternion &p_Direction );

			ZED_INLINE void GetPosition(
				ZED::Arithmetic::Vector3 *p_pPosition ) const
				{ p_pPosition->Copy( m_Position ); }
			/*ZED::Arithmetic::Vector3 GetDirection( ) const
				{ return m_Direction.Vector3( ); }*/

		private:
			ZED::Arithmetic::Vector3 m_Position;
			ZED::Arithmetic::Quaternion m_Direction;
		};
	}
}

#endif
