#include <Matrix4x4.hpp>
#include <Arithmetic.hpp>
#include <Matrix3x3.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Matrix4x4::Matrix4x4( const Quaternion &p_Quat )
		{
		}

		void Matrix4x4::Identity( )
		{
			m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] =
				m_M[ 15 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] =
				m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] =
				m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] =
				m_M[ 13 ] = m_M[ 14 ] = 0.0f;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Quaternion &p_Quat )
		{
			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			// Set the right and bottom column and row to 0.0f
			// Setting the last value to 1.0f
			m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;

			ZED_FLOAT32 Cos = 0.0f, Sin = 0.0f;
			Arithmetic::SinCos( p_Angle, Sin, Cos );

			ZED_FLOAT32 Tan = 1.0f - Cos;
			Vector3 Axis( p_Axis );

			// Intermediate values
			ZED_FLOAT32 TanX = Tan*Axis[ 0 ];
			ZED_FLOAT32 TanY = Tan*Axis[ 1 ];
			ZED_FLOAT32 TanZ = Tan*Axis[ 2 ];

			ZED_FLOAT32 SinX = Sin*Axis[ 0 ];
			ZED_FLOAT32 SinY = Sin*Axis[ 1 ];
			ZED_FLOAT32 SinZ = Sin*Axis[ 2 ];

			ZED_FLOAT32 TanXY = TanX*Axis[ 1 ];
			ZED_FLOAT32 TanYZ = TanY*Axis[ 2 ];
			ZED_FLOAT32 TanXZ = TanX*Axis[ 2 ];

			// Set up the matrix
			m_M[ 0 ] = TanX*Axis[ 0 ] + Cos;
			m_M[ 4 ] = TanXY - SinZ;
			m_M[ 8 ] = TanXZ + SinY;
			m_M[ 1 ] = TanXY + SinZ;
			m_M[ 5 ] = TanY*Axis[ 1 ] + Cos;
			m_M[ 9 ] = TanYZ - SinX;
			m_M[ 2 ] = TanXZ - SinY;
			m_M[ 6 ] = TanYZ + SinX;
			m_M[ 10 ] = TanZ*Axis[ 2 ] + Cos;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Roll,
			const ZED_FLOAT32 p_Pitch, const ZED_FLOAT32 p_Yaw )
		{
			// Unroll the concatenation of the X, Y and Z rotation matrices
			ZED_FLOAT32 CosX = 0.0f, SinX = 0.0f;
			ZED_FLOAT32 CosY = 0.0f, SinY = 0.0f;
			ZED_FLOAT32 CosZ = 0.0f, SinZ = 0.0f;

			ZED::Arithmetic::SinCos( p_Yaw, SinX, CosX );
			ZED::Arithmetic::SinCos( p_Pitch, SinY, CosY );
			ZED::Arithmetic::SinCos( p_Roll, SinZ, CosZ );

			m_M[ 0 ] = CosY*CosZ;
			m_M[ 4 ] = -( CosY*SinZ );
			m_M[ 8 ] = SinY;
			
			m_M[ 1 ] = ( SinX*SinY*CosZ ) + ( CosX*CosZ );
			m_M[ 5 ] = -( SinX*SinY*SinZ ) + ( CosX*CosZ );
			m_M[ 9 ] = -( SinX*CosY );

			m_M[ 2 ] = -( CosX*SinY*CosZ ) + ( SinX*SinZ );
			m_M[ 6 ] = ( CosX*SinY*SinZ ) + ( SinX*CosZ );
			m_M[ 10 ] = CosX*CosY;

			// Zero-out the last row and last column, sans the last element in
			// the matrix which should be 1.0f
			m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Matrix3x3 &p_Rotation )
		{
			// Assume the matrix passed in is a rotation matrix
			m_M[ 0 ] = p_Rotation[ 0 ];
			m_M[ 1 ] = p_Rotation[ 1 ];
			m_M[ 2 ] = p_Rotation[ 2 ];
			m_M[ 3 ] = 0.0f;
			m_M[ 4 ] = p_Rotation[ 3 ];
			m_M[ 5 ] = p_Rotation[ 4 ];
			m_M[ 6 ] = p_Rotation[ 5 ];
			m_M[ 7 ] = 0.0f;
			m_M[ 8 ] = p_Rotation[ 6 ];
			m_M[ 9 ] = p_Rotation[ 7 ];
			m_M[ 10 ] = p_Rotation[ 8 ];
			m_M[ 11 ] = 0.0f;
			m_M[ 12 ] = 0.0f;
			m_M[ 13 ] = 0.0f;
			m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateX( const ZED_FLOAT32 p_X )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_X, SinTheta, CosTheta );

			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = 0.0f;
			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = CosTheta;
			m_M[ 6 ] = SinTheta;
			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = -SinTheta;
			m_M[ 10 ] = CosTheta;

			// Zero-out the last row and last column, sans the last element in
			// the matrix which should be 1.0f
			m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateY( const ZED_FLOAT32 p_Y )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_Y, SinTheta, CosTheta );

			m_M[ 0 ] = CosTheta;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = -SinTheta;
			m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = 1.0f;
			m_M[ 6 ] = 0.0f;
			m_M[ 8 ] = SinTheta;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = CosTheta;

			// Zero-out the last row and last column, sans the last element in
			// the matrix which should be 1.0f
			m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateZ( const ZED_FLOAT32 p_Z )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_Z, SinTheta, CosTheta );

			m_M[ 0 ] = CosTheta;
			m_M[ 1 ] = SinTheta;
			m_M[ 2 ] = 0.0f;
			m_M[ 4 ] = -SinTheta;
			m_M[ 5 ] = CosTheta;
			m_M[ 6 ] = 0.0f;
			m_M[ 8 ] = 0.0f;
			m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = 1.0f;

			// Zero-out the last row and last column, sans the last element in
			// the matrix which should be 1.0f
			m_M[ 3 ] = m_M[ 7 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const ZED_FLOAT32 p_Scale )
		{
			// Set the diagonal to the scale value
			m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = p_Scale;

			// Set all other elements to 0.0f, except for the last
			// element in the array
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const Vector3 &p_Scale )
		{
			m_M[ 0 ] = p_Scale[ 0 ];
			m_M[ 4 ] = p_Scale[ 1 ];
			m_M[ 8 ] = p_Scale[ 2 ];

			// Set all other elements to 0.0f, except for the last
			// element in the array
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleX( const ZED_FLOAT32 p_X )
		{
			m_M[ 0 ] = p_X;

			// Set all other elements to 0.0f, except for the last
			// element in the array
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = m_M[ 10 ] = m_M[ 11 ] =
				m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleY( const ZED_FLOAT32 p_Y )
		{
			m_M[ 4 ] = p_Y;

			// Set all other elements to 0.0f, except for the last
			// element in the array
			m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = m_M[ 10 ] = m_M[ 11 ] =
				m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleZ( const ZED_FLOAT32 p_Z )
		{
			m_M[ 8 ] = p_Z;

			// Set all other elements to 0.0f, except for the last
			// element in the array
			m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 5 ] =
				m_M[ 6 ] = m_M[ 7 ] = m_M[ 9 ] = m_M[ 10 ] = m_M[ 11 ] =
				m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		ZED_BOOL Matrix4x4::IsZero( ) const
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				if( !ZED::Arithmetic::IsZero( m_M[ i ] ) )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix4x4::IsIdentity( ) const
		{
			return( ZED::Arithmetic::Equal( 1.0f, m_M[ 0 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_M[ 5 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_M[ 10 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_M[ 15 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 1 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 2 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 3 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 4 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 6 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 7 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 8 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 9 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 11 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 12 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 13 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 14 ] ) );
		}

		void Matrix4x4::SetRows( const Vector4 &p_Row1, const Vector4 &p_Row2,
			const Vector4 &p_Row3, const Vector4 &p_Row4 )
		{
			m_M[ 0 ] = p_Row1[ 0 ];
			m_M[ 4 ] = p_Row1[ 1 ];
			m_M[ 8 ] = p_Row1[ 2 ];
			m_M[ 12 ] = p_Row1[ 3 ];

			m_M[ 1 ] = p_Row2[ 0 ];
			m_M[ 5 ] = p_Row2[ 1 ];
			m_M[ 9 ] = p_Row2[ 2 ];
			m_M[ 13 ] = p_Row2[ 3 ];

			m_M[ 2 ] = p_Row3[ 0 ];
			m_M[ 6 ] = p_Row3[ 1 ];
			m_M[ 10 ] = p_Row3[ 2 ];
			m_M[ 14 ] = p_Row3[ 3 ];

			m_M[ 3 ] = p_Row4[ 0 ];
			m_M[ 7 ] = p_Row4[ 1 ];
			m_M[ 11 ] = p_Row4[ 2 ];
			m_M[ 15 ] = p_Row4[ 3 ];
		}

		void Matrix4x4::GetRows( Vector4 &p_Row1, Vector4 &p_Row2,
			Vector4 &p_Row3, Vector4 &p_Row4 ) const
		{
			p_Row1[ 0 ] = m_M[ 0 ];
			p_Row1[ 1 ] = m_M[ 4 ];
			p_Row1[ 2 ] = m_M[ 8 ];
			p_Row1[ 3 ] = m_M[ 12 ];

			p_Row2[ 0 ] = m_M[ 1 ];
			p_Row2[ 1 ] = m_M[ 5 ];
			p_Row2[ 2 ] = m_M[ 9 ];
			p_Row2[ 3 ] = m_M[ 13 ];

			p_Row3[ 0 ] = m_M[ 2 ];
			p_Row3[ 1 ] = m_M[ 6 ];
			p_Row3[ 2 ] = m_M[ 10 ];
			p_Row3[ 3 ] = m_M[ 14 ];
			
			p_Row4[ 0 ] = m_M[ 3 ];
			p_Row4[ 1 ] = m_M[ 7 ];
			p_Row4[ 2 ] = m_M[ 11 ];
			p_Row4[ 3 ] = m_M[ 15 ];
		}

		void Matrix4x4::GetRow( const ZED_MEMSIZE p_RowNumber,
			Vector4 &p_Row ) const
		{
			ZED_UINT32 BoundsCheck = 0;

			// Make sure the value is valid
			if( p_RowNumber < 0 )
			{
				BoundsCheck = 0;
			}
			else if( p_RowNumber > 4 )
			{
				BoundsCheck = 4;
			}
			else
			{
				BoundsCheck = p_RowNumber;
			}

			p_Row.Set( m_M[ BoundsCheck ], m_M[ BoundsCheck+4 ],
				m_M[ BoundsCheck+8 ], m_M[ BoundsCheck+12 ] );
		}

		void Matrix4x4::SetColumns( const Vector4 &p_Column1,
			const Vector4 &p_Column2, const Vector4 &p_Column3,
			const Vector4 &p_Column4 )
		{
			m_M[ 0 ] = p_Column1[ 0 ];
			m_M[ 1 ] = p_Column1[ 1 ];
			m_M[ 2 ] = p_Column1[ 2 ];
			m_M[ 3 ] = p_Column1[ 3 ];

			m_M[ 4 ] = p_Column2[ 0 ];
			m_M[ 5 ] = p_Column2[ 1 ];
			m_M[ 6 ] = p_Column2[ 2 ];
			m_M[ 7 ] = p_Column2[ 3 ];
			
			m_M[ 8 ] = p_Column3[ 0 ];
			m_M[ 9 ] = p_Column3[ 1 ];
			m_M[ 10 ] = p_Column3[ 2 ];
			m_M[ 11 ] = p_Column3[ 3 ];

			m_M[ 12 ] = p_Column4[ 0 ];
			m_M[ 13 ] = p_Column4[ 1 ];
			m_M[ 14 ] = p_Column4[ 2 ];
			m_M[ 15 ] = p_Column4[ 3 ];
		}

		void Matrix4x4::GetColumns( Vector4 &p_Column1, Vector4 &p_Column2,
			Vector4 &p_Column3, Vector4 &p_Column4 ) const
		{
			p_Column1[ 0 ] = m_M[ 0 ];
			p_Column1[ 1 ] = m_M[ 1 ];
			p_Column1[ 2 ] = m_M[ 2 ];
			p_Column1[ 3 ] = m_M[ 3 ];

			p_Column2[ 0 ] = m_M[ 4 ];
			p_Column2[ 1 ] = m_M[ 5 ];
			p_Column2[ 2 ] = m_M[ 6 ];
			p_Column2[ 3 ] = m_M[ 7 ];

			p_Column3[ 0 ] = m_M[ 8 ];
			p_Column3[ 1 ] = m_M[ 9 ];
			p_Column3[ 2 ] = m_M[ 10 ];
			p_Column3[ 3 ] = m_M[ 11 ];

			p_Column4[ 0 ] = m_M[ 12 ];
			p_Column4[ 1 ] = m_M[ 13 ];
			p_Column4[ 2 ] = m_M[ 14 ];
			p_Column4[ 3 ] = m_M[ 15 ];
		}

		void Matrix4x4::GetColumn( const ZED_MEMSIZE p_ColumnNumber,
			Vector4 &p_Column ) const
		{
			ZED_UINT32 BoundsCheck = 0;

			if( p_ColumnNumber < 0 )
			{
				BoundsCheck = 0;
			}
			else if( p_ColumnNumber > 4 )
			{
				BoundsCheck = 4;
			}
			else
			{
				BoundsCheck = p_ColumnNumber;
			}

			p_Column.Set( m_M[ BoundsCheck*4 ],
							m_M[ ( BoundsCheck*4 ) + 1 ],
							m_M[ ( BoundsCheck*4 ) + 2 ],
							m_M[ ( BoundsCheck*4 ) + 3 ] );
		}

		void Matrix4x4::Clean( )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				if( ZED::Arithmetic::IsZero( m_M[ i ] ) )
				{
					m_M[ i ] = 0.0f;
				}
			}
		}

		Matrix4x4 &Matrix4x4::Transpose( )
		{
			ZED_FLOAT32 Temp = m_M[ 1 ];
			m_M[ 1 ] = m_M[ 4 ];
			m_M[ 4 ] = Temp;

			Temp = m_M[ 8 ];
			m_M[ 8 ] = m_M[ 2 ];
			m_M[ 2 ] = Temp;

			Temp = m_M[ 12 ];
			m_M[ 12 ] = m_M[ 3 ];
			m_M[ 3 ] = Temp;

			Temp = m_M[ 9 ];
			m_M[ 9 ] = m_M[ 6 ];
			m_M[ 6 ] = Temp;

			Temp = m_M[ 15 ];
			m_M[ 15 ] = m_M[ 7 ];
			m_M[ 7 ] = Temp;

			Temp = m_M[ 14 ];
			m_M[ 14 ] = m_M[ 11 ];
			m_M[ 11 ] = Temp;

			return *this;
		}

		void Matrix4x4::Transpose( Matrix4x4 &p_Matrix ) const
		{
			p_Matrix.TransposeOf( *this );
		}

		Matrix4x4 &Matrix4x4::TransposeOf( const Matrix4x4 &p_Transpose )
		{
			// Copy the matrix across
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				m_M[ i ] = p_Transpose[ i ];
			}

			// Transpose the current matrix
			Transpose( );

			// Finally, return the transpose of the copy of the matrix that
			// was passed in, which is now this matrix
			return *this;
		}

		Matrix4x4 &Matrix4x4::AffineInverse( )
		{
			// Get the determinate of the upper-left matrix
			ZED_FLOAT32 CoFactor0 = m_M[ 5 ]*m_M[ 10 ] - m_M[ 6 ]*m_M[ 9 ];
			ZED_FLOAT32 CoFactor4 = m_M[ 2 ]*m_M[ 9 ] - m_M[ 1 ]*m_M[ 10 ];
			ZED_FLOAT32 CoFactor8 = m_M[ 1 ]*m_M[ 6 ] - m_M[ 2 ]*m_M[ 5 ];
			ZED_FLOAT32 Det = m_M[ 0 ]*CoFactor0 + m_M[ 4 ]*CoFactor4 +
				m_M[ 8 ]*CoFactor8;

			Matrix4x4 Temp;

			if( ZED::Arithmetic::IsZero( Det ) )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[Matrix4x4::AffineInverse]  Singular Matrix!\n\n" );
				return *this;
			}

			// Create adjunct matrix, then multiply by 1/Det to get the upper
			// 3x3 matrix
			ZED_FLOAT32 InvDet = 1.0f/Det;

			Temp.m_M[ 0 ] = InvDet*CoFactor0;
			Temp.m_M[ 1 ] = InvDet*CoFactor4;
			Temp.m_M[ 2 ] = InvDet*CoFactor8;

			Temp.m_M[ 4 ] = InvDet*( m_M[ 6 ]*m_M[ 8 ] - m_M[ 4 ]*m_M[ 10 ] );
			Temp.m_M[ 5 ] = InvDet*( m_M[ 0 ]*m_M[ 10 ] - m_M[ 2 ]*m_M[ 8 ] );
			Temp.m_M[ 6 ] = InvDet*( m_M[ 2 ]*m_M[ 4 ] - m_M[ 0 ]*m_M[ 6 ] );

			Temp.m_M[ 8 ] = InvDet*( m_M[ 4 ]*m_M[ 9 ] - m_M[ 5 ]*m_M[ 8 ] );
			Temp.m_M[ 9 ] = InvDet*( m_M[ 1 ]*m_M[ 8 ] - m_M[ 0 ]*m_M[ 9 ] );
			Temp.m_M[ 10 ] = InvDet*( m_M[ 0 ]*m_M[ 5 ] - m_M[ 1 ]*m_M[ 4 ] );

			// Multiply -Trans by the inverted 3x3 to get the inverse
			Temp.m_M[ 12 ] = -Temp.m_M[ 0 ]*m_M[ 12 ] -
							 Temp.m_M[ 4 ]*m_M[ 13 ] -
							 Temp.m_M[ 8 ]*m_M[ 14 ];
			Temp.m_M[ 13 ] = -Temp.m_M[ 1 ]*m_M[ 12 ] -
							 Temp.m_M[ 5 ]*m_M[ 13 ] -
							 Temp.m_M[ 9 ]*m_M[ 14 ];
			Temp.m_M[ 14 ] = -Temp.m_M[ 2 ]*m_M[ 12 ] -
							 Temp.m_M[ 6 ]*m_M[ 13 ] -
							 Temp.m_M[ 10 ]*m_M[ 14 ];

			*this = Temp;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Translate( const Vector3 &p_Translate )
		{
			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = 0.0f;
			m_M[ 5 ] = 1.0f;
			m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = 0.0f;
			m_M[ 10 ] = 1.0f;
			m_M[ 11 ] = 0.0f;
			m_M[ 12 ] = p_Translate[ 0 ];
			m_M[ 13 ] = p_Translate[ 1 ];
			m_M[ 14 ] = p_Translate[ 2 ];
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Vector3 &Matrix4x4::Transform( const Vector3 &p_Point ) const
		{
			Vector3 Temp;

			Temp[ 0 ] = m_M[ 0 ]*Temp[ 0 ] + m_M[ 4 ]*Temp[ 1 ] +
						m_M[ 8 ]*Temp[ 2 ] + m_M[ 12 ];
			Temp[ 1 ] = m_M[ 1 ]*Temp[ 0 ] + m_M[ 5 ]*Temp[ 1 ] +
						m_M[ 9 ]*Temp[ 2 ] + m_M[ 13 ];
			Temp[ 3 ] = m_M[ 2 ]*Temp[ 0 ] + m_M[ 6 ]*Temp[ 1 ] +
						m_M[ 10 ]*Temp[ 2 ] + m_M[ 14 ];

			return Temp;
		}

		ZED_FLOAT32 Matrix4x4::Trace( ) const
		{
			return m_M[ 0 ] + m_M[ 5 ] + m_M[ 10 ] + m_M[ 15 ];
		}

		System::Writer &operator<<( System::Writer &p_Out,
			const Matrix4x4 &p_Source )
		{
			for( ZED_MEMSIZE i = 0; i < 4; i++ )
			{
				p_Out << "| ";
				for( ZED_MEMSIZE j = 0; j < 4; j++ )
				{
					p_Out << p_Source[ j*4 + i ] << ' ';
				}
				p_Out << "|" << std::endl;
			}

			return p_Out;
		}

		ZED_BOOL Matrix4x4::operator==( const Matrix4x4 &p_Other ) const
		{
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				if( !ZED::Arithmetic::Equal( m_M[ i ], p_Other.m_M[ i ] ) )
				{
					return ZED_FALSE;
				}
			}
			return ZED_TRUE;
		}

		ZED_BOOL Matrix4x4::operator!=( const Matrix4x4 &p_Other ) const
		{
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				if( ZED::Arithmetic::Equal( m_M[ i ], p_Other.m_M[ i ] ) )
				{
					return ZED_FALSE;
				}
			}
			return ZED_TRUE;
		}

		Matrix4x4 Matrix4x4::operator+( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Add;

			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				Add.m_M[ i ] = m_M[ i ] + p_Other.m_M[ i ];
			}

			return Add;
		}

		Matrix4x4 Matrix4x4::operator-( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Sub;

			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				Sub.m_M[ i ] = m_M[ i ] - p_Other.m_M[ i ];
			}

			return Sub;
		}

		Matrix4x4 Matrix4x4::operator-( ) const
		{
			Matrix4x4 Neg;

			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				Neg.m_M[ i ] = -m_M[ i ];
			}

			return Neg;
		}

		Matrix4x4 Matrix4x4::operator*( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Mul;

			Mul.m_M[ 0 ] =	m_M[ 0 ]*p_Other.m_M[ 0 ] +
							m_M[ 4 ]*p_Other.m_M[ 1 ] +
							m_M[ 8 ]*p_Other.m_M[ 2 ] +
							m_M[ 12 ]*p_Other.m_M[ 3 ];
			Mul.m_M[ 1 ] =	m_M[ 1 ]*p_Other.m_M[ 0 ] +
							m_M[ 5 ]*p_Other.m_M[ 1 ] +
							m_M[ 9 ]*p_Other.m_M[ 2 ] +
							m_M[ 13 ]*p_Other.m_M[ 3 ];
			Mul.m_M[ 2 ] =	m_M[ 2 ]*p_Other.m_M[ 0 ] +
							m_M[ 6 ]*p_Other.m_M[ 1 ] +
							m_M[ 10 ]*p_Other.m_M[ 2 ] +
							m_M[ 14 ]*p_Other.m_M[ 3 ];
			Mul.m_M[ 3 ] =	m_M[ 3 ]*p_Other.m_M[ 0 ] +
							m_M[ 7 ]*p_Other.m_M[ 1 ] +
							m_M[ 11 ]*p_Other.m_M[ 2 ] +
							m_M[ 15 ]*p_Other.m_M[ 3 ];

			Mul.m_M[ 4 ] =	m_M[ 0 ]*p_Other.m_M[ 4 ] +
							m_M[ 4 ]*p_Other.m_M[ 5 ] +
							m_M[ 8 ]*p_Other.m_M[ 6 ] +
							m_M[ 12 ]*p_Other.m_M[ 7 ];
			Mul.m_M[ 5 ] =	m_M[ 1 ]*p_Other.m_M[ 4 ] +
							m_M[ 5 ]*p_Other.m_M[ 5 ] +
							m_M[ 9 ]*p_Other.m_M[ 6 ] +
							m_M[ 13 ]*p_Other.m_M[ 7 ];
			Mul.m_M[ 6 ] =	m_M[ 2 ]*p_Other.m_M[ 4 ] +
							m_M[ 6 ]*p_Other.m_M[ 5 ] +
							m_M[ 10 ]*p_Other.m_M[ 6 ] +
							m_M[ 14 ]*p_Other.m_M[ 7 ];
			Mul.m_M[ 7 ] =	m_M[ 3 ]*p_Other.m_M[ 4 ] +
							m_M[ 7 ]*p_Other.m_M[ 5 ] +
							m_M[ 11 ]*p_Other.m_M[ 6 ] +
							m_M[ 15 ]*p_Other.m_M[ 7 ];

			Mul.m_M[ 8 ] =	m_M[ 0 ]*p_Other.m_M[ 8 ] +
							m_M[ 4 ]*p_Other.m_M[ 9 ] +
							m_M[ 8 ]*p_Other.m_M[ 10 ] +
							m_M[ 12 ]*p_Other.m_M[ 11 ];
			Mul.m_M[ 9 ] =	m_M[ 1 ]*p_Other.m_M[ 8 ] +
							m_M[ 5 ]*p_Other.m_M[ 9 ] +
							m_M[ 9 ]*p_Other.m_M[ 10 ] +
							m_M[ 13 ]*p_Other.m_M[ 11 ];
			Mul.m_M[ 10 ] =	m_M[ 2 ]*p_Other.m_M[ 8 ] +
							m_M[ 6 ]*p_Other.m_M[ 9 ] +
							m_M[ 10 ]*p_Other.m_M[ 10 ] +
							m_M[ 14 ]*p_Other.m_M[ 11 ];
			Mul.m_M[ 11 ] =	m_M[ 3 ]*p_Other.m_M[ 8 ] +
							m_M[ 7 ]*p_Other.m_M[ 9 ] +
							m_M[ 11 ]*p_Other.m_M[ 10 ] +
							m_M[ 15 ]*p_Other.m_M[ 11 ];

			Mul.m_M[ 12 ] =	m_M[ 0 ]*p_Other.m_M[ 12 ] +
							m_M[ 4 ]*p_Other.m_M[ 13 ] +
							m_M[ 8 ]*p_Other.m_M[ 14 ] +
							m_M[ 12 ]*p_Other.m_M[ 15 ];
			Mul.m_M[ 13 ] =	m_M[ 1 ]*p_Other.m_M[ 12 ] +
							m_M[ 5 ]*p_Other.m_M[ 13 ] +
							m_M[ 9 ]*p_Other.m_M[ 14 ] +
							m_M[ 13 ]*p_Other.m_M[ 15 ];
			Mul.m_M[ 14 ] =	m_M[ 2 ]*p_Other.m_M[ 12 ] +
							m_M[ 6 ]*p_Other.m_M[ 13 ] +
							m_M[ 10 ]*p_Other.m_M[ 14 ] +
							m_M[ 14 ]*p_Other.m_M[ 15 ];
			Mul.m_M[ 15 ] =	m_M[ 3 ]*p_Other.m_M[ 12 ] +
							m_M[ 7 ]*p_Other.m_M[ 13 ] +
							m_M[ 11 ]*p_Other.m_M[ 14 ] +
							m_M[ 15 ]*p_Other.m_M[ 15 ];

			return Mul;
		}

		Matrix4x4 Matrix4x4::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			Matrix4x4 MulScale;

			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				MulScale.m_M[ i ] = m_M[ i ] * p_Scalar;
			}

			return MulScale;
		}

		Matrix4x4 operator*( const ZED_FLOAT32 p_Scalar,
			const Matrix4x4 &p_Self )
		{
			Matrix4x4 ScaleMul;

			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				ScaleMul[ i ] = p_Scalar * p_Self[ i ];
			}

			return ScaleMul;
		}

		Vector4 Matrix4x4::operator*( const Vector4 &p_Vec ) const
		{
			Vector4 MulVec;

			MulVec[ 0 ] =	p_Vec[ 0 ]*m_M[ 0 ] + p_Vec[ 1 ]*m_M[ 4 ] +
							p_Vec[ 2 ]*m_M[ 8 ] + p_Vec[ 3 ]*m_M[ 12 ];
			MulVec[ 1 ] =	p_Vec[ 0 ]*m_M[ 1 ] + p_Vec[ 1 ]*m_M[ 5 ] +
							p_Vec[ 2 ]*m_M[ 9 ] + p_Vec[ 3 ]*m_M[ 13 ];
			MulVec[ 2 ] =	p_Vec[ 0 ]*m_M[ 2 ] + p_Vec[ 1 ]*m_M[ 6 ] +
							p_Vec[ 2 ]*m_M[ 10 ] + p_Vec[ 3 ]*m_M[ 14 ];
			MulVec[ 3 ] =	p_Vec[ 0 ]*m_M[ 3 ] + p_Vec[ 1 ]*m_M[ 7 ] +
							p_Vec[ 2 ]*m_M[ 11 ] + p_Vec[ 3 ]*m_M[ 15 ];

			return MulVec;
		}

		Matrix4x4 &Matrix4x4::operator+=( const Matrix4x4 &p_Other )
		{
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				m_M[ i ] *= p_Other.m_M[ i ];
			}

			return *this;
		}

		Matrix4x4 &Matrix4x4::operator-=( const Matrix4x4 &p_Other )
		{
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				m_M[ i ] -= p_Other.m_M[ i ];
			}
			return *this;
		}

		Matrix4x4 &Matrix4x4::operator*=( const Matrix4x4 &p_Other )
		{
			Matrix4x4 Mul;

			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				m_M[ i ] = Mul.m_M[ 0 ];
			}
			return *this;
		}

		Matrix4x4 &Matrix4x4::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			for( ZED_MEMSIZE i = 0; i < 16; i++ )
			{
				m_M[ i ] *= p_Scalar;
			}

			return *this;
		}

		ZED_FLOAT32 &Matrix4x4::operator( )( const ZED_MEMSIZE p_Row,
			const ZED_MEMSIZE p_Column )
		{
			// No bounds checking!
			//return m_M[ p_Column + ( p_Row*4 ) ];
			return m_M[ p_Row + ( p_Column*4 ) ];
		}

		ZED_FLOAT32 Matrix4x4::operator( )( const ZED_MEMSIZE p_Row,
			const ZED_MEMSIZE p_Column ) const
		{
			// No bounds checking!
			//return m_M[ p_Column + ( p_Row*4 ) ];
			return m_M[ p_Row + ( p_Column*4 ) ];
		}
	}
}
