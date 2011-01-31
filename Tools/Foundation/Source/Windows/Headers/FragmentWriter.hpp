// [DESC]	A series of functions that aide in converting Maya assets into
//			.ZED[A|C] [Animation|Character] data

#ifndef __FOUNDATION_FRAGMENTWRITER_HPP__
#define __FOUNDATION_FRAGMENTWRITER_HPP__

// Using the ZED functions
#include <ModelStructs.hpp>
#include <list>

namespace Foundation
{
	static FILEHEADER				g_LeadHeader;
	static std::list< VERTEX >		g_Vertices;
	static std::list< FACE >		g_Faces;
	static std::list< MESH >		g_Meshes;
	static std::list< MATERIAL >	g_Materials;

	// Read in the various data types for processing
	ZED_UINT32 ReadEverything( );
	ZED_UINT32 ReadVertices( );
	ZED_UINT32 ReadFaces( );
	ZED_UINT32 ReadMeshes( );
	ZED_UINT32 ReadMaterials( );

	// Dump the data to the destination file
	ZED_UINT32 DumpEverything( FILE *p_pFILE );
	ZED_UINT32 DumpHeader( FILE *p_pFILE, ZED_UCHAR8 p_Type );
	ZED_UINT32 DumpVertices( FILE *p_pFILE );
	ZED_UINT32 DumpFaces( FILE *p_pFILE );
	ZED_UINT32 DumpMeshes( FILE *p_pFILE );
	ZED_UINT32 DumpMaterials( FILE *p_pFILE );

	void WriteChunk( ZED_UINT16 p_ID, ZED_UINT64 p_Size, FILE *p_pFILE );
}

#endif
