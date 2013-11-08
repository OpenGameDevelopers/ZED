#include <Arithmetic/Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Quaternion::Quaternion( const Quaternion &p_Other )
		{
		}

		Quaternion &Quaternion::operator=( const Quaternion &p_Other )
		{
			return *this;
		}

		Vector3 Quaternion::AsVector( )
		{
			Vector3 Vector;
			
			return Vector;
		}

		void Quaternion::Normalise( )
		{
			const ZED_FLOAT32 Square = ( m_W*m_W + m_X*m_X + m_Y*m_Y +
				m_Z*m_Z );

			if( ZED::Arithmetic::IsZero( Square ) )
			{
				zedTrace( "[ZED::Arithmetic::Quaternion::Normalise] <ERORR> "
					"Square of quaternion is zero\n" );
			}
			else
			{
				const ZED_FLOAT32 Factor =
					1.0f / ZED::Arithmetic::SquareRoot( Square );

				m_X *= Factor;
				m_Y *= Factor;
				m_Z *= Factor;
				m_W *= Factor;
			}
		}

		ZED_FLOAT32 Quaternion::Magnitude( ) const
		{
			return ZED::Arithmetic::SquareRoot( m_W*m_W + m_X*m_X + m_Y*m_Y +
				m_Z*m_Z );
		}

		ZED_FLOAT32 Quaternion::MagnitudeSq( ) const
		{
			return ( m_W*m_W + m_X*m_X + m_Y*m_Y + m_Z*m_Z );
		}

		Quaternion Quaternion::Conjugate( ) const
		{
			Quaternion Conjugate;
			Conjugate.m_W = m_W;
			Conjugate.m_X = -m_X;
			Conjugate.m_Y = -m_Y;
			Conjugate.m_Z = -m_Z;

			return Conjugate;
		}

		ZED_BOOL Quaternion::IsUnit( ) const
		{
			return ZED_FALSE;
		}

		Quaternion Quaternion::operator*( const Quaternion &p_Other ) const
		{
			Quaternion Multiplied;

			Multiplied.m_X =( m_W * p_Other.m_X ) + ( m_X * p_Other.m_W ) +
							( m_Y * p_Other.m_Z ) - ( m_Z * p_Other.m_Y );
			Multiplied.m_Y =( m_W * p_Other.m_Y ) - ( m_X * p_Other.m_Z ) +
							( m_Y * p_Other.m_W ) + ( m_Z * p_Other.m_X );
			Multiplied.m_Z =( m_W * p_Other.m_Z ) + ( m_X * p_Other.m_Y ) -
							( m_Y * p_Other.m_X ) + ( m_Z * p_Other.m_W );
			Multiplied.m_W =( m_W * p_Other.m_W ) - ( m_X * p_Other.m_X ) -
							( m_Y * p_Other.m_Y ) - ( m_Z * p_Other.m_Z );

			return Multiplied;
		}

		Quaternion &Quaternion::operator*=( const Quaternion &p_Other )
		{
			Quaternion Original( m_W, m_X, m_Y, m_Z );

			m_X =	( Original.m_W * p_Other.m_X ) +
					( Original.m_X * p_Other.m_W ) +
					( Original.m_Y * p_Other.m_Z ) -
					( Original.m_Z * p_Other.m_Y );
			
			m_Y =	( Original.m_W * p_Other.m_Y ) -
					( Original.m_X * p_Other.m_Z ) +
					( Original.m_Y * p_Other.m_W ) +
					( Original.m_Z * p_Other.m_X );

			m_Z =	( Original.m_W * p_Other.m_Z ) +
					( Original.m_X * p_Other.m_Y ) -
					( Original.m_Y * p_Other.m_X ) +
					( Original.m_Z * p_Other.m_W );

			m_W =	( Original.m_W * p_Other.m_W ) -
					( Original.m_X * p_Other.m_X ) -
					( Original.m_Y * p_Other.m_Y ) -
					( Original.m_Z * p_Other.m_Z );

			return *this;
		}
	}
}

