#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{		
		void Vector3::Normalise( )
		{
			ZED_FLOAT32 All = ( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
			
			if( ZED::Arithmetic::IsZero( All ) )
			{
				Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor = ZED::Arithmetic::InvSquareRoot( All );
				m_X *= Factor;
				m_Y *= Factor;
				m_Z *= Factor;
			}
		}
	}
}

