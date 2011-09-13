#ifndef __ZED_ARITHMETIC_MATRIX4X4_HPP__
#define __ZED_ARITHMETIC_MATRIX4X4_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		class Matrix3x3;
		class Quaternion;

		class Matrix4x4
		{
		public:
			// Constructors
			ZED_INLINE Matrix4x4( ){ Identity( ); }
			ZED_INLINE ~Matrix4x4( ){ }
			ZED_EXPLICIT Matrix4x4( const Quaternion &p_Quat );

			Matrix4x4 &Clone( ) const;
			void Copy( const Matrix4x4 &p_Original );

			void Identity( );

			Matrix4x4 &Rotate( const Quaternion &p_Quat );
			Matrix4x4 &Rotate( const ZED_FLOAT32 p_Angle,
				const Vector3 &p_Axis );
			Matrix4x4 &Rotate( const ZED_FLOAT32 p_Roll,
				const ZED_FLOAT32 p_Pitch,
				const ZED_FLOAT32 p_Yaw );
			Matrix4x4 &Rotate( const Matrix3x3 &p_Matrix );

			Matrix4x4 &RotateX( const ZED_FLOAT32 p_X );
			Matrix4x4 &RotateY( const ZED_FLOAT32 p_Y );
			Matrix4x4 &RotateZ( const ZED_FLOAT32 p_Z );

			Matrix4x4 &Scale( const ZED_FLOAT32 p_Value );
			Matrix4x4 &Scale( const Vector3 &p_Scale );

			Matrix4x4 &ScaleX( const ZED_FLOAT32 p_X );
			Matrix4x4 &ScaleY( const ZED_FLOAT32 p_Y );
			Matrix4x4 &ScaleZ( const ZED_FLOAT32 p_Z );

			ZED_BOOL IsZero( ) const;
			ZED_BOOL IsIdentity( ) const;

			// Accessors/Manipulators [rows]
			void SetRows( const Vector4 &p_Row1, const Vector4 &p_Row2,
				const Vector4 &p_Row3, const Vector4 &p_Row4 );
			void GetRows( Vector4 &p_Row1, Vector4 &p_Row2,
				Vector4 &p_Row3, Vector4 &p_Row4 ) const;
			Vector4 GetRow( const ZED_UINT32 p_Index ) const;

			// Accessors/Manipulators [columns]
			void SetColumns( const Vector4 &p_Column1,
				const Vector4 &p_Column2,
				const Vector4 &p_Column3, const Vector4 &p_Column4 );
			void GetColumns( Vector4 &p_Column1, Vector4 &p_Column2,
				Vector4 &p_Column3, Vector4 &p_Column4 ) const;
			Vector4 GetColumn( const ZED_UINT32 p_Index ) const;

			ZED_FLOAT32 *GetMatrix( ) const
			// Get the raw matrix data
			{
				ZED_FLOAT32 *pReturn = new ZED_FLOAT32[ 16 ];
				for( ZED_MEMSIZE i = 0; i < 16; i++ )
				{
					pReturn[ i ] = m_M[ i ];
				}

				return pReturn;
			}

			// If any elements are close to zero, set them to zero
			void Clean( );

			Matrix4x4 &Transpose( );
			void Transpose( Matrix4x4 &p_Matrix ) const;
			// Offer a way to modify the current matrix without breaking
			// another
			Matrix4x4 &TransposeOf( const Matrix4x4 &p_Transpose );

			Matrix4x4 &AffineInverse( );
			Matrix4x4 &Translate( const Vector3 &p_Translate );
			Vector3 &Transform( const Vector3 &p_Point ) const;
			ZED_FLOAT32 Trace( ) const;

			// Operator overloads
			// - Debugging-
			friend System::Writer &operator<<( System::Writer &p_Out,
				const Matrix4x4 &p_Source );

			// -Equality-
			ZED_BOOL operator==( const Matrix4x4 &p_Other ) const;
			ZED_BOOL operator!=( const Matrix4x4 &p_Other ) const;

			// -Addition/Subtraction-
			Matrix4x4 operator+( const Matrix4x4 &p_Other ) const;
			Matrix4x4 operator-( const Matrix4x4 &p_Other ) const;

			// -Unary negation-
			Matrix4x4 operator-( ) const;

			// -Multiplication-
			Matrix4x4 operator*( const Matrix4x4 &p_Other ) const;
			Matrix4x4 operator*( const ZED_FLOAT32 p_Scalar ) const;
			friend Matrix4x4 operator*( const ZED_FLOAT32 p_Scalar,
				const Matrix4x4 &p_Self );
			// --Column-wise multiplication--
			Vector4 operator*( const Vector4 &p_Vec ) const;
			// --Row-wise multiplication--
			friend Vector4 operator*( const Vector4 &p_Vec,
				const Matrix4x4 &p_Self );

			// -Assignment-
			Matrix4x4 &operator+=( const Matrix4x4 &p_Other );
			Matrix4x4 &operator-=( const Matrix4x4 &p_Other );
			Matrix4x4 &operator*=( const Matrix4x4 &p_Other );
			Matrix4x4 &operator*=( const ZED_FLOAT32 p_Scalar );

			// -Manipulators-
			ZED_INLINE operator ZED_FLOAT32*( ){ return m_M; }
			ZED_INLINE operator const ZED_FLOAT32*( ){ return m_M; }

			// Row-Column accessors and manipulators
			ZED_FLOAT32 &operator( )( const ZED_UINT32 p_Row,
				const ZED_UINT32 p_Column );
			ZED_FLOAT32 operator( )( const ZED_UINT32 p_Row,
				const ZED_UINT32 p_Column ) const;

			// -Manipulate-
			ZED_INLINE ZED_FLOAT32 &operator[ ]( const ZED_UINT32 p_Index )
				{ return m_M[ p_Index ]; }
			// -Access-
			ZED_INLINE ZED_FLOAT32 operator[ ](
				const ZED_UINT32 p_Index ) const
				{ return m_M[ p_Index ]; }

		private:
			ZED_FLOAT32 m_M[ 16 ];

			// No implicit copying or cloning
			Matrix4x4( const Matrix4x4 &p_Cpoy );
			Matrix4x4 &operator=( const Matrix4x4 &p_Clone );
		};
	}
}

#endif

