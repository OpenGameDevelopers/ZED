#include <Arithmetic/Ray.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Arithmetic/Plane.hpp>
#include <Arithmetic/AABB.hpp>
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
			Matrix4x4 Tmp( p_Matrix );

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

		ZED_BOOL Ray::Intersects( const Plane &p_Plane, ZED_BOOL p_Cull,
			ZED_FLOAT32 *p_Length, Vector3 *p_HitPos )
		{
			Vector3 Normal;
			p_Plane.GetNormal( &Normal );
			ZED_FLOAT32 Dist = Normal.Dot( m_Direction );

			if( Arithmetic::IsZero( Dist ) )
			{
				return ZED_FALSE;
			}

			// Check if the plane normal is facing away from the ray's
			// direction
			if( p_Cull && ( Dist > ZED_Epsilon ) )
			{
				return ZED_FALSE;
			}

			ZED_FLOAT32 Origin = -( ( Normal.Dot( m_Origin ) ) +
				p_Plane.GetDistance( ) );

			ZED_FLOAT32 Length = Origin / Dist;

			// Intersection before ray origin
			if( Length < ( -ZED_Epsilon ) )
			{
				return ZED_FALSE;
			}

			if( p_HitPos )
			{
				( *p_HitPos ) = ( m_Origin + ( m_Direction*Length ) );
				// As OpenGL uses a forward Z, negate it to get the correct
				// result
				( *p_HitPos )[ 2 ] = m_Origin[ 2 ]-( m_Direction[ 2 ]*Length );
				( *p_HitPos ).Clean( );
			}

			if( p_Length )
			{
				( *p_Length ) = Length;
			}

			return ZED_TRUE;
		}

		void Ray::Set( const Vector3 &p_Origin, const Vector3 &p_Direction )
		{
			m_Origin = p_Origin;
			m_Direction = p_Direction;
		}

		void Pick( const ZED_UINT32 p_Width, const ZED_UINT32 p_Height,
			const ZED_UINT32 p_X, const ZED_UINT32 p_Y,
			const Matrix4x4 &p_Projection, const Matrix4x4 &p_View,
			Ray *p_Picked )
		{
			Vector3 RayPos, Origin, Direction;
			Matrix4x4 InvView( p_View );
			InvView.AffineInverse( );

			RayPos[ 0 ] = ( ( ( 2.0f * p_X )/p_Width ) - 1.0f )/
				p_Projection( 0, 0 );
			RayPos[ 1 ] = ( ( ( 2.0f * p_Y )/p_Height ) - 1.0f )/
				p_Projection( 1, 1 );
			RayPos[ 2 ] = 1.0f;

			Direction[ 0 ] = RayPos[ 0 ]*InvView( 0, 0 ) +
							 RayPos[ 1 ]*InvView( 1, 0 ) +
							 RayPos[ 2 ]*InvView( 2, 0 );
			Direction[ 1 ] = RayPos[ 0 ]*InvView( 0, 1 ) +
							 RayPos[ 1 ]*InvView( 1, 1 ) +
							 RayPos[ 2 ]*InvView( 2, 1 );
			Direction[ 2 ] = RayPos[ 0 ]*InvView( 0, 2 ) +
							 RayPos[ 1 ]*InvView( 1, 2 ) +
							 RayPos[ 2 ]*InvView( 2, 2 );

			Origin[ 0 ] = InvView( 0, 3 );
			Origin[ 1 ] = InvView( 1, 3 );
			Origin[ 2 ] = InvView( 2, 3 );

			p_Picked->Set( Origin, Direction );
		}
	}
}
