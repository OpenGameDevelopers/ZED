#include <Arithmetic/Quaternion.hpp>
#include <Arithmetic/Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Quaternion::Quaternion( ) :
			m_X( 0.0f ),
			m_Y( 0.0f ),
			m_Z( 0.0f ),
			m_W( 0.0f )
		{
		}

		Quaternion::Quaternion( const Quaternion &p_Other )
		{
			m_X = p_Other.m_X;
			m_Y = p_Other.m_Y;
			m_Z = p_Other.m_Z;
			m_W = p_Other.m_W;
		}

		Quaternion &Quaternion::operator=( const Quaternion &p_Other )
		{
			m_X = p_Other.m_X;
			m_Y = p_Other.m_Y;
			m_Z = p_Other.m_Z;
			m_W = p_Other.m_W;

			return *this;
		}

		Quaternion::Quaternion( const ZED_FLOAT32 p_W, const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z ) :
				m_X( p_X ),
				m_Y( p_Y ),
				m_Z( p_Z ),
				m_W( p_W )
		{
		}

		Quaternion::Quaternion( const ZED_FLOAT32 p_W,
			const Vector3 &p_Vector ) :
				m_X( p_Vector.X( ) ),
				m_Y( p_Vector.Y( ) ),
				m_Z( p_Vector.Z( ) ),
				m_W( p_W )
		{
		}

		Quaternion::~Quaternion( )
		{
		}

		void Quaternion::Set( const ZED_FLOAT32 p_W, const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z )
		{
			m_X = p_X;
			m_Y = p_Y;
			m_Z = p_Z;
			m_W = p_W;
		}

		void Quaternion::SetIdentity( )
		{
			m_X = 0.0f;
			m_Y = 0.0f;
			m_Z = 0.0f;
			m_W = 1.0f;
		}

		ZED_BOOL Quaternion::IsIdentity( ) const
		{
			if( ZED::Arithmetic::IsZero( m_X ) &&
				ZED::Arithmetic::IsZero( m_Y ) &&
				ZED::Arithmetic::IsZero( m_Z ) &&
				ZED::Arithmetic::Equal( m_W, 1.0f ) )
			{
				return ZED_TRUE;
			}

			return ZED_FALSE;
		}

		Vector3 Quaternion::AsVector( )
		{
			Vector3 Vector;

			Vector[ 0 ] = m_X;
			Vector[ 1 ] = m_Y;
			Vector[ 2 ] = m_Z;
			
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
			if( ZED::Arithmetic::Equal( this->Magnitude( ), 1.0f ) )
			{
				return ZED_TRUE;
			}

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

