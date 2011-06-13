#include <ModelStructs.hpp>
#include <memory>

// Really, these are to ease any cross-platform niggles
void zedZeroFileHeaderV1( ZED::Renderer::LPFILEHEADER_V1 p_pFileHeader )
{
	memset( p_pFileHeader, 0, sizeof( ZED::Renderer::LPFILEHEADER_V1 ) );
}

void zedZeroMaterialUNK( ZED::Renderer::LPMATERIAL_V1 p_pMaterial )
{
	memset( p_pMaterial, 0, sizeof( ZED::Renderer::LPMATERIAL_V1 ) );
}

void zedZeroMeshV1( ZED::Renderer::LPMESH_V1 p_pMesh )
{
	memset( p_pMesh, 0, sizeof( ZED::Renderer::LPMESH_V1 ) );
}

void zedZeroLocationV1( ZED::Renderer::LPLOCATION_V1 p_pLocation )
{
	memset( p_pLocation, 0, sizeof( ZED::Renderer::LPLOCATION_V1 ) );
}
