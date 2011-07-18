#include <Matrix3x3.hpp>
#include <Quaternion.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		/*Matrix3x3::Matrix3x3( const Matrix3x3 &p_Copy )
		{
			// Probably not too efficient
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_M[ i ] = p_Copy.m_M[ i ];
			}
		}*/

		Matrix3x3::Matrix3x3( const Quaternion &p_Quat )
		{
			// Just assemble from the rotation function
			// [cast to void as nothing is being returned]
			( void )Rotate( p_Quat );
		}

		void Matrix3x3::Identity( )
		{
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] =
				m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] = 0.0f;
			m_M[ 0 ] = m_M[ 4 ] = m_M[ 8 ] = 1.0f;
		}

		Matrix3x3 &Matrix3x3::Rotate( const Quaternion &p_Quat )
		{
			// Manke sure the quaternion is a unit [1.0f] quaternion
			// before moving on
			zedAssert( p_Quat.IsUnit( ) );

			ZED_FLOAT32 XS, YS, ZS, WX, WY, WZ, XX, XY, XZ, YY, YZ, ZZ;

			XS = p_Quat[ 0 ]+p_Quat[ 0 ];
			YS = p_Quat[ 1 ]+p_Quat[ 1 ];
			ZS = p_Quat[ 2 ]+p_Quat[ 2 ];
			WX = p_Quat[ 3 ]*XS;
			WY = p_Quat[ 3 ]*YS;
			WZ = p_Quat[ 3 ]*ZS;
			XX = p_Quat[ 0 ]*XS;
			XY = p_Quat[ 0 ]*YS;
			XZ = p_Quat[ 0 ]*ZS;
			YY = p_Quat[ 1 ]*YS;
			YZ = p_Quat[ 1 ]*ZS;
			ZZ = p_Quat[ 2 ]*ZS;

			m_M[ 0 ] = 1.0f - ( YY * ZZ );
			m_M[ 3 ] = XY - WZ;
			m_M[ 6 ] = XZ + WY;

			m_M[ 1 ] = XY + WZ;
			m_M[ 4 ] = 1.0f - ( XX + ZZ );
			m_M[ 7 ] = YZ - WX;

			m_M[ 2 ] = XZ - WY;
			m_M[ 5 ] = YZ + WX;
			m_M[ 8 ] = 1.0f - ( XX + YY );

			return *this;
		}

		Matrix3x3 &Matrix3x3::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			ZED_FLOAT32 Sin, Cos, Tan;
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
			ZED_FLOAT32 TXZ = TX*Axis[ 3 ];

			// Finally, set up the matrix
			m_M[ 0 ] = TX*Axis[ 0 ] + Cos;
			m_M[ 3 ] = TXY - SZ;
			m_M[ 6 ] = TXZ + SY;

			m_M[ 1 ] = TXY + SZ;
			m_M[ 4 ] = TY*Axis[ 1 ] + Cos;
			m_M[ 7 ] = TYZ - SX;

			m_M[ 2 ] = TXZ - SY;
			m_M[ 5 ] = TYZ + SX;
			m_M[ 8 ] = TZ*Axis[ 2 ] + Cos;

			return *this;
		}
		
		Matrix3x3 &Matrix3x3::Rotate( const ZED_FLOAT32 p_Z,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_X )
		{
			float CosX, SinX, CosY, SinY, CosZ, SinZ;
			
			Arithmetic::SinCos( p_X, CosX, SinX );
			Arithmetic::SinCos( p_Y, CosY, SinY );
			Arithmetic::SinCos( p_Z, CosZ, SinZ );

			m_M[ 0 ] = ( CosY * CosZ );
			m_M[ 3 ] = -( CosY * SinZ );
			m_M[ 6 ] = SinY;

			m_M[ 1 ] = ( SinX * SinY * SinZ ) + ( CosX * SinZ );
			m_M[ 4 ] = -( SinX * SinY * SinZ ) + ( CosX * CosZ );
			m_M[ 7 ] = -( SinX * CosY );

			m_M[ 2 ] = -( CosX * SinY * CosZ ) + ( SinX * SinZ );
			m_M[ 5 ] = ( CosX * SinY * SinZ ) + ( SinX * CosZ );
			m_M[ 8 ] = ( CosX * CosY );

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateX( const ZED_FLOAT32 p_X )
		{
			ZED_FLOAT32 Sin, Cos;
			Arithmetic::SinCos( p_X, Sin, Cos );

			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 6 ] = 0.0f;
			
			m_M[ 4 ] = Cos;
			m_M[ 5 ] = Sin;
			m_M[ 7 ] = -Sin;
			m_M[ 8 ] = Cos;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateY( const ZED_FLOAT32 p_Y )
		{
			ZED_FLOAT32 Sin, Cos;
			Arithmetic::SinCos( p_Y, Sin, Cos );

			m_M[ 4 ] = 1.0f;
			m_M[ 1 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 7 ] = 0.0f;
			m_M[ 0 ] = Cos;
			m_M[ 2 ] = -Sin;
			m_M[ 6 ] = Sin;
			m_M[ 8 ] = Cos;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateZ( const ZED_FLOAT32 p_Z )
		{
			ZED_FLOAT32 Sin, Cos;
			Arithmetic::SinCos( p_Z, Sin, Cos );

			m_M[ 2 ] = m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] = 0.0f;
			m_M[ 8 ] = 1.0f;
			m_M[ 0 ] = Cos;
			m_M[ 1 ] = Sin;
			m_M[ 3 ] = -Sin;
			m_M[ 4 ] = Cos;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Scale( const ZED_FLOAT32 p_Value )
		{
			m_M[ 0 ] = m_M[ 4 ] = m_M[ 8 ] = p_Value;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] =
				0.0f;
			return *this;
		}

		Matrix3x3 &Matrix3x3::Scale( const Vector3 &p_Scale )
		{
			m_M[ 0 ] = p_Scale[ 0 ];
			m_M[ 4 ] = p_Scale[ 1 ] ;
			m_M[ 8 ] = p_Scale[ 2 ];

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] =
				0.0f;
			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleX( const ZED_FLOAT32 p_X )
		{
			m_M[ 0 ] = p_X;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = m_M[ 8 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleY( const ZED_FLOAT32 p_Y )
		{
			m_M[ 4 ] = p_Y;
			m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = m_M[ 8 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleZ( const ZED_FLOAT32 p_Z )
		{
			m_M[ 8 ] = p_Z;
			m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] = m_M[ 5 ] =
				m_M[ 6 ] = m_M[ 7 ] = 0.0f;

			return *this;
		}

		ZED_BOOL Matrix3x3::IsZero( ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( Arithmetic::IsZero( m_M[ i ] ) == ZED_FALSE )
				{
					return ZED_FALSE;
				}
			}
			return ZED_TRUE;
		}

		ZED_BOOL Matrix3x3::IsIdentity( ) const
		{
			if( Arithmetic::Equal( m_M[ 0 ], 1.0f ) &&
				Arithmetic::Equal( m_M[ 1 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 2 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 3 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 4 ], 1.0f ) &&
				Arithmetic::Equal( m_M[ 5 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 6 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 7 ], 0.0f ) &&
				Arithmetic::Equal( m_M[ 8 ], 1.0f ) )
			{
				return ZED_TRUE;
			}
			return ZED_FALSE;
		}

		void Matrix3x3::SetRows( const Vector3 &p_Row1, const Vector3 &p_Row2,
				const Vector3 &p_Row3 )
		{
			m_M[ 0 ] = p_Row1[ 0 ];
			m_M[ 1 ] = p_Row1[ 1 ];
			m_M[ 2 ] = p_Row1[ 2 ];

			m_M[ 3 ] = p_Row2[ 0 ];
			m_M[ 4 ] = p_Row2[ 1 ];
			m_M[ 5 ] = p_Row2[ 2 ];

			m_M[ 6 ] = p_Row3[ 0 ];
			m_M[ 7 ] = p_Row3[ 1 ];
			m_M[ 8 ] = p_Row3[ 2 ];
		}

		void Matrix3x3::GetRows( Vector3 &p_Row1, Vector3 &p_Row2,
			Vector3 &p_Row3 ) const
		{
			p_Row1[ 0 ] = m_M[ 0 ];
			p_Row1[ 1 ] = m_M[ 1 ];
			p_Row1[ 2 ] = m_M[ 2 ];

			p_Row2[ 0 ] = m_M[ 3 ];
			p_Row2[ 1 ] = m_M[ 4 ];
			p_Row2[ 2 ] = m_M[ 5 ];

			p_Row3[ 0 ] = m_M[ 6 ];
			p_Row3[ 1 ] = m_M[ 7 ];
			p_Row3[ 2 ] = m_M[ 8 ];
		}

		Vector3 Matrix3x3::GetRow( const ZED_UINT32 p_RowNumber ) const
		{
			return Vector3( m_M[ p_RowNumber*3 ], m_M[ ( p_RowNumber*3 )+1 ],
				m_M[ ( p_RowNumber*3 )+2 ] );
		}

		void Matrix3x3::SetColumns( const Vector3 &p_Column1,
			const Vector3 &p_Column2, const Vector3 &p_Column3 )
		{
			m_M[ 0 ] = p_Column1[ 0 ];
			m_M[ 3 ] = p_Column1[ 1 ];
			m_M[ 6 ] = p_Column1[ 2 ];

			m_M[ 1 ] = p_Column2[ 0 ];
			m_M[ 4 ] = p_Column2[ 1 ];
			m_M[ 7 ] = p_Column2[ 2 ];

			m_M[ 2 ] = p_Column3[ 0 ];
			m_M[ 5 ] = p_Column3[ 1 ];
			m_M[ 8 ] = p_Column3[ 2 ];
		}

		void Matrix3x3::GetColumns( Vector3 &p_Column1, 
			Vector3 &p_Column2, Vector3 &p_Column3 ) const
		{
			p_Column1[ 0 ] = m_M[ 0 ];
			p_Column1[ 1 ] = m_M[ 3 ];
			p_Column1[ 2 ] = m_M[ 6 ];

			p_Column2[ 0 ] = m_M[ 1 ];
			p_Column2[ 1 ] = m_M[ 4 ];
			p_Column2[ 2 ] = m_M[ 7 ];

			p_Column3[ 0 ] = m_M[ 2 ];
			p_Column3[ 1 ] = m_M[ 5 ];
			p_Column3[ 2 ] = m_M[ 8 ];
		}

		Vector3 Matrix3x3::GetColumn( const ZED_UINT32 p_ColumnNumber ) const
		{
			return Vector3( m_M[ p_ColumnNumber ], m_M[ p_ColumnNumber+3 ],
				m_M[ p_ColumnNumber+6 ] );
		}

		void Matrix3x3::Clean( )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( Arithmetic::IsZero( m_M[ i ] ) )
				{
					m_M[ i ] = 0.0f;
				}
			}
		}

		Matrix3x3 &Matrix3x3::Transpose( )
		{
			ZED_FLOAT32 Temp = m_M[ 1 ];
			m_M[ 1 ] = m_M[ 3 ];
			m_M[ 3 ] = Temp;

			Temp = m_M[ 2 ];
			m_M[ 2 ] = m_M[ 6 ];
			m_M[ 6 ] = Temp;

			Temp = m_M[ 5 ];
			m_M[ 5 ] = m_M[ 7 ];
			m_M[ 7 ] = Temp;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Inverse( )
		{
			// Compute the determinate, first
			ZED_FLOAT32 CoFactor0, CoFactor3, CoFactor6;
			CoFactor0 = m_M[ 4 ]*m_M[ 8 ] - m_M[ 5 ]*m_M[ 7 ];
			CoFactor3 = m_M[ 2 ]*m_M[ 7 ] - m_M[ 1 ]*m_M[ 8 ];
			CoFactor6 = m_M[ 1 ]*m_M[ 5 ] - m_M[ 2 ]*m_M[ 4 ];
			
			ZED_FLOAT32 Det = m_M[ 0 ]*CoFactor0 + m_M[ 3 ]*CoFactor3 +
				m_M[ 6 ]*CoFactor6;

			if( Arithmetic::IsZero( Det ) )
			{
				// Effectively, always stop execution when the debugger is
				// attached
				zedAssert( ZED_FALSE );
				zedTrace( "Matrix3x3::Inverse( ) - SINGULAR MATRIX\n" );
				return *this;
			}

			// Create adjoint matrix and multiply by 1/Det to get inverse
			ZED_FLOAT32 InvDet = 1.0f/Det;

			m_M[ 0 ] = InvDet*CoFactor0;
			m_M[ 1 ] = InvDet*CoFactor3;
			m_M[ 2 ] = InvDet*CoFactor6;

			m_M[ 3 ] = InvDet*( m_M[ 5 ]*m_M[ 6 ] - m_M[ 3 ]*m_M[ 8 ] );
			m_M[ 4 ] = InvDet*( m_M[ 0 ]*m_M[ 8 ] - m_M[ 2 ]*m_M[ 6 ] );
			m_M[ 5 ] = InvDet*( m_M[ 2 ]*m_M[ 3 ] - m_M[ 0 ]*m_M[ 5 ] );

			m_M[ 6 ] = InvDet*( m_M[ 3 ]*m_M[ 7 ] - m_M[ 4 ]*m_M[ 6 ] );
			m_M[ 7 ] = InvDet*( m_M[ 1 ]*m_M[ 6 ] - m_M[ 0 ]*m_M[ 7 ] );
			m_M[ 8 ] = InvDet*( m_M[ 0 ]*m_M[ 4 ] - m_M[ 1 ]*m_M[ 3 ] );

			return *this;
		}

		Matrix3x3 Matrix3x3::Adjoint( ) const
		{
			Matrix3x3 Adjoint;

			Adjoint.m_M[ 0 ] = m_M[ 4 ]*m_M[ 8 ] - m_M[ 5 ]*m_M[ 7 ];
			Adjoint.m_M[ 1 ] = m_M[ 2 ]*m_M[ 7 ] - m_M[ 1 ]*m_M[ 8 ];
			Adjoint.m_M[ 2 ] = m_M[ 1 ]*m_M[ 5 ] - m_M[ 2 ]*m_M[ 4 ];

			Adjoint.m_M[ 3 ] = m_M[ 5 ]*m_M[ 6 ] - m_M[ 3 ]*m_M[ 8 ];
			Adjoint.m_M[ 4 ] = m_M[ 0 ]*m_M[ 8 ] - m_M[ 2 ]*m_M[ 6 ];
			Adjoint.m_M[ 5 ] = m_M[ 2 ]*m_M[ 3 ] - m_M[ 0 ]*m_M[ 5 ];

			Adjoint.m_M[ 6 ] = m_M[ 3 ]*m_M[ 7 ] - m_M[ 4 ]*m_M[ 6 ];
			Adjoint.m_M[ 7 ] = m_M[ 1 ]*m_M[ 6 ] - m_M[ 0 ]*m_M[ 7 ];
			Adjoint.m_M[ 8 ] = m_M[ 0 ]*m_M[ 4 ] - m_M[ 1 ]*m_M[ 3 ];

			return Adjoint;
		}

		ZED_FLOAT32 Matrix3x3::Determinate( ) const
		{
			return m_M[ 0 ]*( m_M[ 4 ]*m_M[ 8 ] - m_M[ 5 ]*m_M[ 7 ]) +
				m_M[ 3 ]*( m_M[ 2 ]*m_M[ 7 ] - m_M[ 1 ]*m_M[ 8 ] ) +
				m_M[ 6 ]*( m_M[ 1 ]*m_M[ 5 ] - m_M[ 2 ]*m_M[ 4 ] );
		}

		ZED_FLOAT32 Matrix3x3::Trace( ) const
		{
			return m_M[ 0 ] + m_M[ 4 ] + m_M[ 8 ];
		}

		System::Writer &operator<<( System::Writer p_Out,
			const Matrix3x3 &p_Source )
		{
			for( ZED_UINT32 i = 0; i < 3; i++ )
			{
				p_Out << "| ";
				for( ZED_UINT32 j = 0; j < 3; j++ )
				{
					p_Out << p_Source.m_M[ j*3 + i ] << " ";
				}
				p_Out << "|" << std::endl;
			}

			return p_Out;
		}

		ZED_BOOL Matrix3x3::operator==( const Matrix3x3 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( Arithmetic::Equal( m_M[ i ], p_Other.m_M[ i ] ) ==
					ZED_FALSE )
				{
					ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix3x3::operator!=( const Matrix3x3 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( Arithmetic::Equal( m_M[ i ], p_Other.m_M[ i ] ) ==
					ZED_TRUE )
				{
					ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		Matrix3x3 Matrix3x3::operator+( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 Return;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				Return.m_M[ i ] = m_M[ i ] + p_Other.m_M[ i ];
			}

			return Return;
		}

		Matrix3x3 Matrix3x3::operator-( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 Return;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				Return.m_M[ i ] = m_M[ i ] + p_Other.m_M[ i ];
			}

			return Return;
		}

		Matrix3x3 Matrix3x3::operator-( ) const
		{
			Matrix3x3 Return;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				Return.m_M[ i ] = -m_M[ i ];
			}

			return Return;
		}

		Matrix3x3 Matrix3x3::operator*( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 Return;

			Return.m_M[ 0 ] =	m_M[ 0 ]*p_Other.m_M[ 0 ] +
								m_M[ 3 ]*p_Other.m_M[ 1 ] +
								m_M[ 6 ]*p_Other.m_M[ 2 ];
			Return.m_M[ 1 ] =	m_M[ 1 ]*p_Other.m_M[ 0 ] +
								m_M[ 4 ]*p_Other.m_M[ 1 ] +
								m_M[ 7 ]*p_Other.m_M[ 2 ];
			Return.m_M[ 2 ] =	m_M[ 2 ]*p_Other.m_M[ 0 ] +
								m_M[ 5 ]*p_Other.m_M[ 1 ] +
								m_M[ 8 ]*p_Other.m_M[ 2 ];

			Return.m_M[ 3 ] =	m_M[ 0 ]*p_Other.m_M[ 3 ] +
								m_M[ 3 ]*p_Other.m_M[ 4 ] +
								m_M[ 6 ]*p_Other.m_M[ 5 ];
			Return.m_M[ 4 ] =	m_M[ 1 ]*p_Other.m_M[ 3 ] +
								m_M[ 4 ]*p_Other.m_M[ 4 ] +
								m_M[ 7 ]*p_Other.m_M[ 5 ];
			Return.m_M[ 5 ] =	m_M[ 2 ]*p_Other.m_M[ 3 ] +
								m_M[ 5 ]*p_Other.m_M[ 4 ] +
								m_M[ 8 ]*p_Other.m_M[ 5 ];

			Return.m_M[ 6 ] =	m_M[ 0 ]*p_Other.m_M[ 6 ] +
								m_M[ 3 ]*p_Other.m_M[ 7 ] +
								m_M[ 6 ]*p_Other.m_M[ 8 ];
			Return.m_M[ 7 ]	=	m_M[ 1 ]*p_Other.m_M[ 6 ] +
								m_M[ 4 ]*p_Other.m_M[ 7 ] +
								m_M[ 7 ]*p_Other.m_M[ 8 ];
			Return.m_M[ 8 ] =	m_M[ 2 ]*p_Other.m_M[ 6 ] +
								m_M[ 5 ]*p_Other.m_M[ 7 ] +
								m_M[ 8 ]*p_Other.m_M[ 8 ];

			return Return;
		}

		Vector3 operator*( const Vector3 &p_Vec, const Matrix3x3 &p_Matrix )
		{
			Vector3 Ret;

			Ret[ 0 ] = p_Matrix.m_M[ 0 ]*p_Vec[ 0 ] + p_Matrix.m_M[ 1 ]*p_Vec[ 1 ] +
				p_Matrix.m_M[ 2 ]*p_Vec[ 2 ];
			Ret[ 1 ] = p_Matrix.m_M[ 3 ]*p_Vec[ 0 ] + p_Matrix.m_M[ 4 ]*p_Vec[ 1 ] +
				p_Matrix.m_M[ 5 ]*p_Vec[ 2 ];
			Ret[ 2 ] = p_Matrix.m_M[ 6 ]*p_Vec[ 0 ] + p_Matrix.m_M[ 7 ]*p_Vec[ 1 ] +
				p_Matrix.m_M[ 8 ]*p_Vec[ 2 ];

			return Ret;
		}

		Matrix3x3 operator*( const ZED_FLOAT32 p_Scalar, const Matrix3x3 &p_Matrix )
		{
			Matrix3x3 Ret;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				Ret.m_M[ i ] = p_Matrix.m_M[ i ]*p_Scalar;
			}

			return Ret;
		}

		Matrix3x3 Matrix3x3::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			Matrix3x3 Ret;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				Ret.m_M[ i ] = m_M[ i ]*p_Scalar;
			}

			return Ret;
		}

		Vector3 Matrix3x3::operator*( const Vector3 &p_Vec ) const
		{
			Vector3 Ret;

			Ret[ 0 ] = m_M[ 0 ]*p_Vec[ 0 ] + m_M[ 3 ]*p_Vec[ 1 ] +
				m_M[ 6 ]*p_Vec[ 2 ];
			Ret[ 1 ] = m_M[ 1 ]*p_Vec[ 0 ] + m_M[ 4 ]*p_Vec[ 1 ] +
				m_M[ 7 ]*p_Vec[ 2 ];
			Ret[ 2 ] = m_M[ 2 ]*p_Vec[ 0 ] + m_M[ 5 ]*p_Vec[ 1 ] +
				m_M[ 8 ]*p_Vec[ 2 ];

			return Ret;
		}

		/*Matrix3x3 &Matrix3x3::operator=( const Matrix3x3 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_M[ i ] = p_Other.m_M[ i ];
			}

			return *this;
		}*/

		Matrix3x3 &Matrix3x3::operator+=( const Matrix3x3 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_M[ i ] += p_Other.m_M[ i ];
			}

			return *this;
		}

		Matrix3x3 &Matrix3x3::operator-=( const Matrix3x3 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_M[ i ] -= p_Other.m_M[ i ];
			}

			return *this;
		}

		Matrix3x3 &Matrix3x3::operator*=( const Matrix3x3 &p_Other )
		{
			m_M[ 0 ] = m_M[ 0 ]*p_Other.m_M[ 0 ] + m_M[ 3 ]*p_Other.m_M[ 1 ] +
				m_M[ 6 ]*p_Other.m_M[ 2 ];
			m_M[ 1 ] = m_M[ 1 ]*p_Other.m_M[ 0 ] + m_M[ 4 ]*p_Other.m_M[ 1 ] +
				m_M[ 7 ]*p_Other.m_M[ 2 ];
			m_M[ 2 ] = m_M[ 2 ]*p_Other.m_M[ 0 ] + m_M[ 5 ]*p_Other.m_M[ 1 ] +
				m_M[ 8 ]*p_Other.m_M[ 2 ];

			m_M[ 3 ] = m_M[ 0 ]*p_Other.m_M[ 3 ] + m_M[ 3 ]*p_Other.m_M[ 4 ] +
				m_M[ 6 ]*p_Other.m_M[ 5 ];
			m_M[ 4 ] = m_M[ 1 ]*p_Other.m_M[ 3 ] + m_M[ 4 ]*p_Other.m_M[ 4 ] +
				m_M[ 7 ]*p_Other.m_M[ 5 ];
			m_M[ 5 ] = m_M[ 2 ]*p_Other.m_M[ 3 ] + m_M[ 5 ]*p_Other.m_M[ 4 ] +
				m_M[ 8 ]*p_Other.m_M[ 5 ];

            m_M[ 6 ] = m_M[ 0 ]*p_Other.m_M[ 6 ] + m_M[ 3 ]*p_Other.m_M[ 7 ] +
				m_M[ 6 ]*p_Other.m_M[ 8 ];
			m_M[ 7 ] = m_M[ 1 ]*p_Other.m_M[ 6 ] + m_M[ 3 ]*p_Other.m_M[ 7 ] +
				m_M[ 7 ]*p_Other.m_M[ 8 ];
			m_M[ 8 ] = m_M[ 2 ]*p_Other.m_M[ 6 ] + m_M[ 3 ]*p_Other.m_M[ 7 ] +
				m_M[ 8 ]*p_Other.m_M[ 8 ];

			return *this;
		}

		Matrix3x3 &Matrix3x3::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_M[ i ] *= p_Scalar;
			}

			return *this;
		}

		ZED_FLOAT32 &Matrix3x3::operator( )( const ZED_UINT32 p_Row,
			const ZED_UINT32 p_Column )
		{
			return m_M[ p_Row+( p_Column*3 ) ];
		}

		ZED_FLOAT32 Matrix3x3::operator( )( const ZED_UINT32 p_Row,
			const ZED_UINT32 p_Column ) const
		{
			return m_M[ p_Row+( p_Column*3 ) ];
		}
	}
}
