#include <ModelStructs.hpp>
#include <cstring>
#include <memory>

// Really, these are to ease any cross-platform niggles
void zedZeroFileHeaderV1( ZED::Renderer::LPFILEHEADER_V1 p_pFileHeader )
{
	memset( p_pFileHeader, 0, sizeof( ZED::Renderer::FILEHEADER_V1 ) );
}

void zedZeroMaterialUNK( ZED::Renderer::LPMATERIAL_V1 p_pMaterial )
{
	memset( p_pMaterial, 0, sizeof( ZED::Renderer::MATERIAL_V1 ) );
}

void zedZeroMeshV1( ZED::Renderer::LPMESH_V1 p_pMesh )
{
	memset( p_pMesh, 0, sizeof( ZED::Renderer::MESH_V1 ) );
}

void zedZeroLocationV1( ZED::Renderer::LPLOCATION_V1 p_pLocation )
{
	memset( p_pLocation, 0, sizeof( ZED::Renderer::LOCATION_V1 ) );
}

void zedZeroFileHeaderV2( ZED::Renderer::LPFILEHEADER_V2 p_pFileHeader )
{
	memset( p_pFileHeader, 0, sizeof( ZED::Renderer::FILEHEADER_V2 ) );
}

void zedZeroMeshV2( ZED::Renderer::LPMESH_V2 p_pMesh )
{
	memset( p_pMesh, 0, sizeof( ZED::Renderer::MESH_V2 ) );
}

void zedZeroJointV2( ZED::Renderer::LPJOINT_V2 p_pJoint )
{
	memset( p_pJoint, 0, sizeof( ZED::Renderer::JOINT_V2 ) );
}

void zedZeroJointTV2( ZED::Renderer::LPJOINTT_V2 p_pJointT )
{
	memset( p_pJointT, 0, sizeof( ZED::Renderer::JOINTT_V2 ) );
}

void zedZeroSkeletonV2( ZED::Renderer::LPSKELETON_V2 p_pSkeleton )
{
	memset( p_pSkeleton, 0, sizeof( ZED::Renderer::SKELETON_V2 ) );
}

void zedZeroSkeletonTV2( ZED::Renderer::LPSKELETONT_V2 p_pSkeletonT )
{
	memset( p_pSkeletonT, 0, sizeof( ZED::Renderer::SKELETONT_V2 ) );
}
