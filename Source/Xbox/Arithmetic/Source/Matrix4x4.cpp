#include <Matrix4x4.hpp>
#include <Matrix3x3.hpp>
#include <Quaternion.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Matrix4x4::Matrix4x4( const Matrix4x4 &p_Copy )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				m_M[ i ] = p_Copy.m_M[ i ];
			}
		}

		Matrix4x4::Matrix4x4( const Quaternion &p_Quat )
		{
			Rotate( p_Quat );
		}

		void Matrix4x4::Identity( )
		{
			// Maybe this will be a bad idea...
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				m_M[ i ] = 0.0f;
			}

			m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = m_M[ 15 ] = 1.0f;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Quaternion &p_Quat )
		{
			zedAssert( p_Quat.IsUnit( ) );

			ZED_FLOAT32 XS, YS, ZS, WX, WY, WZ, XX, XY, XZ, YY, YZ, ZZ;

			XS = p_Quat[ 0 ] + p_Quat[ 0 ];
			YS = p_Quat[ 1 ] + p_Quat[ 1 ];
			ZS = p_Quat[ 2 ] + p_Quat[ 2 ];
			WX = p_Quat[ 3 ] * XS;
			WY = p_Quat[ 3 ] * YS;
			WZ = p_Quat[ 3 ] * ZS;
			XX = p_Quat[ 0 ] * XS;
			XY = p_Quat[ 0 ] * YS;
			XZ = p_Quat[ 0 ] * ZS;
			YY = p_Quat[ 1 ] * YS;
			YZ = p_Quat[ 1 ] * ZS;
			ZZ = p_Quat[ 2 ] * ZS;

			m_M[ 0 ] = 1.0f - ( YY + ZZ );
			m_M[ 4 ] = XY - WZ;
			m_M[ 8 ] = XZ + WY;
			m_M[ 12 ] = 0.0f;

			m_M[ 1 ] = XY + WZ;
			m_M[ 5 ] = 1.0f - ( XX + ZZ );
			m_M[ 9 ] = YZ - WX;
			m_M[ 13 ] = 0.0f;

			m_M[ 2 ] = XZ - WY;
			m_M[ 6 ] = YZ + WX;
			m_M[ 10 ] = 1.0f - ( XX + YY );
			m_M[ 14 ] = 0.0f;

			m_M[ 3 ] = 0.0f;
			m_M[ 7 ] = 0.0f;
			m_M[ 11 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			ZED_FLOAT32 Cos, Sin, Tan;
			Arithmetic::SinCos( p_Angle, Sin, Cos );
			Tan = 1.0f - Cos;

			Vector3 Axis = p_Axis;
			Axis.Normalise( );

			// Shorthand variables
			ZED_FLOAT32 TX = Tan*Axis[ 0 ];
			ZED_FLOAT32 TY = Tan*Axis[ 1 ];
			ZED_FLOAT32 TZ = Tan*Axis[ 2 ];
			ZED_FLOAT32 SX = Sin*Axis[ 0 ];
			ZED_FLOAT32 SY = Sin*Axis[ 1 ];
			ZED_FLOAT32 SZ = Sin*Axis[ 2 ];
			ZED_FLOAT32 TXY = TX*Axis[ 1 ];
			ZED_FLOAT32 TYZ = TY*Axis[ 2 ];
			ZED_FLOAT32 TXZ = TX*Axis[ 2 ];

			// Finally, set up the matrix
			m_M[ 0 ] = TX*Axis[ 0 ] + Cos;
			m_M[ 4 ] = TXY - SZ;
			m_M[ 8 ] = TXZ + SY;
			m_M[ 12 ] = 0.0f;

			m_M[ 1 ] = TXY + SZ;
			m_M[ 5 ] = TY*Axis[ 1 ] + Cos;
			m_M[ 9 ] = TYZ - SX;
			m_M[ 13 ] = 0.0f;

			m_M[ 2 ] = TXZ - SY;
			m_M[ 6 ] = TYZ + SX;
			m_M[ 10 ] = TZ*Axis[ 2 ] + Cos;
			m_M[ 14 ] = 0.0f;

			m_M[ 3 ] = 0.0f;
			m_M[ 7 ] = 0.0f;
			m_M[ 11 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const ZED_FLOAT32 p_Z,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_X )
		{
			ZED_FLOAT32 SinX, CosX, SinY, CosY, SinZ, CosZ;

			Arithmetic::SinCos( p_X, SinX, CosX );
			Arithmetic::SinCos( p_Y, SinY, CosY );
			Arithmetic::SinCos( p_Z, SinZ, CosZ );

			m_M[ 0 ] = ( CosY * CosZ );
			m_M[ 4 ] = -( CosY * SinZ );
			m_M[ 8 ] = SinY;
			m_M[ 12 ] = 0.0f;

			m_M[ 1 ] = ( SinX * SinY * CosZ ) + ( CosX * SinZ );
			m_M[ 5 ] = -( SinX * SinY * SinZ ) + ( CosX * CosZ );
			m_M[ 9 ] = -( SinX * CosY );
			m_M[ 13 ] = 0.0f;

			m_M[ 2 ] = -( CosX * SinY * SinZ ) + ( SinX * SinZ );
			m_M[ 6 ] = ( CosX * SinY * SinZ ) + ( SinX * CosZ );
			m_M[ 10 ] = ( CosX * CosY );
			m_M[ 14 ] = 0.0f;

			m_M[ 3 ] = 0.0f;
			m_M[ 7 ] = 0.0f;
			m_M[ 11 ] = 0.0f;
			m_M[ 15 ] = 1.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Rotate( const Matrix3x3 &p_Matrix )
		{
			// Just copy the matrix into the upper-3^2 part of the
			// 4x4 matrix
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
			ZED_FLOAT32 SinX, CosX;
			Arithmetic::SinCos( p_X, SinX, CosX );

			m_M[ 0 ] = m_M[ 15 ] = 1.0f;

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 7 ] = m_M[ 8 ] =
				m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;

			m_M[ 5 ] = SinX;
			m_M[ 9 ] = CosX;
			m_M[ 6 ] = -SinX;
			m_M[ 10 ] = CosX;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateY( const ZED_FLOAT32 p_Y )
		{
			ZED_FLOAT32 SinY, CosY;
			Arithmetic::SinCos( p_Y, SinY, CosY );

			m_M[ 5 ] = m_M[ 15 ] = 1.0f;
			m_M[ 1 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] = m_M[ 9 ] =
				m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;

			m_M[ 0 ] = CosY;
			m_M[ 8 ] = -SinY;
			m_M[ 2 ] = SinY;
			m_M[ 10 ] = CosY;

			return *this;
		}

		Matrix4x4 &Matrix4x4::RotateZ( const ZED_FLOAT32 p_Z )
		{
			ZED_FLOAT32 SinZ, CosZ;
			Arithmetic::SinCos( p_Z, SinZ, CosZ );

			m_M[ 10 ] = m_M[ 15 ] = 1.0f;
			m_M[ 2 ] = m_M[ 3 ] = m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] =
				m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 1.0f;

			m_M[ 0 ] = CosZ;
			m_M[ 4 ] = SinZ;
			m_M[ 1 ] = -SinZ;
			m_M[ 5 ] = CosZ;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const ZED_FLOAT32 p_Value )
		{
			m_M[ 0 ] = m_M[ 5 ] = m_M[ 10 ] = p_Value;
			m_M[ 15 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::Scale( const Vector3 &p_Scale )
		{
			m_M[ 0 ] = p_Scale[ 0 ];
			m_M[ 5 ] = p_Scale[ 1 ];
			m_M[ 10 ] = p_Scale[ 2 ];
			m_M[ 15 ] = 1.0f;

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] = m_M[ 12 ] = m_M[ 13 ] =
				m_M[ 14 ] = 0.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleX( const ZED_FLOAT32 p_X )
		{
			m_M[ 0 ] = p_X;
			m_M[ 15 ] = 1.0f;

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = m_M[ 10 ] = m_M[ 11 ] =
				m_M[ 12 ] = m_M[ 13 ] = m_M[ 14] = 0.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleY( const ZED_FLOAT32 p_Y )
		{
			m_M[ 5 ] = p_Y;
			m_M[ 15 ] = 1.0f;

            m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 6 ] =
				m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = m_M[ 10 ] = m_M[ 11 ] =
				m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;

			return *this;
		}

		Matrix4x4 &Matrix4x4::ScaleZ( const ZED_FLOAT32 p_Z )
		{
			m_M[ 10 ] = p_Z;
			m_M[ 15 ] = 1.0f;

            m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 5 ] =
				m_M[ 6 ] = m_M[ 7 ] = m_M[ 8 ] = m_M[ 9 ] = m_M[ 11 ] =
				m_M[ 12 ] = m_M[ 13 ] = m_M[ 14 ] = 0.0f;

			return *this;
		}

		ZED_BOOL Matrix4x4::IsZero( ) const
		{
			// If one of the elements in the matrix is zero, then it is
			// immediately returned as false
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				if( Arithmetic::IsZero( m_M[ i ] ) == ZED_FALSE )
				{
					return ZED_FALSE;
				}
			}
			return ZED_TRUE;
		}

		ZED_BOOL Matrix4x4::IsIdentity( ) const
		{
			if( Arithmetic::Equal( m_M[ 0 ], 1.0f ) &&
				Arithmetic::Equal( m_M[ 1 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 2 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 3 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 4 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 5 ], 1.0f ) &&
				Arithmetic::Equal( m_M[ 6 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 7 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 8 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 9 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 10 ], 1.0f ) &&
				Arithmetic::Equal( m_M[ 11 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 12 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 13 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 14 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 15 ], 1.0f ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
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

		Vector4 Matrix4x4::GetRow( const ZED_UINT32 p_RowNumber ) const
		{
			return Vector4( m_M[ p_RowNumber*4 ], m_M[ ( p_RowNumber*4 ) + 1 ],
				m_M[ ( p_RowNumber*4 ) + 2 ], m_M[ ( p_RowNumber*4 ) + 3 ] );
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

		Vector4 Matrix4x4::GetColumn( const ZED_UINT32 p_ColumnNumber ) const
		{
			return Vector4( m_M[ p_ColumnNumber ], m_M[ p_ColumnNumber+1 ],
				m_M[ p_ColumnNumber+2 ], m_M[ p_ColumnNumber+3 ] );
		}

		void Matrix4x4::Clean( )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				if( Arithmetic::IsZero( m_M[ i ] ) )
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

			Temp = m_M[ 2 ];
			m_M[ 2 ] = m_M[ 8 ];
			m_M[ 8 ] = Temp;

			Temp = m_M[ 3 ];
			m_M[ 3 ] = m_M[ 12 ];
			m_M[ 12 ] = Temp;

			Temp = m_M[ 6 ];
			m_M[ 6 ] = m_M[ 9 ];
			m_M[ 9 ] = Temp;

			Temp = m_M[ 7 ];
			m_M[ 7 ] = m_M[ 13 ];
			m_M[ 13 ] = Temp;

			Temp = m_M[ 11 ];
			m_M[ 11 ] = m_M[ 14 ];
			m_M[ 14 ] = Temp;

			return *this;
		}

		Matrix4x4 Matrix4x4::TransposeOf( const Matrix4x4 &p_Transpose )
		{
			Matrix4x4 Result;

			Result.m_M[ 0 ] = p_Transpose.m_M[ 0 ];
			Result.m_M[ 1 ] = p_Transpose.m_M[ 4 ];
			Result.m_M[ 2 ] = p_Transpose.m_M[ 8 ];
			Result.m_M[ 3 ] = p_Transpose.m_M[ 12 ];

			Result.m_M[ 4 ] = p_Transpose.m_M[ 1 ];
			Result.m_M[ 5 ] = p_Transpose.m_M[ 5 ];
			Result.m_M[ 6 ] = p_Transpose.m_M[ 9 ];
			Result.m_M[ 7 ] = p_Transpose.m_M[ 13 ];
			
			Result.m_M[ 8 ] = p_Transpose.m_M[ 2 ];
			Result.m_M[ 9 ] = p_Transpose.m_M[ 6 ];
			Result.m_M[ 10 ] = p_Transpose.m_M[ 10 ];
			Result.m_M[ 11 ] = p_Transpose.m_M[ 14 ];

			Result.m_M[ 12 ] = p_Transpose.m_M[ 3 ];
			Result.m_M[ 13 ] = p_Transpose.m_M[ 7 ];
			Result.m_M[ 14 ] = p_Transpose.m_M[ 11 ];
			Result.m_M[ 15 ] = p_Transpose.m_M[ 15 ];

			return Result;
		}

		Matrix4x4 &Matrix4x4::AffineInverse( )
		{
			Matrix4x4 Return;

			// Compute upper 3^2 matrix determinate
			ZED_FLOAT32 CoFactor0 = m_M[ 5 ]*m_M[ 10 ] - m_M[ 6 ]*m_M[ 9 ];
			ZED_FLOAT32 CoFactor4 = m_M[ 2 ]*m_M[ 9 ] - m_M[ 1 ]*m_M[ 10 ];
			ZED_FLOAT32 CoFactor8 = m_M[ 1 ]*m_M[ 6 ] - m_M[ 2 ]*m_M[ 5 ];
			ZED_FLOAT32 Det = m_M[ 0 ]*CoFactor0 + m_M[ 4 ]*CoFactor4 +
				m_M[ 8 ]*CoFactor8;

			if( Arithmetic::IsZero( Det ) )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "Matrix4x4::AffineInverse( ) SINGULAR MATRIX!\n" );
				return Return;
			}

			ZED_FLOAT32 InvDet = 1.0f/Det;

			Return.m_M[ 0 ] = InvDet*CoFactor0;
			Return.m_M[ 1 ] = InvDet*CoFactor4;
			Return.m_M[ 2 ] = InvDet*CoFactor8;

			Return.m_M[ 4 ] = InvDet*( m_M[ 6 ]*m_M[ 8 ] - m_M[ 4 ]*m_M[ 10 ] );
			Return.m_M[ 5 ] = InvDet*( m_M[ 0 ]*m_M[ 10 ] - m_M[ 2 ]*m_M[ 8 ] );
			Return.m_M[ 6 ] = InvDet*( m_M[ 2 ]*m_M[ 4 ] - m_M[ 0 ]*m_M[ 6 ] );

			Return.m_M[ 8 ] = InvDet*( m_M[ 4 ]*m_M[ 9 ] - m_M[ 5 ]*m_M[ 8 ] );
			Return.m_M[ 9 ] = InvDet*( m_M[ 1 ]*m_M[ 8 ] - m_M[ 0 ]*m_M[ 9 ] );
			Return.m_M[ 10 ] = InvDet*( m_M[ 0 ]*m_M[ 5 ] - m_M[ 1 ]*m_M[ 4 ] );

			// Multiply -translation by inverted 3x3 to get its inverse
			Return.m_M[ 12 ] = -Return.m_M[ 0 ]*Return.m_M[ 12 ] -
				Return.m_M[ 4 ]*Return.m_M[ 13 ] -
				Return.m_M[ 8 ]*Return.m_M[ 14 ];
			Return.m_M[ 13 ] = -Return.m_M[ 1 ]*Return.m_M[ 12 ] -
				Return.m_M[ 5 ]*Return.m_M[ 13 ] -
				Return.m_M[ 9 ]*Return.m_M[ 14 ];
			Return.m_M[ 14 ] = -Return.m_M[ 2 ]*Return.m_M[ 12 ] -
				Return.m_M[ 6 ]*Return.m_M[ 13 ] -
				Return.m_M[ 10 ]*Return.m_M[ 14 ];

			return Return;
		}

		Matrix4x4 &Matrix4x4::Translate( const Vector3 &p_Translate )
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
			Vector3 Ret;

            Ret[ 0 ] = m_M[ 0 ]*p_Point[ 0 ] + m_M[ 4 ]*p_Point[ 1 ] +
				m_M[ 8 ]*p_Point[ 2 ] + m_M[ 12 ];
			Ret[ 1 ] = m_M[ 1 ]*p_Point[ 0 ] + m_M[ 5 ]*p_Point[ 1 ] +
				m_M[ 9 ]*p_Point[ 2 ] + m_M[ 13 ];
			Ret[ 2 ] = m_M[ 2 ]*p_Point[ 0 ] + m_M[ 6 ]*p_Point[ 1 ] +
				m_M[ 10 ]*p_Point[ 2 ] + m_M[ 14 ];

			return Ret;
		}

		ZED_FLOAT32 Matrix4x4::Trace( ) const
		{
			return m_M[ 0 ] + m_M[ 5 ] + m_M[ 10 ] + m_M[ 15 ];
		}

		System::Writer &operator<<( System::Writer p_Out,
			const Matrix4x4 p_Source )
		{
			for( ZED_UINT32 i = 0; i < 4; i++ )
			{
				p_Out << "| ";
				for( ZED_UINT32 j = 0; j < 4; j++ )
				{
					p_Out << p_Source[ j*4 + i ] << " ";
				}
				p_Out << "|" << std::endl;
			}

			return p_Out;
		}

		ZED_BOOL Matrix4x4::operator==( const Matrix4x4 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				if( Arithmetic::Equal( m_M[ i ], p_Other.m_M[ i ] ) ==
					ZED_FALSE )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix4x4::operator!=( const Matrix4x4 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				if( Arithmetic::Equal( m_M[ i ], p_Other.m_M[ i ] ) ==
					ZED_TRUE )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		Matrix4x4 Matrix4x4::operator+( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Ret;

			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				Ret.m_M[ i ] = m_M[ i ]+p_Other.m_M[ i ];
			}

			return Ret;
		}

		Matrix4x4 Matrix4x4::operator-( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Ret;

			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				Ret.m_M[ i ] = m_M[ i ]-p_Other.m_M[ i ];
			}

			return Ret;
		}

		Matrix4x4 Matrix4x4::operator-( ) const
		{
			Matrix4x4 Ret;

			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				Ret.m_M[ i ] = -m_M[ i ];
			}

			return Ret;
		}

		Matrix4x4 Matrix4x4::operator*( const Matrix4x4 &p_Other ) const
		{
			Matrix4x4 Ret;

			Ret.m_M[ 0 ] =	m_M[ 0 ]*p_Other.m_M[ 0 ] +
							m_M[ 4 ]*p_Other.m_M[ 1 ] +
							m_M[ 8 ]*p_Other.m_M[ 2 ] +
							m_M[ 12 ]*p_Other.m_M[ 3 ];
			Ret.m_M[ 1 ] =	m_M[ 1 ]*p_Other.m_M[ 0 ] +
							m_M[ 5 ]*p_Other.m_M[ 1 ] +
							m_M[ 9 ]*p_Other.m_M[ 2 ] +
							m_M[ 13 ]*p_Other.m_M[ 3 ];
			Ret.m_M[ 2 ] =	m_M[ 2 ]*p_Other.m_M[ 0 ] +
							m_M[ 6 ]*p_Other.m_M[ 1 ] +
							m_M[ 10 ]*p_Other.m_M[ 2 ] +
							m_M[ 14 ]*p_Other.m_M[ 3 ];
			Ret.m_M[ 3 ] =	m_M[ 3 ]*p_Other.m_M[ 0 ] +
							m_M[ 7 ]*p_Other.m_M[ 1 ] +
							m_M[ 11 ]*p_Other.m_M[ 2 ] +
							m_M[ 15 ]*p_Other.m_M[ 3 ];

			Ret.m_M[ 4 ] =	m_M[ 0 ]*p_Other.m_M[ 4 ] +
							m_M[ 4 ]*p_Other.m_M[ 5 ] +
							m_M[ 8 ]*p_Other.m_M[ 6 ] +
							m_M[ 12 ]*p_Other.m_M[ 7 ];
			Ret.m_M[ 5 ] =	m_M[ 1 ]*p_Other.m_M[ 4 ] +
							m_M[ 5 ]*p_Other.m_M[ 5 ] +
							m_M[ 9 ]*p_Other.m_M[ 6 ] +
							m_M[ 13 ]*p_Other.m_M[ 7 ];
			Ret.m_M[ 6 ] =	m_M[ 2 ]*p_Other.m_M[ 4 ] +
							m_M[ 6 ]*p_Other.m_M[ 5 ] +
							m_M[ 10 ]*p_Other.m_M[ 6 ] +
							m_M[ 14 ]*p_Other.m_M[ 7 ];
			Ret.m_M[ 7 ] =	m_M[ 3 ]*p_Other.m_M[ 4 ] +
							m_M[ 7 ]*p_Other.m_M[ 5 ] +
							m_M[ 11 ]*p_Other.m_M[ 6 ] +
							m_M[ 15 ]*p_Other.m_M[ 7 ];

			Ret.m_M[ 8 ] =	m_M[ 0 ]*p_Other.m_M[ 8 ] +
							m_M[ 4 ]*p_Other.m_M[ 9 ] +
							m_M[ 8 ]*p_Other.m_M[ 10 ] +
							m_M[ 12 ]*p_Other.m_M[ 11 ];
			Ret.m_M[ 9 ] =	m_M[ 1 ]*p_Other.m_M[ 8 ] +
							m_M[ 5 ]*p_Other.m_M[ 9 ] +
							m_M[ 9 ]*p_Other.m_M[ 10 ] +
							m_M[ 13 ]*p_Other.m_M[ 11 ];
			Ret.m_M[ 10 ] =	m_M[ 2 ]*p_Other.m_M[ 8 ] +
							m_M[ 6 ]*p_Other.m_M[ 9 ] +
							m_M[ 10 ]*p_Other.m_M[ 10 ] +
							m_M[ 14 ]*p_Other.m_M[ 11 ];
			Ret.m_M[ 11 ] = m_M[ 3 ]*p_Other.m_M[ 8 ] +
							m_M[ 7 ]*p_Other.m_M[ 9 ] +
							m_M[ 11 ]*p_Other.m_M[ 10 ] +
							m_M[ 15 ]*p_Other.m_M[ 11 ];

			Ret.m_M[ 12 ] = m_M[ 0 ]*p_Other.m_M[ 12 ] +
							m_M[ 4 ]*p_Other.m_M[ 13 ] +
							m_M[ 8 ]*p_Other.m_M[ 14 ] +
							m_M[ 12 ]*p_Other.m_M[ 15 ];
			Ret.m_M[ 13 ] = m_M[ 1 ]*p_Other.m_M[ 12 ] +
							m_M[ 5 ]*p_Other.m_M[ 13 ] +
							m_M[ 9 ]*p_Other.m_M[ 14 ] +
							m_M[ 13 ]*p_Other.m_M[ 15 ];
			Ret.m_M[ 14 ] = m_M[ 2 ]*p_Other.m_M[ 12 ] +
							m_M[ 6 ]*p_Other.m_M[ 13 ] +
							m_M[ 10 ]*p_Other.m_M[ 14 ] +
							m_M[ 14 ]*p_Other.m_M[ 15 ];
			Ret.m_M[ 15 ] = m_M[ 3 ]*p_Other.m_M[ 12 ] +
							m_M[ 7 ]*p_Other.m_M[ 13 ] +
							m_M[ 11 ]*p_Other.m_M[ 14 ] +
							m_M[ 15 ]*p_Other.m_M[ 15 ];

			return Ret;
		}

		Matrix4x4 Matrix4x4::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			Matrix4x4 Ret;

			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				Ret.m_M[ i ] = m_M[ i ]*p_Scalar;
			}

			return Ret;
		}

		Matrix4x4 operator*( const ZED_UINT32 p_Scalar,
			const Matrix4x4 &p_Matrix )
		{
			Matrix4x4 Ret;

			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				Ret[ i ] = p_Matrix[ i ]*p_Scalar;
			}

			return Ret;
		}

		Vector4 Matrix4x4::operator*( const Vector4 &p_Vec ) const
		{
			Vector4 Ret;

			Ret[ 0 ] = m_M[ 0 ]*p_Vec[ 0 ] + m_M[ 4 ]*p_Vec[ 1 ] +
				m_M[ 8 ]*p_Vec[ 2 ] + m_M[ 12 ]*p_Vec[ 3 ];
			Ret[ 1 ] = m_M[ 1 ]*p_Vec[ 0 ] + m_M[ 5 ]*p_Vec[ 1 ] +
				m_M[ 9 ]*p_Vec[ 2 ] + m_M[ 13 ]*p_Vec[ 3 ];
			Ret[ 2 ] = m_M[ 2 ]*p_Vec[ 0 ] + m_M[ 6 ]*p_Vec[ 1 ] +
				m_M[ 10 ]*p_Vec[ 2 ] + m_M[ 14 ]*p_Vec[ 3 ];
			Ret[ 3 ] = m_M[ 3 ]*p_Vec[ 0 ] + m_M[ 7 ]*p_Vec[ 1 ] +
				m_M[ 11 ]*p_Vec[ 2 ] + m_M[ 15 ]*p_Vec[ 3 ];

			return Ret;
		}

		Vector4 operator*( const Vector4 &p_Vec, const Matrix4x4 &p_Matrix )
		{
			Vector4 Ret;

			Ret[ 0 ] = p_Vec[ 0 ]*p_Matrix[ 0 ] + p_Vec[ 1 ]*p_Matrix[ 1 ] +
				p_Vec[ 2 ]*p_Matrix[ 2 ] + p_Vec[ 3 ]*p_Matrix[ 3 ];
			Ret[ 1 ] = p_Vec[ 0 ]*p_Matrix[ 4 ] + p_Vec[ 1 ]*p_Matrix[ 5 ] +
				p_Vec[ 2 ]*p_Matrix[ 6 ] + p_Vec[ 3 ]*p_Matrix[ 7 ];
			Ret[ 2 ] = p_Vec[ 0 ]*p_Matrix[ 8 ] + p_Vec[ 1 ]*p_Matrix[ 9 ] +
				p_Vec[ 2 ]*p_Matrix[ 10 ] + p_Vec[ 3 ]*p_Matrix[ 11 ];
			Ret[ 3 ] = p_Vec[ 0 ]*p_Matrix[ 12 ] + p_Vec[ 1 ]*p_Matrix[ 13 ] +
				p_Vec[ 2 ]*p_Matrix[ 14 ] + p_Vec[ 3 ]*p_Matrix[ 15 ];

			return Ret;
		}

		Matrix4x4 &Matrix4x4::operator=( const Matrix4x4 &p_Copy )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				m_M[ i ] = p_Copy[ i ];
			}

			return *this;
		}

		Matrix4x4 &Matrix4x4::operator+=( const Matrix4x4 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				m_M[ i ] += p_Other.m_M[ i ];
			}

			return *this;
		}

		Matrix4x4 &Matrix4x4::operator-=( const Matrix4x4 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				m_M[ i ] -= p_Other.m_M[ i ];
			}

			return *this;
		}

		Matrix4x4 &Matrix4x4::operator*=( const Matrix4x4 &p_Other )
		{
			m_M[ 0 ] = m_M[ 0 ]*p_Other.m_M[ 0 ] + m_M[ 4 ]*p_Other.m_M[ 1 ] +
				m_M[ 8 ]*p_Other.m_M[ 2 ] + m_M[ 12 ]*p_Other.m_M[ 3 ];
			m_M[ 1 ] = m_M[ 1 ]*p_Other.m_M[ 0 ] + m_M[ 5 ]*p_Other.m_M[ 1 ] +
				m_M[ 9 ]*p_Other.m_M[ 2 ] + m_M[ 13 ]*p_Other.m_M[ 3 ];
			m_M[ 2 ] = m_M[ 2 ]*p_Other.m_M[ 0 ] + m_M[ 6 ]*p_Other.m_M[ 1 ] +
				m_M[ 10 ]*p_Other.m_M[ 2 ] + m_M[ 14 ]*p_Other.m_M[ 3 ];
			m_M[ 3 ] = m_M[ 3 ]*p_Other.m_M[ 0 ] + m_M[ 7 ]*p_Other.m_M[ 1 ] +
				m_M[ 11 ]*p_Other.m_M[ 2 ] + m_M[ 15 ]*p_Other.m_M[ 3 ];

			m_M[ 4 ] = m_M[ 0 ]*p_Other.m_M[ 4 ] + m_M[ 4 ]*p_Other.m_M[ 5 ] +
				m_M[ 8 ]*p_Other.m_M[ 6 ] + m_M[ 12 ]*p_Other.m_M[ 7 ];
			m_M[ 5 ] = m_M[ 1 ]*p_Other.m_M[ 4 ] + m_M[ 5 ]*p_Other.m_M[ 5 ] +
				m_M[ 9 ]*p_Other.m_M[ 6 ] + m_M[ 13 ]*p_Other.m_M[ 7 ];
			m_M[ 6 ] = m_M[ 2 ]*p_Other.m_M[ 4 ] + m_M[ 6 ]*p_Other.m_M[ 5 ] +
				m_M[ 10 ]*p_Other.m_M[ 6 ] + m_M[ 14 ]*p_Other.m_M[ 7 ];
			m_M[ 7 ] = m_M[ 3 ]*p_Other.m_M[ 4 ] + m_M[ 7 ]*p_Other.m_M[ 5 ] +
				m_M[ 11 ]*p_Other.m_M[ 6 ] + m_M[ 15 ]*p_Other.m_M[ 7 ];

			m_M[ 8 ] = m_M[ 0 ]*p_Other.m_M[ 8 ] + m_M[ 4 ]*p_Other.m_M[ 9 ] +
				m_M[ 8 ]*p_Other.m_M[ 10 ] + m_M[ 12 ]*p_Other.m_M[ 11 ];
			m_M[ 9 ] = m_M[ 1 ]*p_Other.m_M[ 8 ] + m_M[ 5 ]*p_Other.m_M[ 9 ] +
				m_M[ 9 ]*p_Other.m_M[ 10 ] + m_M[ 13 ]*p_Other.m_M[ 11 ];
			m_M[ 10 ] = m_M[ 2 ]*p_Other.m_M[ 8 ] + m_M[ 6 ]*p_Other.m_M[ 9 ] +
				m_M[ 10 ]*p_Other.m_M[ 10 ] + m_M[ 14 ]*p_Other.m_M[ 11 ];
			m_M[ 11 ] = m_M[ 3 ]*p_Other.m_M[ 8 ] + m_M[ 7 ]*p_Other.m_M[ 9 ] +
				m_M[ 11 ]*p_Other.m_M[ 10 ] + m_M[ 15 ]*p_Other.m_M[ 11 ];

			m_M[ 12 ] = m_M[ 0 ]*p_Other.m_M[ 12 ] +
						m_M[ 4 ]*p_Other.m_M[ 13 ] +
						m_M[ 8 ]*p_Other.m_M[ 14 ] +
						m_M[ 12 ]*p_Other.m_M[ 15 ];
			m_M[ 13 ] = m_M[ 1 ]*p_Other.m_M[ 12 ] +
						m_M[ 5 ]*p_Other.m_M[ 13 ] +
						m_M[ 9 ]*p_Other.m_M[ 14 ] +
						m_M[ 13 ]*p_Other.m_M[ 15 ];
			m_M[ 14 ] = m_M[ 2 ]*p_Other.m_M[ 12 ] +
						m_M[ 6 ]*p_Other.m_M[ 13 ] +
						m_M[ 10 ]*p_Other.m_M[ 14 ] +
						m_M[ 14 ]*p_Other.m_M[ 15 ];
			m_M[ 15 ] = m_M[ 3 ]*p_Other.m_M[ 12 ] +
						m_M[ 7 ]*p_Other.m_M[ 13 ] +
						m_M[ 11 ]*p_Other.m_M[ 14 ] +
						m_M[ 15 ]*p_Other.m_M[ 15 ];

			return *this;
		}

		Matrix4x4 &Matrix4x4::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			for( ZED_UINT32 i = 0; i < 16; i++ )
			{
				m_M[ i ] *= p_Scalar;
			}

			return *this;
		}

		ZED_FLOAT32 &Matrix4x4::operator( )( const ZED_UINT32 p_Row,
			const ZED_UINT32 p_Column )
		{
			return m_M[ p_Row+( p_Column*4 ) ];
		}

		ZED_FLOAT32 Matrix4x4::operator( )( const ZED_UINT32 p_Row,
			const ZED_UINT32 p_Column ) const
		{
			return m_M[ p_Row+( p_Column*4 ) ];
		}
	}
}
