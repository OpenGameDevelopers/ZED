#ifndef __ZEDMATRIX3X3_HPP
#define __ZEDMATRIX3X3_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Quaternion;
		class Vector3;

		class Matrix3x3
		{
		public:
			ZED_INLINE Matrix3x3( ){ Identity( ); }
			ZED_INLINE ~Matrix3x3( ){ }
			ZED_EXPLICIT Matrix3x3( const Quaternion &p_Quat );

			void Identity( );
			
			void Rotate( const Quaternion &p_Quat );
			void Rotate( const ZED_FLOAT32 p_Angle, const Vector3 &p_Axis );
			void RotateXYZ( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Z );

			void RotateX( ZED_FLOAT32 p_X );
			void RotateY( ZED_FLOAT32 p_Y );
			void RotateZ( ZED_FLOAT32 p_Z );

			void Scale( ZED_FLOAT32 p_Value );

			void ScaleX( ZED_FLOAT32 p_X );
			void ScaleY( ZED_FLOAT32 p_Y );
			void ScaleZ( ZED_FLOAT32 p_Z );

			ZED_BOOL IsZero( ) const;
			ZED_BOOL IsIdentity( ) const;

			// Get/Set rows
			void SetRows( const Vector3 &p_Row1, const Vector3 &p_Row2, 
				const Vector3 &p_Row3 );
			void GetRows( Vector3 *p_pRow1, Vector3 *p_pRow2,
				Vector3 *p_pRow3 ) const;
			Vector3 GetRow( ZED_UINT32 p_RowNumebr ) const;

			// Get/Set columns
			void SetColumns( const Vector3 &p_Col1, const Vector3 &p_Col2,
				const Vector3 &p_Col3 );
			void GetColumns( Vector3 *p_pCol1, Vector3 *p_pCol2,
				Vector3 *p_pCol3 ) const;
			Vector3 GetColumn( ZED_UINT32 p_ColumnNumber ) const;

			// If any elements in the matrix are near-zero, set them as zero to
			// avoid floating point problems
			void Clean( );

			Matrix3x3 &Transpose( ) const;
			Matrix3x3 &Inverse( ) const;
			Matrix3x3 &Adjoint( ) const;
			ZED_FLOAT32 Determinate( ) const;
			ZED_FLOAT32 Trace( ) const;

			// Operator overloading
			// -Debugging-
			// Really need this...

			// -Equality-
			ZED_BOOL operator==( const Matrix3x3 &p_Other ) const;
			ZED_BOOL operator!=( const Matrix3x3 &p_Other ) const;

			// -Addition/Subtraction-
			Matrix3x3 operator+( const Matrix3x3 &p_Other ) const;
			Matrix3x3 operator-( const Matrix3x3 &p_Otehr ) const;
			
			// -Unary negation-
			Matrix3x3 operator-( ) const;

			// -Multiplication-
			Matrix3x3 operator*( const Matrix3x3 &p_Other ) const;
			// Row-wise multiplication
			friend Vector3 operator*( const Vector3 &p_Vec,
				const Matrix3x3 &p_Matrix );
			friend Matrix3x3 operator*( ZED_FLOAT32 p_Scalar,
				const Matrix3x3 &p_Matrix );
			Matrix3x3 operator*( ZED_FLOAT32 p_Scalar ) const;
			// Column-wise multiplication
			Vector3 operator*( const Vector3 &p_Vector ) const;

			// -Assignment-
			Matrix3x3 &operator+=( const Matrix3x3 &p_Other );
			Matrix3x3 &operator-=( const Matrix3x3 &p_Other );
			Matrix3x3 &operator*=( const Matrix3x3 &p_Other );
			Matrix3x3 &operator*=( const ZED_FLOAT32 p_Scalar );

			// -Manipulators-
			ZED_INLINE operator ZED_FLOAT32*( ){ return m_Element; }
			ZED_INLINE operator const ZED_FLOAT32*( ){ return m_Element; }
			ZED_FLOAT32 &operator( )( ZED_UINT32 p_Row, ZED_UINT32 p_Column );
			ZED_FLOAT32 operator( )( ZED_UINT32 p_Row, ZED_FLOAT32 p_Column )
				const;

		private:
			ZED_FLOAT32 m_Element [ 9 ];
		};
	}
}

#endif
