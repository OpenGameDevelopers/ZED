#ifndef __ZEDMODELSTRUCTS_HPP__
#define __ZEDMODELSTRUCTS_HPP__

#include <DataTypes.hpp>

// Pack the structs into single bytes
#pragma pack( 1 )

// The header goes before each type, defining what is to be processed
typedef struct _HEADER
{
	ZED_UINT16	ID;
	ZED_UINT64	Size;
} HEADER, *LPHEADER;

// The header for the whole file, contains everything to describe a ZED asset
typedef struct _FILEHEADER
{
	// The asset's name (of course)
	ZED_UCHAR8	Name[ 32 ];
	// File version [Major.Minor.Revision]
	ZED_UINT32	Version[ 3 ];
	// Type of file being written [ZED]
	ZED_UCHAR8	ID[ 3 ];
	// The type of file to process [A|C|G|W]
	ZED_UCHAR8	Type;
	// Endian-ness [E|e]
	ZED_UCHAR8	Endianness;
} FILEHEADER, *LPFILEHEADER;

// Vertices define the position, normal and UV coords of a point in space
typedef struct _VERTEX
{
	ZED_FLOAT32	Position[ 3 ];
	ZED_FLOAT32	Normal[ 3 ];
	ZED_FLOAT32 UV[ 2 ];
} VERTEX, *LPVERTEX;

// Faces are used to describe planar surfaces consisting of three vertices
// Size: 48 bytes
typedef struct _FACE
{
	ZED_UINT64	Indices[ 3 ];
	ZED_FLOAT32 Normal[ 3 ];
	ZED_UINT64	MeshID;
	ZED_UINT32	MaterialID;
} FACE, *LPFACE;

// Materials hold information pertaining to the surface type (audio cues can be
// extended into using this at a later stage, hashing the material as a whole
// for comparison would save time)
typedef struct _MATERIAL
{
	// Hopefully, some material names will fill 32 chars
	ZED_UCHAR8	Name[ 32 ];
	// Textures used by the material (it may be a better idea to provide
	// generic textures, i.e. TextureMaps[ 8 ][ 32 ]
	ZED_UCHAR8	AlbedoMap[ 32 ];
	ZED_UCHAR8	SpecularMap[ 32 ];
	ZED_UCHAR8	NormalMap[ 32 ];
} MATERIAL, *LPMATERIAL;

// A mesh really is just a name, a material, and a bunch of faces
// This type is pretty bad, as the actual face indicies are stored after
// the main chunk is written
typedef struct _MESH
{
	ZED_UCHAR8	Name[ 32 ];
	ZED_UINT64	NumFaces;
	ZED_UINT32	MaterialID;
} MESH, *LPMESH;

// For the world type, extract the position and orientation of the buildings
// along with their names
typedef struct _LOCATION
{
	// Hope that no location is more than 32 chars!
	ZED_UCHAR8	pGeoType[ 32 ];
	ZED_FLOAT32	Position[ 2 ];
	ZED_FLOAT32	Rotation;
} LOCATION, *LPLOCATION;

#pragma pack( )

// Helper functions
void ZeroFileHeader( LPFILEHEADER p_pFileHeader );
void ZeroMaterial( LPMATERIAL p_pMaterial );
void ZeroMesh( LPMESH p_pMesh );
void ZeroLocation( LPLOCATION p_pLocation );

#endif
