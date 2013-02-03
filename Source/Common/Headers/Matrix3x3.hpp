#ifndef __ZED_ARITHMETIC_MATRIX3X3_HPP__
#define __ZED_ARITHMETIC_MATRIX3X3_HPP__

#include <DataTypes.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Quaternion;
		class Vector3;

		class Matrix3x3
		{
		public:
			// Constructors
			ZED_INLINE Matrix3x3( ){ Identity( ); }

			Matrix3x3( const Matrix3x3 &p_Other );
			Matrix3x3 &operator=( const Matrix3x3 &p_Other );

			ZED_EXPLICIT Matrix3x3( const Quaternion &p_Quaternion );

			ZED_INLINE ~Matrix3x3( ){ }

			void Identity( );
			
			Matrix3x3 &Rotate( const Quaternion &p_Quat );
			Matrix3x3 &Rotate( const ZED_FLOAT32 p_Angle,
				const Vector3 &p_Axis );
			Matrix3x3 &Rotate( const ZED_FLOAT32 p_Roll,
				const ZED_FLOAT32 p_Pitch,
				const ZED_FLOAT32 p_Yaw );

			Matrix3x3 &RotateX( const ZED_FLOAT32 p_X );
			Matrix3x3 &RotateY( const ZED_FLOAT32 p_Y );
			Matrix3x3 &RotateZ( const ZED_FLOAT32 p_Z );

			Matrix3x3 &Scale( const ZED_FLOAT32 p_Scale );
			Matrix3x3 &Scale( const Vector3 &p_Scale );

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
			void GetRow( const ZED_MEMSIZE p_Index, Vector3 &p_Row ) const;

			// Get/Set columns
			void SetColumns( const Vector3 &p_Column1,
				const Vector3 &p_Column2,
				const Vector3 &p_Column3 );
			void GetColumns( Vector3 &p_Column1, Vector3 &p_Column2,
				Vector3 &p_Column3 ) const;
			void GetColumn( const ZED_MEMSIZE p_Index,
				Vector3 &p_Column ) const;

			// Get the complete matrix as an array (assuming array is of
			// correct size)
			ZED_INLINE ZED_FLOAT32 *GetMatrix( ) const
			{
				ZED_FLOAT32 *pRaw = new ZED_FLOAT32[ 9 ];
				for( ZED_MEMSIZE i = 0; i < 9; i++ )
				{
					pRaw[ i ] = m_M[ i ];
				}

				return pRaw;
			}

			// If any elements in the matrix are near-zero, set them as zero
			// to avoid floating point problems
			void Clean( );

			Matrix3x3 &Transpose( );
			void Transpose( Matrix3x3 &p_Matrix ) const;
			Matrix3x3 &TransposeOf( const Matrix3x3 &p_Transpose );

			Matrix3x3 &Inverse( );
			void Inverse( Matrix3x3 &p_Matrix ) const;
			Matrix3x3 &InverseOf( const Matrix3x3 &p_Inverse );

			Matrix3x3 Adjoint( ) const;
			ZED_FLOAT32 Determinate( ) const;
			ZED_FLOAT32 Trace( ) const;

			// Operator overloading
			// -Debugging-
			friend System::Writer &operator<<( System::Writer &p_Out,
				const Matrix3x3 &p_Source );

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
			Matrix3x3 operator*( const ZED_FLOAT32 p_Scalar ) const;
			// Column-wise multiplication
			Vector3 operator*( const Vector3 &p_Vector ) const;

			// -Assignment-
			Matrix3x3 &operator+=( const Matrix3x3 &p_Other );
			Matrix3x3 &operator-=( const Matrix3x3 &p_Other );
			Matrix3x3 &operator*=( const Matrix3x3 &p_Other );
			Matrix3x3 &operator*=( const ZED_FLOAT32 p_Scalar );

			// -Manipulators-
			ZED_INLINE operator ZED_FLOAT32*( ){ return m_M; }
			ZED_INLINE ZED_FLOAT32 &operator( )( const ZED_MEMSIZE p_Row,
				const ZED_MEMSIZE p_Column )
				{ return m_M[ p_Row+( p_Column*3 ) ]; }
			
			// -Accessors-
			ZED_INLINE operator const ZED_FLOAT32*( ) const { return m_M; }
			ZED_INLINE ZED_FLOAT32 operator( )( const ZED_MEMSIZE p_Row,
				const ZED_MEMSIZE p_Column ) const
				{ return m_M[ p_Row+( p_Column*3 ) ]; }

			// -Manipulate-
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const ZED_MEMSIZE p_Index )
				{ return m_M[ p_Index ]; }
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const int p_Index )
				{ return m_M[ p_Index ]; }
			// -Access-
			ZED_INLINE ZED_FLOAT32 operator[ ](
				const ZED_MEMSIZE p_Index ) const
				{ return m_M[ p_Index ]; }
			ZED_INLINE ZED_FLOAT32 operator[ ]( const int p_Index ) const
				{ return m_M[ p_Index ]; }

		private:
			/**
				\brief The Matrix is stored in column-major form

				| 0 3 6 |\n
				| 1 4 7 |\n
				| 2 5 8 |\n
			*/
			ZED_FLOAT32 m_M[ 9 ];
		};
	}
}

#endif
