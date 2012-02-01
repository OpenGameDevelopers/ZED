#ifndef __ZEDMODELSTRUCTS_HPP__
#define __ZEDMODELSTRUCTS_HPP__

#include <DataTypes.hpp>

// Pack the structs into single bytes
#pragma pack( 1 )

namespace ZED
{
	namespace Renderer
	{
		// The header goes before each type, defining what is to be processed
		// N.B.
		//	The chunk structure will not change, so there is no need to version
		//	it
		typedef struct _CHUNK
		{
			ZED_UINT16	ID;
			ZED_UINT64	Size;
		} CHUNK, *LPCHUNK;

		///////////////////////////////////////////////////////////////////////
		//                                                            VERSION 1
		///////////////////////////////////////////////////////////////////////

		// The header for the whole file, contains everything to describe a ZED
		// asset
		typedef struct _FILEHEADER_V1
		{
			// The asset's name (of course)
			ZED_UCHAR8	Name[ 32 ];
			// File version [Major.Minor.Revision]
			ZED_UINT32	Version[ 3 ];
			// Vertex count
			ZED_UINT32	Vertices;
			// Index count
			ZED_UINT32	Indices;
			// Face count
			ZED_UINT32	Faces;
			// Amount of Shaders to use
			ZED_UINT32	Shaders;
			// How many meshes compose this model?
			ZED_UINT32	Meshes;
			// Type of file being written [ZED]
			ZED_UCHAR8	ID[ 3 ];
			// The type of file to process [A|C|G|W]
			ZED_UCHAR8	Type;
			// Endianess [E|e]
			ZED_UCHAR8	Endianess;
		} FILEHEADER_V1, *LPFILEHEADER_V1;

		// Vertices define the position, normal and UV coords of a point in
		// space
		typedef struct _VERTEX_V1
		{
			ZED_FLOAT32	Position[ 3 ];
			ZED_FLOAT32	Normal[ 3 ];
			ZED_FLOAT32	UV[ 2 ];
		} VERTEX_V1, *LPVERTEX_V1;
		// Faces are used to describe planar surfaces consisting of three
		// vertices
		// Size: 48 bytes
		typedef struct _FACE_V1
		{
			// If the amount of indices goes above 65,536, optimise the mesh
			// better =P
			//ZED_UINT16	UVIndex[ 3 ];
			ZED_FLOAT32 Normal[ 3 ];
			ZED_UINT16	Indices[ 3 ];
		} FACE_V1, *LPFACE_V1;

		// Materials hold information pertaining to the surface type (audio
		// cues can be extended into using this at a later stage, hashing the
		// material as a whole for comparison would save time)
		// N.B.
		//	As there are no materials supported in Ver. 1, this is just a
		//	placeholder
		typedef struct _MATERIAL_V1
		{
			// Hopefully, some material names will fill 32 chars
			ZED_UCHAR8	Name[ 32 ];
			// Textures used by the material (it may be a better idea to
			// provide generic textures, i.e. TextureMaps[ 8 ][ 32 ]
			ZED_UCHAR8	AlbedoMap[ 32 ];
			ZED_UCHAR8	SpecularMap[ 32 ];
			ZED_UCHAR8	NormalMap[ 32 ];
		} MATERIAL_V1, *LPMATERIAL_V1;

		// A mesh really is just a name, a material, and a bunch of faces
		// This type is pretty bad, as the actual face indicies are stored
		// after the main chunk is written.  Though it could be a lot worse.
		typedef struct _MESH_V1
		{
			ZED_UCHAR8	Name[ 32 ];
			ZED_UINT16	NumFaces;
		} MESH_V1, *LPMESH_V1;

		// For the world type, extract the position and orientation of the
		// buildings along with their names
		typedef struct _LOCATION_V1
		{
			// Hope that no location is more than 32 chars!
			ZED_UCHAR8	pGeoType[ 32 ];
			ZED_FLOAT32	Position[ 2 ];
			ZED_FLOAT32	Rotation;
		} LOCATION_V1, *LPLOCATION_V1;

