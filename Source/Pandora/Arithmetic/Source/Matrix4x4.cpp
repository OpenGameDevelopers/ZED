#include <Matrix4x4.hpp>
#include <Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Matrix4x4::Matrix4x4( const Quaternion &p_Quaternion )
		{
		}

		Matrix4x4 &Matrix4x4::Clone( ) const
		{
			Matrix4x4 Clone;

			return Clone;
		}

		void Matrix4x4::Copy( const Matrix4x4 &p_Original )
		{
		}

		void Matrix4x4::Identity( )
		{
		}

		Matrix4x4 &Matrix4x4::Rotate( const Quaternion &p_Quaternion )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Roll,
			const ZED_FLOAT32 p_Pitch, const ZED_FLOAT32 p_Yaw )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Matrix3x3 &p_Matrix )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateX( const ZED_FLOAT32 p_X )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateY( const ZED_FLOAT32 p_Y )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateZ( const ZED_FLOAT32 p_Z )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const ZED_FLOAT32 p_Scale )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const Vector3 &p_Scale )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleX( const ZED_FLOAT32 p_X )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleY( const ZED_FLOAT32 p_Y )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleZ( const ZED_FLOAT32 p_Z )
		{
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

		Vector4 Matrix4x4::GetRow( const ZED_MEMSIZE p_Index ) const
		{
			return Vector4( 0.0f, 0.0f, 0.0f, 0.0f );
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

		Vector4 Matrix4x4::GetColumn( const ZED_MEMSIZE p_Index ) const
		{
			return Vector4( 0.0f, 0.0f, 0.0f, 0.0f );
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

		Vector4 Matrix4x4::operator*( const Vector4 &p_Vector ) const
		{
			Vector4 Vector;
			return Vector;
		}

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

