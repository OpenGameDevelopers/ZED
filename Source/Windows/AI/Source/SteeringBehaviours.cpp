#include <SteeringBehaviours.hpp>

namespace ZED
{
	namespace AI
	{
		void SteeringBehaviour::Seek( const Arithmetic::Vector3 &p_Target,
			Arithmetic::Vector3 *p_pFinalTarget )
		{
			p_pFinalTarget->Copy( p_Target - m_Position );
			p_pFinalTarget->Normalise( );
			( *p_pFinalTarget ) *= m_MaxAcceleration;
		}
	}
}
