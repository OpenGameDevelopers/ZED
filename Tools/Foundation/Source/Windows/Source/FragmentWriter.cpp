#include <FragmentWriter.hpp>
#include <cstdio>
#include <curses.h>
#include <maya/MLibrary.h>
#include <maya/MFileIO.h>
#include <maya/MItDag.h>
#include <maya/MPlug.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MItMeshPolygon.h>

namespace Foundation
{
	ZED_UINT32 ReadEverything( )
	{
		ZED_UINT32 Status = ZED_FAIL;

		// Get the vertices
		Status = ReadVertices( );
		if( Status != ZED_OK )
		{
			return Status;
		}

		return Status;
	}

	ZED_UINT32 ReadVertices( )
	{
		// Assuming Maya has been initialised, extract the vertices from this
		// model, unless it's animation data
		MStatus Status;
		MItDag DAGItr( MItDag::kBreadthFirst, MFn::kInvalid, &Status );

		for( ; !DAGItr.isDone( ); DAGItr.next( ) )
		{
			MDagPath DAGPath;
			Status = DAGItr.getPath( DAGPath );

			if( Status )
			{
				MFnDagNode DAGNode( DAGPath );

				// Filter out intermediate and transform objects
				// as well as non-mesh objects
				if( DAGNode.isIntermediateObject( ) )
				{
					continue;
				}
				if( DAGPath.hasFn( MFn::kTransform ) )
				{
					continue;
				}
				if( !DAGPath.hasFn( MFn::kMesh ) )
				{
					continue;
				}

				MFnMesh Mesh( DAGPath );
				printw( "Mesh %s\n", Mesh.name( ).asChar( ) );
				VERTEX TmpVertex;

				// Extract the vertex positions, normals and UVs
				MPointArray			Verts;
				MFloatVectorArray	Norms;
				MFloatArray			Us, Vs;

				Mesh.getPoints( Verts, MSpace::kWorld );
				Mesh.getNormals( Norms );
				Mesh.getUVs( Us, Vs );

				// Write out the list of vertices
				for( ZED_UINT32 i = 0; i < Verts.length( ); i++ )
				{
					// Make sure we're in a comfortable space
					Verts[ i ].cartesianize( );
					MPoint Point = Verts[ i ];

					TmpVertex.Position[ 0 ] = Point.x;
					TmpVertex.Position[ 1 ] = Point.y;
					TmpVertex.Position[ 2 ] = Point.z;

					TmpVertex.Normal[ 0 ] = Norms[ i ].x;
					TmpVertex.Normal[ 1 ] = Norms[ i ].x;
					TmpVertex.Normal[ 2 ] = Norms[ i ].z;

					TmpVertex.UV[ 0 ] = Us[ i ];
					TmpVertex.UV[ 1 ] = Vs[ i ];

					// Finally, add the TmpVertex to the list of vertices
					g_Vertices.push_back( TmpVertex );
				}
			}
		}

		return ZED_OK;
	}
}
