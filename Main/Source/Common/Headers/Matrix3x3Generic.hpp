#ifndef __ZEDMATRIX3X3GENERIC_HPP__
#define __ZEDMATRIX3X3GENERIC_HPP__

#include <Matrix3x3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Matrix3x3Generic : public Matrix3x3
		{
			ZED_INLINE Matrix3x3Generic( ){ Identity( ); }
			ZED_INLINE ~Matrix3x3Generic( ){ }
			Matrix3x3Generic( const Quaternion &p_Quat );

			void Identity( );

			Matrix3x3 &Rotate( const Quaternion p_Quat );
			Matrix3x3 &Rotate( const ZED_FLOAT32 p_Angle,
				const Vector3 &p_Axis );
			Matrix3x3 &Rotate( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Z );

			Matrix3x3 &RotateX( const ZED_FLOAT32 p_X );
			Matrix3x3 &RotateY( const ZED_FLOAT32 p_Y );
			Matrix3x3 &RotateZ( const ZED_FLOAT32 p_Z );

			Matrix3x3 &Scale( const ZED_FLOAT32 p_Scale );
			Matrix3x3 &ScaleX( const ZED_FLOAT32 p_X );
			Matrix3x3 &ScaleY( const ZED_FLOAT32 p_Y );
			Matrix3x3 &ScaleZ( const ZED_FLOAT32 p_Z );

			ZED_BOOL IsZero( ) const;
			ZED_BOOL IsIdentity( ) const;

			// Get/Set rows
			void SetRows( const Vector3 &p_Row1, const Vector3 &p_Row2,
				const Vector3 &p_Row3 );
			void GetRows( Vector3 &p_Row1, Vector3 &p_Row2,
				Vector3 &p_Row3 ) const;
			Vector3 GetRow( const ZED_UINT32 p_RowNumber ) const;

			// If any elements in the matrix are close to zero, set them to
			// zero
			void Clean( );

			Matrix3x3 &Transpose( );
			Matrix3x3 &Inverse( );
			Matrix3x3 Adjoint( ) const;
			ZED_FLOAT32 Determinate( ) const;
			ZED_FLOAT32 Trace( ) const;

			// Operator overloading
			// [Equality]
			ZED_BOOL operator==( const Matrix3x3 &p_Other ) const;
			ZED_BOOL operator!=( const Matrix3x3 &p_Other ) const;

			// [Addition/Subtraction]
			Matrix3x3 operator+( const Matrix3x3 &p_Other ) const;
			Matrix3x3 operator-( const Matrix3x3 &p_Other ) const;

			// [Unary negation]
			Matrix3x3 operator-( ) const;

			// [Multiplication]
			Matrix3x3 operator*( const Matrix3x3 &p_Other ) const;
			// [Row-wise multiplcation]
			friend Vector3 operator*( const Vector3 &p_Vec,
				const Matrix3x3 &p_Matrix );
			friend Matrix3x3 operator*( const ZED_FLOAT32 p_Scalar,
				const Matrix3x3 &p_Matrix );
			Matrix3x3 operator*( const ZED_FLOAT32 p_Scalar ) const;
			// [Column-wise multiplication]
			Vector3 operator*( const Vector3 &p_Vec ) const;
		};
	}
}

#endif
