#include <Renderer/Camera.hpp>

namespace ZED
{
	namespace Renderer
	{
		void Camera::ClippingPlanes( const ZED_FLOAT32 p_Near,
			const ZED_FLOAT32 p_Far )
		{
			m_Near = p_Near;
			m_Far = p_Far;
			if( m_Near < ZED_Epsilon )
			{
				m_Near = ZED_Epsilon;
			}

			if( m_Far <= 1.0f )
			{
				m_Far = 1.0f;
			}
			
			if( m_Near > m_Far )
			{
				m_Near = m_Far;
				m_Far += 1.0f;
			}
		}
	}
}

