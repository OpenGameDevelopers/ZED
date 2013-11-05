#ifndef __ZED_UTILITY_GRID_HPP__
#define __ZED_UTILITY_GRID_HPP__

#include <Renderer/Renderer.hpp>
#include <Renderer/Shader.hpp>
#include <Arithmetic/Matrix4x4.hpp>

namespace ZED
{
	namespace Utility
	{
		typedef enum
		{
			PLANE_AXIS_XY,
			PLANE_AXIS_XZ,
			PLANE_AXIS_YZ,
			PLANE_AXIS_UNKNOWN
		} PLANE_AXIS;

		class Grid
		{
		public:
			Grid( );
			Grid( const ZED::Renderer::Renderer *p_pRenderer );
			~Grid( );

			ZED_UINT32 Initialise( const ZED_UINT32 p_Rows,
				const ZED_UINT32 p_Columns, const PLANE_AXIS p_Plane,
				const ZED_COLOUR p_Colour, const ZED_FLOAT32 p_Offset,
				const ZED_FLOAT32 p_Stride );

			void Render(
				const ZED::Arithmetic::Matrix4x4 *p_pProjectionView ) const;

		private:
			ZED_UINT32				m_Rows;
			ZED_UINT32				m_Columns;
			PLANE_AXIS				m_Plane;
			ZED_COLOUR				m_Colour;
			ZED_FLOAT32				m_Offset;
			ZED_FLOAT32				m_Stride;

			ZED::Renderer::Renderer	*m_pRenderer;
			ZED::Renderer::Shader	*m_pShader;

			ZED_BYTE	*m_pVertices;
			ZED_UINT16	*m_pIndices;
		};
	}
}

#endif // __ZED_UTILITY_GRID_HPP__

