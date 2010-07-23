#ifndef __ZEDTOOLASSEMBLER_MODEL_TYPES_HPP__
#define __ZEDTOOLASSEMBLER_MODEL_TYPES_HPP__

#include <DataTypes.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>

typedef struct _HEADER
{
	// Model's name
	ZED_UCHAR8 Name[ 40 ];
	// Author
	ZED_UCHAR8 Creator[ 32 ];
	// Amount of vertices present
	ZED_UINT64 VertexCount;
	// Amount of Indices
	ZED_UINT64 IndexCount;
	// Faces to construct the model
	ZED_UINT64 FaceCount;
	// Materials to be assigned
	ZED_UINT32 MaterialCount;
	// Verify this is a valid file
	ZED_UINT16 ID;
	// Version [Major] [Minor]
	ZED_UCHAR8 Version[ 2 ];
} MODELHEADER, *LPMODELHEADER;

typedef struct _CHUNK
{
	ZED_UINT16	ID;
	ZED_UINT64	Size;
} CHUNK, *LPCHUNK;

typedef struct _VERTEX
{
	ZED::Arithmetic::Vector3 Position;
	ZED::Arithmetic::Vector3 Normal;
	ZED::Arithmetic::Vector2 UV;
} VERTEX, LPVERTEX;

typedef struct _PRIMITIVE
{
	// Indices to create the primitive (okay, triangle)
	ZED_UINT64 Indices[ 3 ];
	// Primitive normal (on surface)
	ZED::Arithmetic::Vector3 Normal;
	// Which material is assigned?
	ZED_UINT32 MaterialID;
} PRIMITIVE, *LPPRIMITIVE;

typedef struct _MATERIAL
{
	
} MATERIAL, *LPMATERIAL;

#endif
