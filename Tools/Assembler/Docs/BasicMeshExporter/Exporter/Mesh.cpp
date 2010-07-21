//-------------------------------------------------------------------------------
/// \file	Mesh.cpp
/// \author	Robert Bateman   ( robthebloke@hotmail.com )
/// \brief	When accessing any node data within maya, you must use a function set
///			and attach it to the relevant MObject within maya. All function sets
///			are listed with the prefix MFn. The class MFn also contains the type
///			enumerations for the various maya nodes, in the case of a mesh,
///
///									MFn::kMesh
/// \note
///
/// 		This sourcefile is provided as-is and no guarentee or warrenty
///			is provided for any damage that may arise from using it. This
///			code is free for your own use, the only thing i ask is a small
///			credit somewhere for my work. An e-mail saying you found it
///			useful would also be much appreciated by myself ;)
///
///			If you find any bugs arising to maya version changes or you
///			have improvements to the source code, then feel free to mail
///			them to me and i'll update the source-code on the website.
///			Before you e-mail bug reports, make sure that it is my code
///			causing the problem, and not some wierd oddity in the maya
///			scene. This code is designed to handle most common model set-ups,
///			it is not designed to replicate perfectly everything that maya
///			does.
///
///			DO NOT send me feature requests, i'm not likely to add to this
///			code myself unless something really new and groovy comes along.
///			This is mainly due to my current time restrictions. If you do
///			have code written that adds something fairly groovy to the
///			exporter, then i will happily update the exporter code and give
///			full credit where due.....
///
///			Robert Bateman
///
///				email code to:    rbateman@bmth.ac.uk
///				contact:          robthebloke@hotmail.com
///
//-------------------------------------------------------------------------------

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	// disable VC6 template name warning
	#pragma warning(disable:4786)
#endif



// includes
//
#include <maya/MFloatMatrix.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>

#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>

#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "Mesh.h"

MMatrix BuildTransform(MObject& obj)
{
	MFnTransform fn(obj);

	// get matrix transform for this bone
	MMatrix M = fn.transformationMatrix();

	if (fn.parentCount()>0) {
		MObject _obj =fn.parent(0);
		return M* BuildTransform(_obj) ;
	}
	return M;
}

//-------------------------------------------------------------------------------
/// \brief	This function outputs the specified mesh to the specified file stream
///
void OutputMesh(std::ofstream& ofs,const MObject &obj)
{
	// attach a function set to the mesh node. We access all of the meshes data
	// through the function set
	//
	MFnMesh fnMesh(obj);


	// Maya has a tendancy of storing additional nodes and objects all over the place.
	// We may come accross meshes that are nodes used purely as history or other
	// construction data. In our case, we want to ignore all 'intermediate objects'
	//
	if ( !fnMesh.isIntermediateObject() )
	{
		// loop through each instance of the mesh. Rather than do anything particularly
		// clever with instances, i'm just going to write them out as a second surface
		//
		for(int k=0;k<fnMesh.parentCount();++k)
		{
			int i,j;

			// write out some info about the mesh
			//
			ofs << "MESH "
				<< fnMesh.name().asChar()
				<< "\n\tvertex_count "
				<< fnMesh.numVertices()
				<< "\n\tnormal_count "
				<< fnMesh.numNormals()
				<< "\n\ttex_coord_count "
				<< fnMesh.numUVs()
				<< "\n\tface_count "
				<< fnMesh.numPolygons()
				<< std::endl;

			// this will hold the returned vertex positions
			MPointArray vertices;

			// use the function set to get the points
			fnMesh.getPoints(vertices);

			// Here we just grab the transform matrix
			//
			MObject _obj =fnMesh.parent(k);
			MMatrix mat = BuildTransform(_obj);


			// go through each vertex in the array and write it to the file
			//
			ofs << "\n\tvertices\n";
			for(i=0;i!=vertices.length() ; ++i)
			{
				// transform the points from local space to world space
				//
				MPoint OutPoint = vertices[i] * mat;

				// output
				//
				ofs << "\t\t" << OutPoint.x << " " << OutPoint.y << " " << OutPoint.z << "\n";
			}



			// this will store the normals
			MFloatVectorArray normals;

			// get the normals
			fnMesh.getNormals(normals);

			// just to be an utter arse, normals require you to have float matrices.
			// Unfortunately someone decided not to allow you to convert from the
			// MMatrix class to the MFloatMatrix class, therefore we have to do it
			// in a very very gay way...
			//
			MFloatMatrix fmat;

			for(i=0;i!=4;++i)
			{
				for(j=0;j!=4;++j)
				{
					fmat.matrix[i][j] = static_cast<float>(mat.matrix[i][j]);
				}
			}

			// if you ever need to transform a Normal by a matrix, you need to multiply the normal
			// by the inverse transpose of the transform matrix. This just inverts and transposes
			// it so we can transform our normals nicely into world space.
			fmat = fmat.inverse();
			fmat = fmat.transpose();

			// go through each normal in the array and write it to the file
			//
			ofs << "\n\tnormals\n";
			for(i=0;i!=normals.length() ; ++i)
			{

				// transform the normal into world space
				//
				MFloatVector OutNormal = normals[i] ;//* fmat;

				// we must normalise the vector to counter any scaling in the transformation
				//
				OutNormal.normalize();

				// output the normal
				//
				ofs << "\t\t" << OutNormal.x << " " << OutNormal.y << " " << OutNormal.z << "\n";
			}

			// these two arrays will hold the u & v texture co-ordinates
			//
			MFloatArray u_coords;
			MFloatArray v_coords;

			// get the uv coordinates
			//
			ofs << "\n\ttex_coords\n";
			fnMesh.getUVs(u_coords,v_coords);

			// go through each uv in the array and write it to the file
			//
			for(i=0;i!=u_coords.length() ; ++i)
			{
				ofs << "\t\t" << u_coords[i] << " " << v_coords[i] << "\n";
			}


			// to get at the face data, we need to use a mesh polygon iterator.
			//
			MItMeshPolygon itPoly(fnMesh.object());

			// keep looping until the iterator has finished
			//
			while ( !itPoly.isDone() )
			{

				// get the number of vertices in this face
				unsigned int vcount = itPoly.polygonVertexCount();

				// output some info about the face
				//
				ofs << "\n\tFACE "
					<< vcount
					<< std::endl;

				// for each vertex on the face....
				//
				for(i = 0; i!= vcount; ++i)
				{
					// have to get the uv index seperately
					int uv_index;
					itPoly.getUVIndex(i,uv_index);

					// output indices for this vertex
					ofs << "\t\t" << itPoly.vertexIndex(i) << " " << itPoly.normalIndex(i) << " " << uv_index << "\n";
				}

				// move to next poly in the mesh
				//
				itPoly.next();
			}
		}
	}
}
