#include <SteeringBehaviours.hpp>

namespace ZED
{
	namespace AI
	{
		Arithmetic::Vector3 SteeringBehaviour::Seek( const Arithmetic::Vector3 &p_Target )
		{
			Arithmetic::Vector3 Ret;

			Ret = p_Target - m_Position;

			Ret.Normalise( );

			Ret *= m_MaxAcceleration;

			return Ret;
		}
	}
}
