#include <Matrix4x4.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <Matrix3x3.hpp>
#include <Quaternion.hpp>
#include <cstring>

namespace ZED
{
	namespace Arithmetic
	{
		Matrix4x4::Matrix4x4( const Matrix4x4 &p_Other )
		{
			memcpy( m_M, p_Other.m_M, sizeof( ZED_FLOAT32 )*16 );
		}
		
		Matrix4x4 &Matrix4x4::operator=( const Matrix4x4 &p_Other )
		{
			memcpy( m_M, p_Other.m_M, sizeof( ZED_FLOAT32 )*16 );

			return *this;
		}

		Matrix4x4::Matrix4x4( const Quaternion &p_Quaternion )
		{
			this->Rotate( p_Quaternion );
		}

		void Matrix4x4::Identity( )
		{
			m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = m_M[ 15 ] = 1.0f;

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 14 ] = 0.0f;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Quaternion &p_Q )
		{
			m_M[ 0 ] = 1.0f - ( 2*( p_Q[ 1 ]*p_Q[ 1 ] ) ) -
				( 2*( p_Q[ 2 ]*p_Q[ 2 ] ) );
			m_M[ 1 ] = ( 2*p_Q[ 0 ]*p_Q[ 1 ] ) + ( 2*p_Q[ 3 ]*p_Q[ 2 ] );
			m_M[ 2 ] = 2*( p_Q[ 0 ]*p_Q[ 2 ] ) - 2*( p_Q[ 2 ]*p_Q[ 1 ] );
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 2*( p_Q[ 0 ]*p_Q[ 1 ] ) - 2*( p_Q[ 3 ]*p_Q[ 2 ] );
			m_M[ 5 ] = 1-( 2*( p_Q[ 0 ]*p_Q[ 0 ] ) ) -
				( 2*( p_Q[ 2 ]*p_Q[ 2 ] ) );
			m_M[ 6 ] = 2*p_Q[ 1 ]*p_Q[ 2 ] + 2*p_Q[ 3 ]*p_Q[ 0 ];
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = 2*( p_Q[ 0 ]*p_Q[ 2 ] ) + 2*( p_Q[ 3 ]*p_Q[ 1 ] );
			m_M[ 9 ] = 2*( p_Q[ 1 ]*p_Q[ 2 ] ) - 2*( p_Q[ 3 ]*p_Q[ 0 ] );
			m_M[ 10 ] = 1 - ( 2*( p_Q[ 0 ]*p_Q[ 0 ] ) ) -
				( 2*( p_Q[ 1 ]*p_Q[ 1 ] ) );
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;
			
			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			ZED_FLOAT32 Cos = 0.0f, Tan = 0.0f, Sin = 0.0f;
			
			Arithmetic::SinCos( p_Angle, Sin, Cos );

			m_M[ 0 ] = ( Tan*( p_Axis[ 0 ]*p_Axis[ 0 ] ) ) + Cos;
			m_M[ 1 ] = ( Tan*p_Axis[ 0 ]*p_Axis[ 1 ] ) + ( Sin*p_Axis[ 2 ] );
			m_M[ 2 ] = ( Tan*p_Axis[ 0 ]*p_Axis[ 2 ] ) - ( Sin*p_Axis[ 1 ] );

			m_M[ 4 ] = ( Tan*p_Axis[ 0 ]*p_Axis[ 1 ] ) - ( Sin*p_Axis[ 2 ] );
			m_M[ 5 ] = ( Tan*( p_Axis[ 1 ]*p_Axis[ 1 ] ) ) + Cos;
			m_M[ 6 ] = ( Tan*p_Axis[ 1 ]*p_Axis[ 2 ] ) + ( Sin*p_Axis[ 0 ] );

			m_M[ 8 ] = ( Tan*p_Axis[ 0 ]*p_Axis[ 2 ] ) + ( Sin*p_Axis[ 1 ] );
			m_M[ 9 ] = ( Tan*p_Axis[ 1 ]*p_Axis[ 2 ] ) + ( Sin*p_Axis[ 0 ] );
			m_M[ 10 ] = ( Tan*( p_Axis[ 2 ]*p_Axis[ 2 ] ) ) + Cos;

			m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;

			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Roll,
			const ZED_FLOAT32 p_Pitch, const ZED_FLOAT32 p_Yaw )
		{
			ZED_FLOAT32 CX = 0.0f, CY = 0.0f, CZ = 0.0f;
			ZED_FLOAT32 SX = 0.0f, SY = 0.0f, SZ = 0.0f;

			Arithmetic::SinCos( p_Pitch, SX, CX );
			Arithmetic::SinCos( p_Yaw, SY, CY );
			Arithmetic::SinCos( p_Roll, SZ, CZ );

			m_M[ 0 ] = CY*CZ;
			m_M[ 1 ] = ( SZ*SY*CZ ) + ( CX*SZ );
			m_M[ 2 ] = -( CX*SY*CZ ) + ( SX*SZ );
			m_M[ 3 ] = 0.0f;
			
			m_M[ 4 ] = -( CY*SZ );
			m_M[ 5 ] = -( SX*SY*SZ ) + ( CX*CZ );
			m_M[ 6 ] = -( CX*SY*SZ ) + ( SZ*CZ );
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = SY;
			m_M[ 9 ] = -( SX*CY );
			m_M[ 10 ] = ( CX*CY );
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Matrix3x3 &p_Matrix )
		{
			m_M[ 0 ] = p_Matrix[ 0 ];
			m_M[ 1 ] = p_Matrix[ 1 ];
			m_M[ 2 ] = p_Matrix[ 2 ];
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = p_Matrix[ 3 ];
			m_M[ 5 ] = p_Matrix[ 4 ];
			m_M[ 6 ] = p_Matrix[ 5 ];
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = p_Matrix[ 6 ];
			m_M[ 9 ] = p_Matrix[ 7 ];
			m_M[ 10 ] = p_Matrix[ 8 ];
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateX( const ZED_FLOAT32 p_X )
		{
			ZED_FLOAT32 Sin = 0.0f, Cos = 0.0f;
			Arithmetic::SinCos( p_X, Sin, Cos );

			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = Cos;
			m_M[ 6 ] = Sin;
			m_M[ 7 ] = 0.0f;
			
			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = -Sin;
			m_M[ 10 ] = Cos;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateY( const ZED_FLOAT32 p_Y )
		{
			ZED_FLOAT32 Sin = 0.0f, Cos = 0.0f;

			Arithmetic::SinCos( p_Y, Sin, Cos );

			m_M[ 0 ] = Cos;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = -Sin;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = 1.0f;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = Sin;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = Cos;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateZ( const ZED_FLOAT32 p_Z )
		{
			ZED_FLOAT32 Sin = 0.0f, Cos = 0.0f;

			m_M[ 0 ] = Cos;
			m_M[ 1 ] = Sin;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = -Sin;
			m_M[ 5 ] = Cos;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = 1.0f;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 0.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const ZED_FLOAT32 p_Scale )
		{
			m_M[ 0 ] = p_Scale;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = p_Scale;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = p_Scale;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const Vector3 &p_Scale )
		{
			m_M[ 0 ] = p_Scale[ 0 ];
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = p_Scale[ 1 ];
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = p_Scale[ 2 ];
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleX( const ZED_FLOAT32 p_X )
		{
			m_M[ 0 ] = p_X;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = 1.0f;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = 1.0f;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;
			
			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleY( const ZED_FLOAT32 p_Y )
		{
			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = p_Y;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;
			
			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = 1.0f;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleZ( const ZED_FLOAT32 p_Z )
		{
			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = 0.0f;

			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = 1.0f;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = 0.0f;

			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = p_Z;
			m_M[ 11 ] = 0.0f;

			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		ZED_BOOL Matrix4x4::IsZero( ) const
		{
			return ZED_TRUE;
		}

		ZED_BOOL Matrix4x4::IsIdentity( ) const
		{
			return ZED_TRUE;
		}

		void Matrix4x4::SetRows( const Vector4 &p_Row1, const Vector4 &p_Row2,
			const Vector4 &p_Row3, const Vector4 &p_Row4 )
		{
		}

		void Matrix4x4::GetRows( Vector4 &p_Row1, Vector4 &p_Row2,
			Vector4 &p_Row3, Vector4 &p_Row4 ) const
		{
		}

		void Matrix4x4::GetRow( const ZED_MEMSIZE p_Index,
			Vector4 &p_Vector ) const
		{
		}

		void Matrix4x4::SetColumns( const Vector4 &p_Column1,
			const Vector4 &p_Column2, const Vector4 &p_Column3,
			const Vector4 &p_Column4 )
		{
		}

		void Matrix4x4::GetColumns( Vector4 &p_Column1, Vector4 &p_Column2,
			Vector4 &p_Column3, Vector4 &p_Column4 ) const
		{
		}

		void Matrix4x4::GetColumn( const ZED_MEMSIZE p_Index,
			Vector4 &p_Vector ) const
		{
		}

		void Matrix4x4::Clean( )
		{
		}

		Matrix4x4 &Matrix4x4::Transpose( )
		{
			return *this;
		}

		void Matrix4x4::Transpose( Matrix4x4 &p_Matrix ) const
		{
		}

		Matrix4x4 &Matrix4x4::TransposeOf( const Matrix4x4 &p_Transpose )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::AffineInverse( )
		{
			return *this;
		}

		void Matrix4x4::AffineInverse( Matrix4x4 &p_Matrix ) const
		{
		}

		Matrix4x4 &Matrix4x4::AffineInverseOf(
			const Matrix4x4 &p_AffineInverse )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Translate( const Vector3 &p_Translate )
		{
			return *this;
		}

		Vector3 &Matrix4x4::Transform( const Vector3 &p_Point ) const
		{
			Vector3 Tmp;
			return Tmp;
		}

		ZED_FLOAT32 Matrix4x4::Trace( ) const
		{	
			return 1.0f;
		}

		ZED_BOOL Matrix4x4::operator==( const Matrix4x4 &p_Other ) const
		{
			return ZED_TRUE;
		}

		ZED_BOOL Matrix4x4::operator!=( const Matrix4x4 &p_Other ) const
		{
			return ZED_TRUE;
		}

		Matrix4x4 Matrix4x4::operator+( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Matrix;
			return Matrix;
		}

		Matrix4x4 Matrix4x4::operator-( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Matrix;
			return Matrix;
		}

		Matrix4x4 Matrix4x4::operator-( ) const
		{
			Matrix4x4 Matrix;
			return Matrix;
		}

		Matrix4x4 Matrix4x4::operator*( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Matrix;
			return Matrix;
		}

		Matrix4x4 Matrix4x4::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			Matrix4x4 Matrix;
			return Matrix;
		}
/*
		Vector4 Matrix4x4::operator*( const Vector4 &p_Vector ) const
		{
			Vector4 Vector;
			return Vector.Copy( );
		}*/

		Matrix4x4 &Matrix4x4::operator+=( const Matrix4x4 &p_Other )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::operator-=( const Matrix4x4 &p_Other )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::operator*=( const Matrix4x4 &p_Other )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			return *this;
		}
	}
}

