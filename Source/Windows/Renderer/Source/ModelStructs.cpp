#include <ModelStructs.hpp>
#include <memory>

// Really, these are to ease any cross-platform niggles
void ZeroFileHeader( LPFILEHEADER p_pFileHeader )
{
	memset( p_pFileHeader, 0, sizeof( FILEHEADER ) );
}

void ZeroMaterial( LPMATERIAL p_pMaterial )
{
	memset( p_pMaterial, 0, sizeof( MATERIAL ) );
}

void ZeroMesh( LPMESH p_pMesh )
{
	memset( p_pMesh, 0, sizeof( MESH ) );
}

void ZeroLocation( LPLOCATION p_pLocation )
{
	memset( p_pLocation, 0, sizeof( LOCATION ) );
}