		///////////////////////////////////////////////////////////////////////
		//                                                            VERSION 2
		///////////////////////////////////////////////////////////////////////
		// Size: 11 bytes
		typedef struct __FILEHEADER_V2
		{
			// Should be 'Z' 'E' 'D'
			ZED_UCHAR8	ID[ 3 ];
			// Flags are described in the Renderer document
			ZED_UINT32	Flags;
			// Should be 2.x.x
			ZED_UINT16	Version[ 3 ];
			// Mesh and joint count
			ZED_BYTE	MeshCount;
			ZED_BYTE	JointCount;
		}FILEHEADER_V2, *LPFILEHEADER_V2;

		// Meshes are now self-contained
		// Size (64-bit): 33 bytes
		// Size (32-bit): 25 bytes
		typedef struct __MESH_V2
		{
			// Per-vertex attributes
			ZED_UINT64	Attributes;
			// The material to use
			ZED_UINT32	MaterialID;
			// Vertex and Index count
			ZED_UINT16	VertexCount;
			ZED_UINT16	IndexCount;
			// Bytes between vertices
			ZED_BYTE	Stride;
		}MESH_V2, *LPMESH_V2;

		// JOINT_V2 is not the joint changing in time, JOINTT_V2 is for
		// transforming joints.
		// Joints are stored as an orientation and position, which is the initial
		// transform.  All joints have a parent, except for the root joint.
		// Size: 29 bytes
		typedef struct	__JOINT_V2
		{
			ZED_FLOAT32	Orientation[ 4 ];
			ZED_FLOAT32	Position[ 3 ];
			ZED_BYTE	Parent;
		}JOINT_V2, *LPJOINT_V2;

		typedef struct __JOINTT_V2
		{
			ZED_FLOAT32	Rotation[ 4 ];
		}JOINTT_V2, *LPJOINTT_V2;

		// Skeletons are just the joint count plus the joints themselves
		typedef struct __SKELETON_V2
		{
			LPJOINT_V2	pJoints;
			ZED_BYTE	JointCount;
		}SKELETON_V2, *LPSKELETON_V2;

		// Skeleton Transforms can transform specific joints rather than
		// redundantly transforming already in-transition joints
		typedef struct __SKELETONT_V2
		{
			LPSKELETON_V2	pSkeleton;
			LPJOINTT_V2		pJointTransforms;
		}SKELETONT_V2, *LPSKELETONT_V2;
	}
}

#pragma pack( )

/**
	\brief Helper function
*/
void zedZeroFileHeaderV1( ZED::Renderer::LPFILEHEADER_V1 p_pFileHeader );
/**
	\brief Helper function
*/
void zedZeroMaterialV1( ZED::Renderer::LPMATERIAL_V1 p_pMaterial );
/**
	\brief Helper function
*/
void zedZeroMeshV1( ZED::Renderer::LPMESH_V1 p_pMesh );
/**
	\brief Helper function
*/
void zedZeroLocationV1( ZED::Renderer::LPLOCATION_V1 p_pLocation );

/**
	\brief Helper function
*/
void zedZeroFileHeaderV2( ZED::Renderer::LPFILEHEADER_V2 p_pFileHeader );
/**
	\brief Helper function
*/
void zedZeroMeshV2( ZED::Renderer::LPMESH_V2 p_pMesh );
/**
	\brief Helper function
*/
void zedZeroJointV2( ZED::Renderer::LPJOINT_V2 p_pJoint );
/**
	\brief Helper function
*/
void zedZeroJointTV2( ZED::Renderer::LPJOINTT_V2 p_pJointT );
/**
	\brief Helper function
*/
void zedZeroSkeletonV2( ZED::Renderer::LPSKELETON_V2 p_pSkeleton );
/**
	\brief Helper function
*/
void zedZeroSkeletonTV2( ZED::Renderer::LPSKELETONT_V2 p_pSkeletonT );

#endif
