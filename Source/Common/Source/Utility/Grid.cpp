#include <Utility/Grid.hpp>
#include <System/Memory.hpp>
#include <cstring>

#if defined ZED_WINDOWSYSTEM_X11
#include <Renderer/OGL/GLShader.hpp>
#endif // ZED_WINDOWSYSTEM_X11

namespace ZED
{
	namespace Utility
	{
		const char *pGridVertexShaderGL30 =
			"#version 130\n"
			"uniform mat4 u_ProjViewMatrix;\n"
			"uniform vec4 u_Colour;\n"
			"in vec3 a_Position;\n"
			"out vec4 v_Colour;\n"
			"void main( )\n"
			"{\n"
			"	v_Colour = u_Colour;\n"
			"	gl_Position = u_ProjViewMatrix * vec4( a_Position, 1.0 );\n"
			"}\n";
		const char *pGridFragmentShaderGL30 =
			"#version 130\n"
			"in vec4 v_Colour;\n"
			"out vec4 o_FragColour;\n"
			"void main( )\n"
			"{\n"
			"	o_FragColour = v_Colour;\n"
			"}\n";

		Grid::Grid( const ZED::Renderer::Renderer *p_pRenderer )
		{
			m_pRenderer =
				const_cast< ZED::Renderer::Renderer * >( p_pRenderer );

			m_Rows = 0;
			m_Columns = 0;
			m_Plane = PLANE_AXIS_UNKNOWN;
			m_Offset = 0.0f;
			m_Stride = 0.0f;

			m_pVertices = ZED_NULL;
			m_pIndices = ZED_NULL;
			m_pShader = ZED_NULL;
		}

		Grid::~Grid( )
		{
			zedSafeDeleteArray( m_pVertices );
			zedSafeDeleteArray( m_pIndices );
			zedSafeDelete( m_pShader );
		}

		ZED_UINT32 Grid::Initialise( const ZED_UINT32 p_Rows,
			const ZED_UINT32 p_Columns, const PLANE_AXIS p_Plane,
			const ZED_COLOUR p_Colour, const ZED_FLOAT32 p_Offset,
			const ZED_FLOAT32 p_Stride )
		{
			m_Rows = p_Rows;
			m_Columns = p_Columns;
			m_Plane = p_Plane;
			m_Colour = p_Colour;
			m_Offset = p_Offset;
			m_Stride = p_Stride;

			switch( m_pRenderer->BackEnd( ) )
			{
				case ZED_RENDERER_BACKEND_OPENGL:
				{
					m_pShader =
						new ZED::Renderer::GLShader( ZED_TRUE, ZED_TRUE,
							ZED_FALSE );
					break;
				}
				default:
				{
					zedTrace( "[ZED::Utility::Grid::Initialise] <ERROR> "
						"Renderer back end invalid\n" );
					return ZED_FAIL;
				}
			}

			ZED_FLOAT32 HalfRows = static_cast< ZED_FLOAT32 >( m_Rows ) / 2.0f;
			ZED_FLOAT32 HalfColumns =
				static_cast< ZED_FLOAT32 >( m_Columns ) / 2.0f;
			ZED::Arithmetic::Vector3 Vertices[ m_Rows * m_Columns * 2 ];
			ZED_FLOAT32 RowStart = -HalfRows * m_Stride;
			ZED_FLOAT32 ColumnStart = -HalfColumns * m_Stride;

			ZED_FLOAT32 RowIndex = RowStart;
			ZED_FLOAT32 ColumnIndex = ColumnStart;
			ZED_UINT32 Row = 0;

			for( Row = 0; Row < p_Rows * 2; ++Row )
			{
				switch( m_Plane )
				{
					case PLANE_AXIS_XZ:
					{
						Vertices[ Row ][ 0 ] = ColumnStart;
						Vertices[ Row ][ 1 ] = 0.0f;
						Vertices[ Row ][ 2 ] = RowIndex;

						++Row;

						Vertices[ Row ][ 0 ] = -ColumnStart;
						Vertices[ Row ][ 1 ] = 0.0f;
						Vertices[ Row ][ 2 ] = RowIndex;
					}
				}
				RowIndex += m_Stride;
			}

			for( ZED_UINT32 Column = 0; Column < p_Columns * 2; ++Column )
			{
				switch( m_Plane )
				{
					case PLANE_AXIS_XZ:
					{
						Vertices[ Row + Column ][ 0 ] = ColumnIndex;
						Vertices[ Row + Column ][ 1 ] = 0.0f;
						Vertices[ Row + Column ][ 2 ] = RowStart;

						++Column;

						Vertices[ Row + Column ][ 0 ] = ColumnIndex;
						Vertices[ Row + Column ][ 1 ] = 0.0f;
						Vertices[ Row + Column ][ 2 ] = -RowStart;
					}
				}

				ColumnIndex += m_Stride;
			}

			m_pVertices = new ZED_BYTE[ sizeof( ZED::Arithmetic::Vector3 ) *
				m_Rows * m_Columns * 2 ];
			memcpy( m_pVertices, Vertices,
				sizeof( ZED::Arithmetic::Vector3 ) * m_Rows * m_Columns * 2 );

			m_pIndices = new ZED_UINT16[ m_Rows * m_Columns * 2 ];

			for( ZED_UINT32 i = 0; i < m_Rows * m_Columns * 2; ++i )
			{
				m_pIndices[ i ] = i;
			}

			if( m_pShader->Compile( &pGridVertexShaderGL30, ZED_VERTEX_SHADER,
				ZED_FALSE ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			if( m_pShader->Compile( &pGridFragmentShaderGL30, ZED_FRAGMENT_SHADER,
				ZED_FALSE ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			ZED_SHADER_CONSTANT_MAP Constants[ 2 ];

			zedSetConstant( Constants, 0, ZED_MAT4X4, "u_ProjViewMatrix" );
			zedSetConstant( Constants, 1, ZED_FLOAT4, "u_Colour" );

			m_pShader->SetConstantTypes( Constants, 2 );

			m_pShader->Activate( );

			ZED_FLOAT32 Colour[ 4 ];
			memcpy( Colour, &m_Colour, sizeof( ZED_FLOAT32 ) * 4 );

			m_pShader->SetConstantData( 1, &m_Colour );

			return ZED_OK;
		}

		void Grid::Render(
			const ZED::Arithmetic::Matrix4x4 *p_pProjectionView ) const
		{
			if( m_Plane == PLANE_AXIS_UNKNOWN )
			{
				return;
			}

			if( m_pRenderer->ShaderSupport( ) )
			{
				m_pShader->Activate( );
				m_pShader->SetConstantData( 1, p_pProjectionView );
			}

			m_pRenderer->Render( m_Rows*m_Columns, m_pVertices,
				m_Rows*m_Columns, m_pIndices, 0x6, 0, ZED_LINE_LIST );
		}
	}
}

