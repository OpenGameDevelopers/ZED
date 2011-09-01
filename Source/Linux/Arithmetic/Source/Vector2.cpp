#include <Arithmetic.hpp>
#include <Vector2.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		ZED_FLOAT32 Vector2::Dot( const Vector2 &p_Other ) const
		{
			return ( m_X*p_Other.m_X + m_Y*p_Other.m_Y );
		}

		void Vector2::Normalise( )
		{
			ZED_FLOAT32 Length = m_X*m_X + m_Y*m_Y;

			if( ZED::Arithmetic::IsZero( Length ) )
			{
				zedAssert( "Vector2::Normalise( ) Magnitude is zero\n" );
			}
			else
			{
				ZED_FLOAT32 Factor = 1.0f / Length;

				m_X *= Factor;
				m_Y *= Factor;
			}
		}

		void Vector2::Clean( )
		{
			if( ZED::Arithmetic::IsZero( m_X ) )
			{
				m_X = 0.0f;
			}
			if( ZED::Arithmetic::IsZero( m_Y ) )
			{
				m_Y = 0.0f;
			}
		}

		ZED_BOOL Vector2::IsZero( ) const
		{
			if( ZED::Arithmetic::IsZero( m_X ) &&
				ZED::Arithmetic::IsZero( m_Y ) )
			{
				return ZED_TRUE;
			}
			else
			{
				return ZED_FALSE;
			}
		}
	}
}

