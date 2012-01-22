#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		void Vector3::Normalise( )
		{
			ZED_FLOAT32 Mag = ( m_X*m_X )+( m_Y*m_Y )+( m_Z*m_Z );

			// Don't continue if the squared magnitude is zero
			if( ZED::Arithmetic::IsZero( Mag ) )
			{
				zedTrace( "[ZED::Arithmetic::Vector3::Normalise] <WARN> "
					"Magnitude is zero!\n" );
			}
			else
			{
				ZED_FLOAT32 Factor = 1.0f/Mag;

				m_X *= Factor;
				m_Y *= Factor;
				m_Z *= Factor;
			}
		}
	}
}

