#ifndef __ZED_AI_STEERINGBEHAVIOURS_HPP__
#define __ZED_AI_STEERINGBEHAVIOURS_HPP__

#include <AI.hpp>
#include <Vector2.hpp>

namespace ZED
{
	namespace AI
	{
		class SteeringBehaviour
		{
		public:
			SteeringBehaviour( );
			~SteeringBehaviour( );

			void Flee( Arithmetic::Vector2 p_Position );
			Arithmetic::Vector3 Seek( const Arithmetic::Vector3 &p_Target );
			void Arrive( Arithmetic::Vector2 p_Positon );
		private:
			Arithmetic::Vector3	m_Position;
			ZED_FLOAT32			m_Orientation;

			Arithmetic::Vector3	m_Velocity;
			ZED_FLOAT32			m_Rotation;

			ZED_FLOAT32			m_MaxAcceleration;
		};
	}
}

#endif
