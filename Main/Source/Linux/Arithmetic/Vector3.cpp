#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Vector3 &Vector3::Clone( ) const
		{
			Vector3 *pVec = new Vector3( );
			pVec->Copy( *this );
			return *pVec;
		}

		void Vector3::Copy( const Vector3 &Vec3 )
		{
			this->X = Vec3.X;
			this->Y = Vec3.Y;
			this->Z = Vec3.Z;	
		}
		
		void Vector3::Normalise( )
		{
			ZED_FLOAT32 All = ( X*X + Y*Y + Z*Z );
			
			if( zedIsZero( All ) )
			{
				Zero( );
			}
			else
			{
				ZED_FLOAT32 Factor = zedInvSquareRoot( All );
				X *= Factor;
				Y *= Factor;
				Z *= Factor;
			}
		}
	}
}

