#include <Ray.hpp>
#include <Matrix4x4.hpp>
#include <Plane.hpp>
#include <AABB.hpp>
//#include <OBB.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Ray::Ray( const Vector3 &p_Origin, const Vector3 &p_Direction )
		{
			m_Origin = p_Origin;
			m_Direction = p_Direction;
			m_Direction.Normalise( );
		}

		void Ray::Detransform( const Matrix4x4 &p_Matrix )
		{
			Matrix4x4 Tmp = p_Matrix;

			// Take the negated translation from the matrix
			m_Origin[ 0 ] -= p_Matrix( 0, 3 );
			m_Origin[ 1 ] -= p_Matrix( 1, 3 );
			m_Origin[ 2 ] -= p_Matrix( 2, 3 );

			// Get rid of the translation in the matrix
			Tmp( 0, 3 ) = Tmp( 1, 3 ) = Tmp( 2, 3 ) = 0.0f;

			// Invert the matrix
			Tmp.AffineInverse( );

/*UNCOMMENT!			m_Direction = m_Origin * Tmp;*/
		}
	}
}
