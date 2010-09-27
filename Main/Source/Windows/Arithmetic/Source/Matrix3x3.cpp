#include <Matrix3x3.hpp>
//#include <Quaternion.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Matrix3x3::Matrix3x3( const Quaternion &p_Quat )
		{
			Rotate( p_Quat );
		}

		void Matrix3x3::Identity( )
		{
			m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] = m_Element[ 5 ] =
				m_Element[ 6 ] = m_Element[ 7 ] = 0.0f;
			m_Element[ 0 ] = m_Element[ 4 ] = m_Element[ 8 ] = 1.0f;
		}

		/*Matrix3x3 &Matrix3x3::Rotate( const Quaternion &p_Quat )
		{
		}*/

		Matrix3x3 &Matrix3x3::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			ZED_FLOAT32 Cos = 0.0f, Sin = 0.0f;
			SinCos( p_Angle, Sin, Cos );

			ZED_FLOAT32 Tan = 1.0f - Cos;

			Vector3 nAxis = p_Axis;

			// Intermediate values
			ZED_FLOAT32 TanX = Tan*nAxis[ 0 ];
			ZED_FLOAT32 TanY = Tan*nAxis[ 1 ];
			ZED_FLOAT32 TanZ = Tan*nAxis[ 2 ];

			ZED_FLOAT32 SinX = Sin*nAxis[ 0 ];
			ZED_FLOAT32 SinY = Sin*nAxis[ 1 ];
			ZED_FLOAT32 SinZ = Sin*nAxis[ 2 ];
			
			ZED_FLOAT32 TanXY = TanX*nAxis[ 1 ];
			ZED_FLOAT32 TanYZ = TanY*nAxis[ 2 ];
			ZED_FLOAT32 TanXZ = TanX*nAxis[ 2 ];

			// Set up the matrix
			m_Element[ 0 ] = TanX*nAxis[ 0 ] + Cos;
			m_Element[ 3 ] = TanXY - SinZ;
			m_Element[ 6 ] = TanXZ + SinY;
			m_Element[ 1 ] = TanXY + SinZ;
			m_Element[ 4 ] = TanY*nAxis[ 1 ] + Cos;
			m_Element[ 7 ] = TanYZ - SinX;
			m_Element[ 2 ] = TanXZ - SinY;
			m_Element[ 5 ] = TanYZ + SinX;
			m_Element[ 8 ] = TanZ*nAxis[ 2 ] + Cos;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateXYZ( const ZED_FLOAT32 p_X,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_Z )
		{
			// Unroll the contatenation of the X, Y and Z rotation matrices
			ZED_FLOAT32 CosX = 0.0f, SinX = 0.0f;
			ZED_FLOAT32 CosY = 0.0f, SinY = 0.0f;
			ZED_FLOAT32 CosZ = 0.0f, SinZ = 0.0f;

			ZED::Arithmetic::SinCos( p_X, SinX, CosX );
			ZED::Arithmetic::SinCos( p_Y, SinY, CosY );
			ZED::Arithmetic::SinCos( p_Z, SinZ, CosZ );

			m_Element[ 0 ] = CosY*CosZ;
			m_Element[ 3 ] = -( CosY*SinZ );
			m_Element[ 6 ] = SinY;

			m_Element[ 1 ] = ( SinX*SinY*CosZ ) + ( CosX*CosZ );
			m_Element[ 4 ] = -( SinX*SinY*SinZ ) + ( CosX*CosZ );
			m_Element[ 7 ] = -( SinX*CosY );

			m_Element[ 2 ] = -( CosX*SinY*CosZ ) + ( SinX*SinZ );
			m_Element[ 5 ] = ( CosX*SinY*SinZ ) +( SinX*CosZ );
			m_Element[ 8 ] = CosX*CosY;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateX( const ZED_FLOAT32 p_X )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_X, SinTheta, CosTheta );

			m_Element[ 0 ] = 1.0f;
			m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] = 0.0f;
			m_Element[ 4 ] = CosTheta;
			m_Element[ 5 ] = SinTheta;
			m_Element[ 6 ] = 0.0f;
			m_Element[ 7 ] = -SinTheta;
			m_Element[ 8 ] = CosTheta;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateY( const ZED_FLOAT32 p_Y )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_Y, SinTheta, CosTheta );

			m_Element[ 0 ] = CosTheta;
			m_Element[ 1 ] = 0.0f;
			m_Element[ 2 ] = -SinTheta;
			m_Element[ 3 ] = 0.0f;
			m_Element[ 4 ] = 1.0f;
			m_Element[ 5 ] = 0.0f;
			m_Element[ 6 ] = SinTheta;
			m_Element[ 7 ] = 0.0f;
			m_Element[ 8 ] = CosTheta;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateZ( const ZED_FLOAT32 p_Z )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_Z, SinTheta, CosTheta );

			m_Element[ 0 ] = CosTheta;
			m_Element[ 1 ] = SinTheta;
			m_Element[ 2 ] = 0.0f;
			m_Element[ 3 ] = -SinTheta;
			m_Element[ 4 ] = CosTheta;
			m_Element[ 5 ] = m_Element[ 6 ] = m_Element[ 7 ] = 0.0f;
			m_Element[ 8 ] = 1.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Scale( const ZED_FLOAT32 p_Value )
		{
			m_Element[ 0 ] = m_Element[ 4 ] = m_Element[ 8 ] = p_Value;

			m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] = m_Element[ 5 ] =
				m_Element[ 6 ] = m_Element[ 7 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleX( const ZED_FLOAT32 p_X )
		{
			m_Element[ 0 ] = p_X;

			m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] = m_Element[ 4 ] =
				m_Element[ 5 ] = m_Element[ 6 ] = m_Element[ 7 ] =
				m_Element[ 8 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleY( const ZED_FLOAT32 p_Y )
		{
			m_Element[ 4 ] = p_Y;

			m_Element[ 0 ] = m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] =
				m_Element[ 5 ] = m_Element[ 6 ] = m_Element[ 7 ] =
				m_Element[ 8 ] = 0.0f;
			
			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleZ( const ZED_FLOAT32 p_Z )
		{
			m_Element[ 8 ] = p_Z;

			m_Element[ 0 ] = m_Element[ 1 ] = m_Element[ 2 ] = m_Element[ 3 ] =
				m_Element[ 4 ] = m_Element[ 5 ] = m_Element[ 6 ] =
				m_Element[ 7 ] = 0.0f;
			
			return *this;
		}

		ZED_BOOL Matrix3x3::IsZero( ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( !ZED::Arithmetic::IsZero( m_Element[ i ] ) )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix3x3::IsIdentity( ) const
		{
			return( ZED::Arithmetic::Equal( 1.0f, m_Element[ 0 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_Element[ 4 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_Element[ 8 ] ) &&
					ZED::Arithmetic::IsZero( m_Element[ 1 ] ) &&
					ZED::Arithmetic::IsZero( m_Element[ 2 ] ) &&
					ZED::Arithmetic::IsZero( m_Element[ 3 ] ) &&
					ZED::Arithmetic::IsZero( m_Element[ 5 ] ) &&
					ZED::Arithmetic::IsZero( m_Element[ 6 ] ) &&
					ZED::Arithmetic::IsZero( m_Element[ 7 ] ) );
		}

		void Matrix3x3::SetRows( const Vector3 &p_Row1, const Vector3 &p_Row2,
			const Vector3 &p_Row3 )
		{
			m_Element[ 0 ] = p_Row1[ 0 ];
			m_Element[ 3 ] = p_Row1[ 1 ];
			m_Element[ 6 ] = p_Row1[ 2 ];

			m_Element[ 1 ] = p_Row2[ 0 ];
			m_Element[ 4 ] = p_Row2[ 1 ];
			m_Element[ 7 ] = p_Row2[ 2 ];

			m_Element[ 2 ] = p_Row3[ 0 ];
			m_Element[ 5 ] = p_Row3[ 1 ];
			m_Element[ 8 ] = p_Row3[ 2 ];
		}

		void Matrix3x3::GetRows( Vector3 &p_Row1, Vector3 &p_Row2,
				Vector3 &p_Row3 ) const
		{
			p_Row1[ 0 ] = m_Element[ 0 ];
			p_Row1[ 1 ] = m_Element[ 3 ];
			p_Row1[ 2 ] = m_Element[ 6 ];

			p_Row2[ 0 ] = m_Element[ 1 ];
			p_Row2[ 1 ] = m_Element[ 4 ];
			p_Row2[ 2 ] = m_Element[ 7 ];

			p_Row3[ 0 ] = m_Element[ 2 ];
			p_Row3[ 1 ] = m_Element[ 5 ];
			p_Row3[ 2 ] = m_Element[ 8 ];
		}

		Vector3 Matrix3x3::GetRow( const ZED_UINT32 p_RowNumber ) const
		{
			// An assert may be cheaper

			ZED_UINT32 BoundsCheck = 0;
			// Make sure the value is valid
			if( p_RowNumber < 0 )
			{
				BoundsCheck = 0;
			}
			else if( p_RowNumber > 3 )
			{
				BoundsCheck = 3;
			}
			else
			{
				BoundsCheck = p_RowNumber;
			}

			return Vector3( m_Element[ BoundsCheck ],
				m_Element[ BoundsCheck+3 ], m_Element[ BoundsCheck+6 ] );
		}

		void Matrix3x3::SetColumns( const Vector3 &p_Column1,
			const Vector3 &p_Column2, const Vector3 &p_Column3 )
		{
			m_Element[ 0 ] = p_Column1[ 0 ];
			m_Element[ 1 ] = p_Column1[ 1 ];
			m_Element[ 2 ] = p_Column1[ 2 ];

			m_Element[ 3 ] = p_Column2[ 0 ];
			m_Element[ 4 ] = p_Column2[ 1 ];
			m_Element[ 5 ] = p_Column2[ 2 ];

			m_Element[ 6 ] = p_Column3[ 0 ];
			m_Element[ 7 ] = p_Column3[ 1 ];
			m_Element[ 8 ] = p_Column3[ 2 ];
		}

		void Matrix3x3::GetColumns( Vector3 &p_Column1, Vector3 &p_Column2,
			Vector3 &p_Column3 ) const
		{
			p_Column1[ 0 ] = m_Element[ 0 ];
			p_Column1[ 1 ] = m_Element[ 1 ];
			p_Column1[ 2 ] = m_Element[ 2 ];

			p_Column2[ 0 ] = m_Element[ 3 ];
			p_Column2[ 1 ] = m_Element[ 4 ];
			p_Column2[ 2 ] = m_Element[ 5 ];

			p_Column3[ 0 ] = m_Element[ 6 ];
			p_Column3[ 1 ] = m_Element[ 7 ];
			p_Column3[ 2 ] = m_Element[ 8 ];
		}

		Vector3 Matrix3x3::GetColumn( const ZED_UINT32 p_ColumnNumber ) const
		{
			// Again, assert could be cheaper

			ZED_UINT32 BoundsCheck = 0;

			if( p_ColumnNumber < 0 )
			{
				BoundsCheck = 0;
			}
			else if( p_ColumnNumber > 3 )
			{
				BoundsCheck = 3;
			}
			else
			{
				BoundsCheck = p_ColumnNumber;
			}

			return Vector3( m_Element[ BoundsCheck*3 ],
							m_Element[ ( BoundsCheck*3 ) + 1 ],
							m_Element[ ( BoundsCheck*3 ) + 2 ] );
		}

		void Matrix3x3::Clean( )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( ZED::Arithmetic::IsZero( m_Element[ i ] ) )
				{
					m_Element[ i ] = 0.0f;
				}
			}
		}

		Matrix3x3 &Matrix3x3::Transpose( )
		{
			// Cheaper than creating a copy matrix and returning that
			ZED_FLOAT32 Temp = m_Element[ 1 ];
			m_Element[ 1 ] = m_Element[ 3 ];
			m_Element[ 3 ] = Temp;

			Temp = m_Element[ 2 ];
			m_Element[ 2 ] = m_Element[ 6 ];
			m_Element[ 6 ] = Temp;

			Temp = m_Element[ 5 ];
			m_Element[ 5 ] = m_Element[ 7 ];			
			m_Element[ 7 ] = Temp;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Inverse( )
		{
			// Compute determinate
			ZED_FLOAT32 Det = Determinate( );

			if( ZED::Arithmetic::IsZero( Det ) )
			{
				zedTrace( "Error: Matrix::Inverse -- singular matrix!" );
				return *this;
			}

			// Set this matrix as the adjoint and multiply by 1/Det to get the
			// inverse
			ZED_FLOAT32 InverseDet = 1.0f / Det;
			*this = Adjoint( );

			m_Element[ 0 ] *= InverseDet;
			m_Element[ 1 ] *= InverseDet;
			m_Element[ 2 ] *= InverseDet;
			m_Element[ 3 ] *= InverseDet;
			m_Element[ 4 ] *= InverseDet;
			m_Element[ 5 ] *= InverseDet;
			m_Element[ 6 ] *= InverseDet;
			m_Element[ 7 ] *= InverseDet;
			m_Element[ 8 ] *= InverseDet;

			return *this;
		}

		Matrix3x3 Matrix3x3::Adjoint( ) const
		{
			Matrix3x3 ReturnMatrix;

			// Compute transpose of co-factors
			ReturnMatrix.m_Element[ 0 ] = ( m_Element[ 4 ]*m_Element[ 8 ] ) -
										  ( m_Element[ 5 ]*m_Element[ 7 ] );
			ReturnMatrix.m_Element[ 1 ] = ( m_Element[ 2 ]*m_Element[ 7 ] ) -
										  ( m_Element[ 1 ]*m_Element[ 8 ] );
			ReturnMatrix.m_Element[ 2 ] = ( m_Element[ 1 ]*m_Element[ 5 ] ) -
										  ( m_Element[ 2 ]*m_Element[ 4 ] );

			ReturnMatrix.m_Element[ 3 ] = ( m_Element[ 5 ]*m_Element[ 6 ] ) -
										  ( m_Element[ 3 ]*m_Element[ 8 ] );
			ReturnMatrix.m_Element[ 4 ] = ( m_Element[ 0 ]*m_Element[ 8 ] ) -
										  ( m_Element[ 2 ]*m_Element[ 6 ] );
			ReturnMatrix.m_Element[ 5 ] = ( m_Element[ 2 ]*m_Element[ 3 ] ) -
										  ( m_Element[ 0 ]*m_Element[ 5 ] );

			ReturnMatrix.m_Element[ 6 ] = ( m_Element[ 3 ]*m_Element[ 7 ] ) -
										  ( m_Element[ 4 ]*m_Element[ 6 ] );
			ReturnMatrix.m_Element[ 7 ] = ( m_Element[ 1 ]*m_Element[ 6 ] ) -
										  ( m_Element[ 0 ]*m_Element[ 7 ] );
			ReturnMatrix.m_Element[ 8 ] = ( m_Element[ 0 ]*m_Element[ 4 ] ) -
										  ( m_Element[ 1 ]*m_Element[ 3 ] );

			return ReturnMatrix;
		}

		ZED_FLOAT32 Matrix3x3::Determinate( ) const
		{
			return m_Element[ 0 ]*( ( m_Element[ 4 ]*m_Element[ 8 ] ) -
									( m_Element[ 5 ]*m_Element[ 7 ] ) ) +
				   m_Element[ 3 ]*( ( m_Element[ 2 ]*m_Element[ 7 ] ) -
									( m_Element[ 1 ]*m_Element[ 8 ] ) ) +
				   m_Element[ 6 ]*( ( m_Element[ 1 ]*m_Element[ 5 ] ) -
									( m_Element[ 2 ]*m_Element[ 4 ] ) );
		}

		ZED_FLOAT32 Matrix3x3::Trace( ) const
		{
			return m_Element[ 0 ] + m_Element[ 4 ] + m_Element[ 8 ];
		}

		ZED_BOOL Matrix3x3::operator==( const Matrix3x3 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( !ZED::Arithmetic::Equal( m_Element[ i ],
					p_Other.m_Element[ i ] ) )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix3x3::operator!=( const Matrix3x3 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 0; i++ )
			{
				if( ZED::Arithmetic::Equal( m_Element[ i ],
					p_Other.m_Element[ i ] ) )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		Matrix3x3 Matrix3x3::operator+( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_Element[ i ] = m_Element[ i ] +
					p_Other.m_Element[ i ];
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator-( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_Element[ i ] = m_Element[ i ] -
					p_Other.m_Element[ i ];
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator-( ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_Element[ i ] = -( m_Element[ i ] );
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator*( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 ReturnMatrix;

			ReturnMatrix.m_Element[ 0 ] =
				( m_Element[ 0 ]*p_Other.m_Element[ 0 ] ) +
				( m_Element[ 3 ]*p_Other.m_Element[ 1 ] ) +
				( m_Element[ 6 ]*p_Other.m_Element[ 2 ] );
			ReturnMatrix.m_Element[ 1 ] =
				( m_Element[ 1 ]*p_Other.m_Element[ 0 ] ) +
				( m_Element[ 4 ]*p_Other.m_Element[ 1 ] ) +
				( m_Element[ 7 ]*p_Other.m_Element[ 2 ] );
			ReturnMatrix.m_Element[ 2 ] =
				( m_Element[ 2 ]*p_Other.m_Element[ 0 ] ) +
				( m_Element[ 5 ]*p_Other.m_Element[ 1 ] ) +
				( m_Element[ 8 ]*p_Other.m_Element[ 2 ] );

			ReturnMatrix.m_Element[ 3 ] =
				( m_Element[ 0 ]*p_Other.m_Element[ 3 ] ) +
				( m_Element[ 3 ]*p_Other.m_Element[ 4 ] ) +
				( m_Element[ 6 ]*p_Other.m_Element[ 5 ] );
			ReturnMatrix.m_Element[ 4 ] =
				( m_Element[ 1 ]*p_Other.m_Element[ 3 ] ) +
				( m_Element[ 4 ]*p_Other.m_Element[ 4 ] ) +
				( m_Element[ 7 ]*p_Other.m_Element[ 5 ] );
			ReturnMatrix.m_Element[ 5 ] =
				( m_Element[ 2 ]*p_Other.m_Element[ 3 ] ) +
				( m_Element[ 5 ]*p_Other.m_Element[ 4 ] ) +
				( m_Element[ 8 ]*p_Other.m_Element[ 5 ] );

			ReturnMatrix.m_Element[ 6 ] =
				( m_Element[ 0 ]*p_Other.m_Element[ 6 ] ) +
				( m_Element[ 3 ]*p_Other.m_Element[ 7 ] ) +
				( m_Element[ 6 ]*p_Other.m_Element[ 8 ] );
			ReturnMatrix.m_Element[ 7 ] =
				( m_Element[ 1 ]*p_Other.m_Element[ 6 ] ) +
				( m_Element[ 4 ]*p_Other.m_Element[ 7 ] ) +
				( m_Element[ 7 ]*p_Other.m_Element[ 8 ] );
			ReturnMatrix.m_Element[ 8 ] =
				( m_Element[ 2 ]*p_Other.m_Element[ 6 ] ) +
				( m_Element[ 5 ]*p_Other.m_Element[ 7 ] ) +
				( m_Element[ 8 ]*p_Other.m_Element[ 8 ] );

			return ReturnMatrix;
		}

		Vector3 operator*( const Vector3 &p_Vec, const Matrix3x3 &p_Matrix )
		{
			Vector3 ReturnVec;

			ReturnVec[ 0 ] = ( p_Matrix.m_Element[ 0 ]*p_Vec[ 0 ] ) +
							 ( p_Matrix.m_Element[ 1 ]*p_Vec[ 1 ] ) +
							 ( p_Matrix.m_Element[ 2 ]*p_Vec[ 2 ] );
			ReturnVec[ 1 ] = ( p_Matrix.m_Element[ 3 ]*p_Vec[ 0 ] ) +
							 ( p_Matrix.m_Element[ 4 ]*p_Vec[ 1 ] ) +
							 ( p_Matrix.m_Element[ 5 ]*p_Vec[ 2 ] );
			ReturnVec[ 2 ] = ( p_Matrix.m_Element[ 6 ]*p_Vec[ 0 ] ) +
							 ( p_Matrix.m_Element[ 7 ]*p_Vec[ 1 ] ) +
							 ( p_Matrix.m_Element[ 8 ]*p_Vec[ 2 ] );

			return ReturnVec;
		}

		Matrix3x3 operator*( const ZED_FLOAT32 p_Scalar,
			const Matrix3x3 &p_Matrix )
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_Element[ i ] = p_Matrix.m_Element[ i ]*p_Scalar;
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_Element[ i ] = m_Element[ i ]*p_Scalar;
			}

			return ReturnMatrix;
		}

		Vector3 Matrix3x3::operator*( const Vector3 &p_Vec ) const
		{
			Vector3 ReturnVec;

			ReturnVec[ 0 ] = ( m_Element[ 0 ]*p_Vec[ 0 ] ) +
							 ( m_Element[ 3 ]*p_Vec[ 1 ] ) +
							 ( m_Element[ 6 ]*p_Vec[ 2 ] );
			ReturnVec[ 1 ] = ( m_Element[ 1 ]*p_Vec[ 0 ] ) +
							 ( m_Element[ 4 ]*p_Vec[ 1 ] ) +
							 ( m_Element[ 7 ]*p_Vec[ 2 ] );
			ReturnVec[ 2 ] = ( m_Element[ 2 ]*p_Vec[ 0 ] ) +
							 ( m_Element[ 5 ]*p_Vec[ 1 ] ) +
							 ( m_Element[ 8 ]*p_Vec[ 2 ] );

			return ReturnVec;
		}

		Matrix3x3 &Matrix3x3::operator+=( const Matrix3x3 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_Element[ i ] += p_Other.m_Element[ i ];
			}
			
			return *this;
		}

		Matrix3x3 &Matrix3x3::operator-=( const Matrix3x3 &p_Other )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_Element[ i ] -= p_Other.m_Element[ i ];
			}

			return *this;
		}

		Matrix3x3 &Matrix3x3::operator*=( const Matrix3x3 &p_Other )
		{
			Matrix3x3 ResultMatrix;

			ResultMatrix.m_Element[ 0 ] =
				( m_Element[ 0 ]*p_Other.m_Element[ 0 ] ) +
				( m_Element[ 3 ]*p_Other.m_Element[ 1 ] ) +
				( m_Element[ 6 ]*p_Other.m_Element[ 2 ] );
			ResultMatrix.m_Element[ 1 ] =
				( m_Element[ 1 ]*p_Other.m_Element[ 0 ] ) +
				( m_Element[ 4 ]*p_Other.m_Element[ 1 ] ) +
				( m_Element[ 7 ]*p_Other.m_Element[ 2 ] );
			ResultMatrix.m_Element[ 2 ] =
				( m_Element[ 2 ]*p_Other.m_Element[ 0 ] ) +
				( m_Element[ 5 ]*p_Other.m_Element[ 1 ] ) +
				( m_Element[ 8 ]*p_Other.m_Element[ 2 ] );

			ResultMatrix.m_Element[ 3 ] =
				( m_Element[ 0 ]*p_Other.m_Element[ 3 ] ) +
				( m_Element[ 3 ]*p_Other.m_Element[ 4 ] ) +
				( m_Element[ 6 ]*p_Other.m_Element[ 5 ] );
			ResultMatrix.m_Element[ 4 ] =
				( m_Element[ 1 ]*p_Other.m_Element[ 3 ] ) +
				( m_Element[ 4 ]*p_Other.m_Element[ 4 ] ) +
				( m_Element[ 7 ]*p_Other.m_Element[ 5 ] );
			ResultMatrix.m_Element[ 5 ] =
				( m_Element[ 2 ]*p_Other.m_Element[ 3 ] ) +
				( m_Element[ 5 ]*p_Other.m_Element[ 4 ] ) +
				( m_Element[ 8 ]*p_Other.m_Element[ 5 ] );

			ResultMatrix.m_Element[ 6 ] =
				( m_Element[ 0 ]*p_Other.m_Element[ 6 ] ) +
				( m_Element[ 3 ]*p_Other.m_Element[ 7 ] ) +
				( m_Element[ 6 ]*p_Other.m_Element[ 8 ] );
			ResultMatrix.m_Element[ 7 ] =
				( m_Element[ 1 ]*p_Other.m_Element[ 6 ] ) +
				( m_Element[ 4 ]*p_Other.m_Element[ 7 ] ) +
				( m_Element[ 7 ]*p_Other.m_Element[ 8 ] );
			ResultMatrix.m_Element[ 8 ] =
				( m_Element[ 2 ]*p_Other.m_Element[ 6 ] ) +
				( m_Element[ 5 ]*p_Other.m_Element[ 7 ] ) +
				( m_Element[ 8 ]*p_Other.m_Element[ 8 ] );

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_Element[ i ] = ResultMatrix.m_Element[ i ];
			}

			return *this;
		}

		Matrix3x3 &Matrix3x3::operator*=( const ZED_FLOAT32 p_Scalar )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_Element[ i ] *= p_Scalar;
			}

			return *this;
		}

		ZED_FLOAT32 &Matrix3x3::operator( )( const ZED_UINT32 p_Row,
			const ZED_UINT32 p_Column )
		{
			// No error checking!
			return m_Element[ p_Row + ( 3*p_Column ) ];
		}

		ZED_FLOAT32 Matrix3x3::operator( )( const ZED_UINT32 p_Row,
				const ZED_UINT32 p_Column ) const
		{
			// No error checking!
			return m_Element[ p_Row + ( 3*p_Column ) ];
		}
	}
}
