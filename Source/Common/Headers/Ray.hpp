#ifndef __ZED_ARITHMETIC_RAY_HPP__
#define __ZED_ARITHMETIC_RAY_HPP__

#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		// Forward-declarations
		class Vector3;
		class Matrix4x4;
		class Plane;
		class AABB;
		class OBB;

		class Ray
		{
		public:
			ZED_INLINE Ray( ) : m_Origin( 0.0f, 0.0f, 0.0f ),
				m_Direction( 0.0f, 0.0f, 0.0f ){ }
			Ray( const Vector3 &p_Origin, const Vector3 &p_Direction );
			ZED_INLINE ~Ray( ){ }

			// Transform the ray into the co-ordinate space of the matrix
			// passed in
			void Detransform( const Matrix4x4 &p_Matrix );

			// Intersection functions
			// - Triangle -
			ZED_BOOL Intersects( const Vector3 &p_V0, const Vector3 &p_V1,
				const Vector3 &p_V2, ZED_BOOL p_Cull, ZED_FLOAT32 *p_Length );
			ZED_BOOL Intersects( const Vector3 &p_V0, const Vector3 &p_V1,
				const Vector3 &p_V2, ZED_BOOL p_Cull, ZED_FLOAT32 *p_Length,
				const ZED_FLOAT32 p_MaxLength );

			// - Plane -
			ZED_BOOL Intersects( const Plane &p_Plane, ZED_BOOL p_Cull,
				ZED_FLOAT32 *p_Length, Vector3 *p_HitPos );
			ZED_BOOL Intersects( const Plane &p_Plane, ZED_BOOL p_Cull,
				ZED_FLOAT32 *p_Length, Vector3 *p_HitPos,
				const ZED_FLOAT32 p_MaxLength );

			// - AABB -
			ZED_BOOL Intersects( const AABB &p_AABB, ZED_FLOAT32 *p_Length,
				Vector3 *p_HitPos );
			ZED_BOOL Intersects( const AABB &p_AABB, ZED_FLOAT32 *p_Length,
				Vector3 *p_HitPos, const ZED_FLOAT32 p_MaxLength );

			// - OBB -
			ZED_BOOL Intersects( const OBB &p_OBB, ZED_FLOAT32 *p_Length );
			ZED_BOOL Intersects( const OBB &p_OBB, ZED_FLOAT32 *p_Length,
				const ZED_FLOAT32 p_MaxLength );

			// Accessors
			ZED_INLINE Vector3 GetOrigin( ) const
				{ return m_Origin; }
			ZED_INLINE Vector3 GetDirection( ) const
				{ return m_Direction; }
			void Get( Vector3 &p_Origin, Vector3 &p_Direction );

			// Manipulators
			void Set( const Vector3 &p_Origin, const Vector3 &p_Direction );
			ZED_INLINE void Clean( )
				{ m_Origin.Clean( ); m_Direction.Clean( ); }

			// Operator overloading
			// -Equality-
			ZED_BOOL operator==( const Ray &p_Other ) const;
			ZED_BOOL operator!=( const Ray &p_Other ) const;

		private:
			Vector3 m_Origin;
			Vector3 m_Direction;
		};
	}
}

#endif
