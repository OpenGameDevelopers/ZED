#include <Renderer/ModelStructs.hpp>
#include <cstring>
#include <cstdlib>

void zedZeroFileHeaderV1( ZED::Renderer::LPFILEHEADER_V1 p_pFileHeader )
{
	memset( p_pFileHeader, 0, sizeof( ZED::Renderer::FILEHEADER_V1 ) );
}

void zedZeroMeshV1( ZED::Renderer::LPMESH_V1 p_pMesh )
{
}

void zedZeroFileHeaderV2( ZED::Renderer::LPFILEHEADER_V2 p_pFileHeader )
{
	memset( p_pFileHeader, 0, sizeof( ZED::Renderer::FILEHEADER_V2 ) );
}

