#ifndef __ZEDPLANE_HPP__
#define __ZEDPLANE_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>
#include <Ray.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Plane
		{
		public:
			ZED_INLINE Plane( ){ }

			// Should these sets also become contructors?
			void Set( const Vector3 &p_Normal,
				const Vector3 &p_Point );
			void Set( const Vector3 &p_Normal,
				const Vector3 &p_Point, ZED_FLOAT32 p_Distance );
			void Set( const Vector3 &p_Vec1, const Vector3 &p_Vec2,
				const Vector3 &p_Vec3 );
			void Set( const Vector3 &p_Normal,
				const ZED_FLOAT32 p_Distance );

			// Get/Set normal
			ZED_INLINE void GetNormal( Vector3 *p_pNormal ) const
				{ p_pNormal->Copy( m_Normal ); }
			ZED_INLINE void SetNormal( const ZED_FLOAT32 p_X,
				const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z );

			// Get/Set distance
			ZED_INLINE ZED_FLOAT32 GetDistance( ) const { return m_Distance; }
			ZED_INLINE void SetDistance( ZED_FLOAT32 p_Distance )
				{ m_Distance = p_Distance; }

			ZED_INLINE ZED_FLOAT32 Distance( const Vector3 &p_Point );
			// Front, back or intersect?
			ZED_INLINE ZED_UINT32 Classify( const Vector3 &p_Point );
			// Also, polygon classify

			//ZED_BOOL Clip( const Ray &p_Ray, ZED_FLOAT32 p_Length, Ray *p_p

			// Triangle intersection
			ZED_BOOL Intersects( const Vector3 &p_Vec1, const Vector3 &p_Vec2,
				const Vector3 &p_Vec3 );
			// Ray intersection
			ZED_BOOL Intersects( const Plane &p_Plane, Ray *p_pIntersection );
			// Need to add AABB & OBB intersect

		private:
			// Plane's normal vector
			Vector3 m_Normal;
			// A point on the plane
			Vector3 m_Point;
			// Distance to the origin
			ZED_FLOAT32 m_Distance;
		};

		ZED_INLINE void Plane::SetNormal( const ZED_FLOAT32 p_X,
				const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z )
		{
			m_Normal[ 0 ] = p_X;
			m_Normal[ 1 ] = p_Y;
			m_Normal[ 2 ] = p_Z;
		}
	}
}

#endif
