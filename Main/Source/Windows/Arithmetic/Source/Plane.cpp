#include <Plane.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		ZED_INLINE void Plane::Set( const Vector3 &p_Normal,
			const Vector3 &p_Point )
		{
			m_Distance	= -( m_Normal.Dot( m_Point ) );
			m_Normal	= p_Normal;
			m_Point		= p_Point;
		}

		ZED_INLINE void Plane::Set( const Vector3 &p_Normal,
			const Vector3 &p_Point, ZED_FLOAT32 p_Distance )
		{
			m_Distance = p_Distance;
			m_Normal = p_Normal;
			m_Point = p_Point;
		}

		ZED_INLINE void Plane::Set( const Vector3 &p_Vec1,
			const Vector3 &p_Vec2, const Vector3 &p_Vec3 )
		{
			// Take the edges from the extreme vectors to the point
			// p_Vec1 being the point
			Vector3 Edge1 = p_Vec2 - p_Vec1;
			Vector3 Edge2 = p_Vec3 - p_Vec1;

			// Cross Edge1 with Edge2, storing in m_Normal which will
			// give the triangle
			m_Normal = Edge1.Cross( Edge2 );
			// Smooth it out
			m_Normal.Normalise( );
			// The distance is a dot product of the normal's inverse
			m_Distance = -m_Normal.Dot( p_Vec1 );
			// As stated, p_Vec1 is the point
			m_Point = p_Vec1;
		}

		void Plane::Set( const Vector3 &p_Normal,
			const ZED_FLOAT32 p_Distance )
		{
			m_Normal = p_Normal;
			m_Distance = p_Distance;
		}

		ZED_INLINE ZED_FLOAT32 Plane::Distance( const Vector3 &p_Point )
		{
			return ZED::Arithmetic::Absolute( m_Normal.Dot( m_Point ) -
				m_Distance );
		}

		ZED_INLINE ZED_UINT32 Plane::Classify( const Vector3 &p_Point )
		{
			ZED_FLOAT32 Face = m_Point.Dot( m_Normal ) + m_Distance;

			if( Face > ZED_Epsilon )
			{
				return ZED_FRONTFACE;
			}
			if( Face < ZED_Epsilon )
			{
				return ZED_BACKFACE;
			}
			return ZED_PLANARFACE;
		}
	}
}
