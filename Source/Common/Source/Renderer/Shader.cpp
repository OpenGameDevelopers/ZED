#include <Renderer/Shader.hpp>

void zedSetConstant( ZED_SHADER_CONSTANT_MAP *p_pMap, const ZED_UINT32 p_Index,
	const ZED_SHADER_CONSTANT_TYPE p_Type, const ZED_CHAR8 *p_pName )
{
	p_pMap[ p_Index ].Index = p_Index;
	p_pMap[ p_Index ].Type = p_Type;
	p_pMap[ p_Index ].pName = const_cast< ZED_CHAR8 * >( p_pName );
}

