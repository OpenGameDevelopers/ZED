#ifndef __ZEDRAY_HPP__
#define __ZEDRAY_HPP__

#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Ray
		{
		public:
			ZED_INLINE Ray( ) : m_Origin( 0.0f, 0.0f, 0.0f ),
				m_Direction( 0.0f, 0.0f, 0.0f ){ }
			Ray( const Vector3 &p_Origin, const Vector3 &p_Direction );
			ZED_INLINE ~Ray( ){ }

			// Accessors
			ZED_INLINE Vector3 GetOrigin( ) const
				{ return m_Origin; }
			ZED_INLINE Vector3 GetDirection( ) const
				{ return m_Direction; }
			void Get( Vector3 &p_Origin, Vector3 &p_Direction );

			// Manipulators
			void Set( const Vector3 &p_Origin, const Vector3 &p_Direction );
			ZED_INLINE void Clean( ){ m_Origin.Clean( ); m_Direction.Clean( );}

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
