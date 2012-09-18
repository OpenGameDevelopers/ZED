#include <Matrix3x3.hpp>
#include <Quaternion.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Arithmetic
	{
		Matrix3x3::Matrix3x3( const Quaternion &p_Quat )
		{
			Rotate( p_Quat );
		}

		Matrix3x3 &Matrix3x3::Clone( ) const
		{
			Matrix3x3 *pClone = new Matrix3x3( );

			pClone->Copy( *this );

			return *pClone;
		}

		void Matrix3x3::Copy( const Matrix3x3 &p_Original )
		{
			m_M[ 0 ] = p_Original[ 0 ];
			m_M[ 1 ] = p_Original[ 1 ];
			m_M[ 2 ] = p_Original[ 2 ];
			m_M[ 3 ] = p_Original[ 3 ];
			m_M[ 4 ] = p_Original[ 4 ];
			m_M[ 5 ] = p_Original[ 5 ];
			m_M[ 6 ] = p_Original[ 6 ];
			m_M[ 7 ] = p_Original[ 7 ];
			m_M[ 8 ] = p_Original[ 8 ];
		}

		void Matrix3x3::Identity( )
		{
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] =
				m_M[ 6 ] = m_M[ 7 ] = 0.0f;
			m_M[ 0 ] = m_M[ 4 ] = m_M[ 8 ] = 1.0f;
		}

		Matrix3x3 &Matrix3x3::Rotate( const Quaternion &p_Quat )
		{
			return *this;
		}

		Matrix3x3 &Matrix3x3::Rotate( const ZED_FLOAT32 p_Angle,
			const Vector3 &p_Axis )
		{
			ZED_FLOAT32 Cos = 0.0f, Sin = 0.0f;
			SinCos( p_Angle, Sin, Cos );

			ZED_FLOAT32 Tan = 1.0f - Cos;

			Vector3 nAxis;
			nAxis.Copy( p_Axis );

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
			m_M[ 0 ] = TanX*nAxis[ 0 ] + Cos;
			m_M[ 3 ] = TanXY - SinZ;
			m_M[ 6 ] = TanXZ + SinY;
			m_M[ 1 ] = TanXY + SinZ;
			m_M[ 4 ] = TanY*nAxis[ 1 ] + Cos;
			m_M[ 7 ] = TanYZ - SinX;
			m_M[ 2 ] = TanXZ - SinY;
			m_M[ 5 ] = TanYZ + SinX;
			m_M[ 8 ] = TanZ*nAxis[ 2 ] + Cos;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Rotate( const ZED_FLOAT32 p_Z,
			const ZED_FLOAT32 p_Y, const ZED_FLOAT32 p_X )
		{
			// Unroll the concatenation of the X, Y and Z rotation matrices
			ZED_FLOAT32 CosX = 0.0f, SinX = 0.0f;
			ZED_FLOAT32 CosY = 0.0f, SinY = 0.0f;
			ZED_FLOAT32 CosZ = 0.0f, SinZ = 0.0f;

			ZED::Arithmetic::SinCos( p_X, SinX, CosX );
			ZED::Arithmetic::SinCos( p_Y, SinY, CosY );
			ZED::Arithmetic::SinCos( p_Z, SinZ, CosZ );

			m_M[ 0 ] = CosY*CosZ;
			m_M[ 3 ] = -( CosY*SinZ );
			m_M[ 6 ] = SinY;

			m_M[ 1 ] = ( SinX*SinY*CosZ ) + ( CosX*CosZ );
			m_M[ 4 ] = -( SinX*SinY*SinZ ) + ( CosX*CosZ );
			m_M[ 7 ] = -( SinX*CosY );

			m_M[ 2 ] = -( CosX*SinY*CosZ ) + ( SinX*SinZ );
			m_M[ 5 ] = ( CosX*SinY*SinZ ) +( SinX*CosZ );
			m_M[ 8 ] = CosX*CosY;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateX( const ZED_FLOAT32 p_X )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_X, SinTheta, CosTheta );

			m_M[ 0 ] = 1.0f;
			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = 0.0f;
			m_M[ 4 ] = CosTheta;
			m_M[ 5 ] = SinTheta;
			m_M[ 6 ] = 0.0f;
			m_M[ 7 ] = -SinTheta;
			m_M[ 8 ] = CosTheta;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateY( const ZED_FLOAT32 p_Y )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_Y, SinTheta, CosTheta );

			m_M[ 0 ] = CosTheta;
			m_M[ 1 ] = 0.0f;
			m_M[ 2 ] = -SinTheta;
			m_M[ 3 ] = 0.0f;
			m_M[ 4 ] = 1.0f;
			m_M[ 5 ] = 0.0f;
			m_M[ 6 ] = SinTheta;
			m_M[ 7 ] = 0.0f;
			m_M[ 8 ] = CosTheta;

			return *this;
		}

		Matrix3x3 &Matrix3x3::RotateZ( const ZED_FLOAT32 p_Z )
		{
			ZED_FLOAT32 SinTheta = 0.0f, CosTheta = 0.0f;
			ZED::Arithmetic::SinCos( p_Z, SinTheta, CosTheta );

			m_M[ 0 ] = CosTheta;
			m_M[ 1 ] = SinTheta;
			m_M[ 2 ] = 0.0f;
			m_M[ 3 ] = -SinTheta;
			m_M[ 4 ] = CosTheta;
			m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] = 0.0f;
			m_M[ 8 ] = 1.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Scale( const ZED_FLOAT32 p_Value )
		{
			m_M[ 0 ] = m_M[ 4 ] = m_M[ 8 ] = p_Value;

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] =
				m_M[ 6 ] = m_M[ 7 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::Scale( const Vector3 &p_Scale )
		{
			m_M[ 0 ] = p_Scale[ 0 ];
			m_M[ 4 ] = p_Scale[ 1 ];
			m_M[ 8 ] = p_Scale[ 2 ];

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleX( const ZED_FLOAT32 p_X )
		{
			m_M[ 0 ] = p_X;

			m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] = m_M[ 4 ] =
				m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = 0.0f;

			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleY( const ZED_FLOAT32 p_Y )
		{
			m_M[ 4 ] = p_Y;

			m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] =
				m_M[ 5 ] = m_M[ 6 ] = m_M[ 7 ] =
				m_M[ 8 ] = 0.0f;
			
			return *this;
		}

		Matrix3x3 &Matrix3x3::ScaleZ( const ZED_FLOAT32 p_Z )
		{
			m_M[ 8 ] = p_Z;

			m_M[ 0 ] = m_M[ 1 ] = m_M[ 2 ] = m_M[ 3 ] =
				m_M[ 4 ] = m_M[ 5 ] = m_M[ 6 ] =
				m_M[ 7 ] = 0.0f;
			
			return *this;
		}

		ZED_BOOL Matrix3x3::IsZero( ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( !ZED::Arithmetic::IsZero( m_M[ i ] ) )
				{
					return ZED_FALSE;
				}
			}

			return ZED_TRUE;
		}

		ZED_BOOL Matrix3x3::IsIdentity( ) const
		{
			return( ZED::Arithmetic::Equal( 1.0f, m_M[ 0 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_M[ 4 ] ) &&
					ZED::Arithmetic::Equal( 1.0f, m_M[ 8 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 1 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 2 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 3 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 5 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 6 ] ) &&
					ZED::Arithmetic::IsZero( m_M[ 7 ] ) );
		}

		void Matrix3x3::SetRows( const Vector3 &p_Row1, const Vector3 &p_Row2,
			const Vector3 &p_Row3 )
		{
			m_M[ 0 ] = p_Row1[ 0 ];
			m_M[ 3 ] = p_Row1[ 1 ];
			m_M[ 6 ] = p_Row1[ 2 ];

			m_M[ 1 ] = p_Row2[ 0 ];
			m_M[ 4 ] = p_Row2[ 1 ];
			m_M[ 7 ] = p_Row2[ 2 ];

			m_M[ 2 ] = p_Row3[ 0 ];
			m_M[ 5 ] = p_Row3[ 1 ];
			m_M[ 8 ] = p_Row3[ 2 ];
		}

		void Matrix3x3::GetRows( Vector3 &p_Row1, Vector3 &p_Row2,
				Vector3 &p_Row3 ) const
		{
			p_Row1[ 0 ] = m_M[ 0 ];
			p_Row1[ 1 ] = m_M[ 3 ];
			p_Row1[ 2 ] = m_M[ 6 ];

			p_Row2[ 0 ] = m_M[ 1 ];
			p_Row2[ 1 ] = m_M[ 4 ];
			p_Row2[ 2 ] = m_M[ 7 ];

			p_Row3[ 0 ] = m_M[ 2 ];
			p_Row3[ 1 ] = m_M[ 5 ];
			p_Row3[ 2 ] = m_M[ 8 ];
		}

		Vector3 Matrix3x3::GetRow( const ZED_MEMSIZE p_RowNumber ) const
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

			return Vector3( m_M[ BoundsCheck ],
				m_M[ BoundsCheck+3 ], m_M[ BoundsCheck+6 ] );
		}

		void Matrix3x3::SetColumns( const Vector3 &p_Column1,
			const Vector3 &p_Column2, const Vector3 &p_Column3 )
		{
			m_M[ 0 ] = p_Column1[ 0 ];
			m_M[ 1 ] = p_Column1[ 1 ];
			m_M[ 2 ] = p_Column1[ 2 ];

			m_M[ 3 ] = p_Column2[ 0 ];
			m_M[ 4 ] = p_Column2[ 1 ];
			m_M[ 5 ] = p_Column2[ 2 ];

			m_M[ 6 ] = p_Column3[ 0 ];
			m_M[ 7 ] = p_Column3[ 1 ];
			m_M[ 8 ] = p_Column3[ 2 ];
		}

		void Matrix3x3::GetColumns( Vector3 &p_Column1, Vector3 &p_Column2,
			Vector3 &p_Column3 ) const
		{
			p_Column1[ 0 ] = m_M[ 0 ];
			p_Column1[ 1 ] = m_M[ 1 ];
			p_Column1[ 2 ] = m_M[ 2 ];

			p_Column2[ 0 ] = m_M[ 3 ];
			p_Column2[ 1 ] = m_M[ 4 ];
			p_Column2[ 2 ] = m_M[ 5 ];

			p_Column3[ 0 ] = m_M[ 6 ];
			p_Column3[ 1 ] = m_M[ 7 ];
			p_Column3[ 2 ] = m_M[ 8 ];
		}

		Vector3 Matrix3x3::GetColumn( const ZED_MEMSIZE p_ColumnNumber ) const
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

			return Vector3( m_M[ BoundsCheck*3 ],
							m_M[ ( BoundsCheck*3 ) + 1 ],
							m_M[ ( BoundsCheck*3 ) + 2 ] );
		}

		void Matrix3x3::Clean( )
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( ZED::Arithmetic::IsZero( m_M[ i ] ) )
				{
					m_M[ i ] = 0.0f;
				}
			}
		}

		Matrix3x3 &Matrix3x3::Transpose( )
		{
			// Cheaper than creating a copy matrix and returning that
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

			m_M[ 0 ] *= InverseDet;
			m_M[ 1 ] *= InverseDet;
			m_M[ 2 ] *= InverseDet;
			m_M[ 3 ] *= InverseDet;
			m_M[ 4 ] *= InverseDet;
			m_M[ 5 ] *= InverseDet;
			m_M[ 6 ] *= InverseDet;
			m_M[ 7 ] *= InverseDet;
			m_M[ 8 ] *= InverseDet;

			return *this;
		}

		Matrix3x3 Matrix3x3::Adjoint( ) const
		{
			Matrix3x3 ReturnMatrix;

			// Compute transpose of co-factors
			ReturnMatrix.m_M[ 0 ] = ( m_M[ 4 ]*m_M[ 8 ] ) -
										  ( m_M[ 5 ]*m_M[ 7 ] );
			ReturnMatrix.m_M[ 1 ] = ( m_M[ 2 ]*m_M[ 7 ] ) -
										  ( m_M[ 1 ]*m_M[ 8 ] );
			ReturnMatrix.m_M[ 2 ] = ( m_M[ 1 ]*m_M[ 5 ] ) -
										  ( m_M[ 2 ]*m_M[ 4 ] );

			ReturnMatrix.m_M[ 3 ] = ( m_M[ 5 ]*m_M[ 6 ] ) -
										  ( m_M[ 3 ]*m_M[ 8 ] );
			ReturnMatrix.m_M[ 4 ] = ( m_M[ 0 ]*m_M[ 8 ] ) -
										  ( m_M[ 2 ]*m_M[ 6 ] );
			ReturnMatrix.m_M[ 5 ] = ( m_M[ 2 ]*m_M[ 3 ] ) -
										  ( m_M[ 0 ]*m_M[ 5 ] );

			ReturnMatrix.m_M[ 6 ] = ( m_M[ 3 ]*m_M[ 7 ] ) -
										  ( m_M[ 4 ]*m_M[ 6 ] );
			ReturnMatrix.m_M[ 7 ] = ( m_M[ 1 ]*m_M[ 6 ] ) -
										  ( m_M[ 0 ]*m_M[ 7 ] );
			ReturnMatrix.m_M[ 8 ] = ( m_M[ 0 ]*m_M[ 4 ] ) -
										  ( m_M[ 1 ]*m_M[ 3 ] );

			return ReturnMatrix;
		}

		ZED_FLOAT32 Matrix3x3::Determinate( ) const
		{
			return m_M[ 0 ]*( ( m_M[ 4 ]*m_M[ 8 ] ) -
									( m_M[ 5 ]*m_M[ 7 ] ) ) +
				   m_M[ 3 ]*( ( m_M[ 2 ]*m_M[ 7 ] ) -
									( m_M[ 1 ]*m_M[ 8 ] ) ) +
				   m_M[ 6 ]*( ( m_M[ 1 ]*m_M[ 5 ] ) -
									( m_M[ 2 ]*m_M[ 4 ] ) );
		}

		ZED_FLOAT32 Matrix3x3::Trace( ) const
		{
			return m_M[ 0 ] + m_M[ 4 ] + m_M[ 8 ];
		}

		ZED_BOOL Matrix3x3::operator==( const Matrix3x3 &p_Other ) const
		{
			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				if( !ZED::Arithmetic::Equal( m_M[ i ],
					p_Other.m_M[ i ] ) )
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
				if( ZED::Arithmetic::Equal( m_M[ i ],
					p_Other.m_M[ i ] ) )
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
				ReturnMatrix.m_M[ i ] = m_M[ i ] +
					p_Other.m_M[ i ];
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator-( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_M[ i ] = m_M[ i ] -
					p_Other.m_M[ i ];
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator-( ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_M[ i ] = -( m_M[ i ] );
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator*( const Matrix3x3 &p_Other ) const
		{
			Matrix3x3 ReturnMatrix;

			ReturnMatrix.m_M[ 0 ] =
				( m_M[ 0 ]*p_Other.m_M[ 0 ] ) +
				( m_M[ 3 ]*p_Other.m_M[ 1 ] ) +
				( m_M[ 6 ]*p_Other.m_M[ 2 ] );
			ReturnMatrix.m_M[ 1 ] =
				( m_M[ 1 ]*p_Other.m_M[ 0 ] ) +
				( m_M[ 4 ]*p_Other.m_M[ 1 ] ) +
				( m_M[ 7 ]*p_Other.m_M[ 2 ] );
			ReturnMatrix.m_M[ 2 ] =
				( m_M[ 2 ]*p_Other.m_M[ 0 ] ) +
				( m_M[ 5 ]*p_Other.m_M[ 1 ] ) +
				( m_M[ 8 ]*p_Other.m_M[ 2 ] );

			ReturnMatrix.m_M[ 3 ] =
				( m_M[ 0 ]*p_Other.m_M[ 3 ] ) +
				( m_M[ 3 ]*p_Other.m_M[ 4 ] ) +
				( m_M[ 6 ]*p_Other.m_M[ 5 ] );
			ReturnMatrix.m_M[ 4 ] =
				( m_M[ 1 ]*p_Other.m_M[ 3 ] ) +
				( m_M[ 4 ]*p_Other.m_M[ 4 ] ) +
				( m_M[ 7 ]*p_Other.m_M[ 5 ] );
			ReturnMatrix.m_M[ 5 ] =
				( m_M[ 2 ]*p_Other.m_M[ 3 ] ) +
				( m_M[ 5 ]*p_Other.m_M[ 4 ] ) +
				( m_M[ 8 ]*p_Other.m_M[ 5 ] );

			ReturnMatrix.m_M[ 6 ] =
				( m_M[ 0 ]*p_Other.m_M[ 6 ] ) +
				( m_M[ 3 ]*p_Other.m_M[ 7 ] ) +
				( m_M[ 6 ]*p_Other.m_M[ 8 ] );
			ReturnMatrix.m_M[ 7 ] =
				( m_M[ 1 ]*p_Other.m_M[ 6 ] ) +
				( m_M[ 4 ]*p_Other.m_M[ 7 ] ) +
				( m_M[ 7 ]*p_Other.m_M[ 8 ] );
			ReturnMatrix.m_M[ 8 ] =
				( m_M[ 2 ]*p_Other.m_M[ 6 ] ) +
				( m_M[ 5 ]*p_Other.m_M[ 7 ] ) +
				( m_M[ 8 ]*p_Other.m_M[ 8 ] );

			return ReturnMatrix;
		}

		Matrix3x3 operator*( const ZED_FLOAT32 p_Scalar,
			const Matrix3x3 &p_Matrix )
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_M[ i ] = p_Matrix.m_M[ i ]*p_Scalar;
			}

			return ReturnMatrix;
		}

		Matrix3x3 Matrix3x3::operator*( const ZED_FLOAT32 p_Scalar ) const
		{
			Matrix3x3 ReturnMatrix;

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				ReturnMatrix.m_M[ i ] = m_M[ i ]*p_Scalar;
			}

			return ReturnMatrix;
		}

		Vector3 Matrix3x3::operator*( const Vector3 &p_Vec ) const
		{
			Vector3 ReturnVec;

			ReturnVec[ 0 ] = ( m_M[ 0 ]*p_Vec[ 0 ] ) +
							 ( m_M[ 3 ]*p_Vec[ 1 ] ) +
							 ( m_M[ 6 ]*p_Vec[ 2 ] );
			ReturnVec[ 1 ] = ( m_M[ 1 ]*p_Vec[ 0 ] ) +
							 ( m_M[ 4 ]*p_Vec[ 1 ] ) +
							 ( m_M[ 7 ]*p_Vec[ 2 ] );
			ReturnVec[ 2 ] = ( m_M[ 2 ]*p_Vec[ 0 ] ) +
							 ( m_M[ 5 ]*p_Vec[ 1 ] ) +
							 ( m_M[ 8 ]*p_Vec[ 2 ] );

			return Vector3( ReturnVec.m_X, ReturnVec.m_Y, ReturnVec.m_Z );
		}

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
			Matrix3x3 ResultMatrix;

			ResultMatrix.m_M[ 0 ] =
				( m_M[ 0 ]*p_Other.m_M[ 0 ] ) +
				( m_M[ 3 ]*p_Other.m_M[ 1 ] ) +
				( m_M[ 6 ]*p_Other.m_M[ 2 ] );
			ResultMatrix.m_M[ 1 ] =
				( m_M[ 1 ]*p_Other.m_M[ 0 ] ) +
				( m_M[ 4 ]*p_Other.m_M[ 1 ] ) +
				( m_M[ 7 ]*p_Other.m_M[ 2 ] );
			ResultMatrix.m_M[ 2 ] =
				( m_M[ 2 ]*p_Other.m_M[ 0 ] ) +
				( m_M[ 5 ]*p_Other.m_M[ 1 ] ) +
				( m_M[ 8 ]*p_Other.m_M[ 2 ] );

			ResultMatrix.m_M[ 3 ] =
				( m_M[ 0 ]*p_Other.m_M[ 3 ] ) +
				( m_M[ 3 ]*p_Other.m_M[ 4 ] ) +
				( m_M[ 6 ]*p_Other.m_M[ 5 ] );
			ResultMatrix.m_M[ 4 ] =
				( m_M[ 1 ]*p_Other.m_M[ 3 ] ) +
				( m_M[ 4 ]*p_Other.m_M[ 4 ] ) +
				( m_M[ 7 ]*p_Other.m_M[ 5 ] );
			ResultMatrix.m_M[ 5 ] =
				( m_M[ 2 ]*p_Other.m_M[ 3 ] ) +
				( m_M[ 5 ]*p_Other.m_M[ 4 ] ) +
				( m_M[ 8 ]*p_Other.m_M[ 5 ] );

			ResultMatrix.m_M[ 6 ] =
				( m_M[ 0 ]*p_Other.m_M[ 6 ] ) +
				( m_M[ 3 ]*p_Other.m_M[ 7 ] ) +
				( m_M[ 6 ]*p_Other.m_M[ 8 ] );
			ResultMatrix.m_M[ 7 ] =
				( m_M[ 1 ]*p_Other.m_M[ 6 ] ) +
				( m_M[ 4 ]*p_Other.m_M[ 7 ] ) +
				( m_M[ 7 ]*p_Other.m_M[ 8 ] );
			ResultMatrix.m_M[ 8 ] =
				( m_M[ 2 ]*p_Other.m_M[ 6 ] ) +
				( m_M[ 5 ]*p_Other.m_M[ 7 ] ) +
				( m_M[ 8 ]*p_Other.m_M[ 8 ] );

			for( ZED_UINT32 i = 0; i < 9; i++ )
			{
				m_M[ i ] = ResultMatrix.m_M[ i ];
			}

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

		ZED_FLOAT32 &Matrix3x3::operator( )( const ZED_MEMSIZE p_Row,
			const ZED_MEMSIZE p_Column )
		{
			// No error checking!
			return m_M[ p_Row + ( 3*p_Column ) ];
		}

		ZED_FLOAT32 Matrix3x3::operator( )( const ZED_MEMSIZE p_Row,
				const ZED_MEMSIZE p_Column ) const
		{
			// No error checking!
			return m_M[ p_Row + ( 3*p_Column ) ];
		}
	}
}
