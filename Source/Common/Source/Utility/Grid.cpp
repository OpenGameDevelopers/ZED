#include <Utility/Grid.hpp>

namespace ZED
{
	namespace Utility
	{
		Grid::Grid( const ZED::Renderer::Renderer *p_pRenderer )
		{
			m_pRenderer =
				const_cast< ZED::Renderer::Renderer * >( p_pRenderer );
		}

		Grid::~Grid( )
		{
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

			return ZED_OK;
		}

		void Grid::Render( ) const
		{
		}
	}
}

