//-------------------------------------------------------------------------------
/// \file	main.cpp
/// \author	Robert Bateman   ( robthebloke@hotmail.com )
/// \brief	
///	This example uses MLibrary::Initialise to start maya in our own console application. 
///	
///	The first command line arg is a name of a maya file. We can load this file and then
///	specifically read back information we want from the file and write it to our own file.
///	
///	To make life simple, this exporter merely traverses through all meshes in the scene 
///	and outputs them to the file (in world space coords)
///
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


// include each header for each class you need. Gets a bit laborious later on...
//
#include <maya/MFloatMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MLibrary.h>
#include <maya/MFnDagNode.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>
#include <maya/MFileIO.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>



// link to some fairly funky maya libs. NOTE: The Image library only became part of maya
// from version 4.01 onwards. It is not available in version 4.0
//
#ifdef WIN32
	#pragma comment(lib,"Foundation.lib")
	#pragma comment(lib,"OpenMaya.lib")
	#pragma comment(lib,"Image.lib")
#endif

#include "Mesh.h"
#include "FileDialog.h"


//-------------------------------------------------------------------------------
#ifdef WIN32
int main(int argc,char** argv)
{
#else // hack for linux & gtk

int   g_argc=0;
char** g_argv=0;

int main(int argc,char** argv)
{
	g_argc=argc;
	g_argv=argv;
#endif

	// var to hold the input file name
	std::string infilename;

	// var to hold the output file name
	std::string outfilename;

	if (argc == 3 )
	{
		infilename  = argv[1];
		outfilename = argv[2];
	}
	else
	// if only one argument was specified, I'll assume that it is a maya binary
	// or ascii file, in which case use remove the 'mb' or 'ma' and add a 'mod' extension
	// to the end of the filename 
	// 
	if (argc == 2 )
	{
		infilename  = argv[1];
		char buffer[512]={0};
		if(!SaveFileDialog(buffer)) {
			return 0;
		}
		outfilename = buffer;
	}
	else
	if (argc == 1 )
	{
		char buffer[512]={0};
		if(!OpenFileDialog(buffer)) {
			return 0;
		}
		
		infilename = buffer;
		
		if(!SaveFileDialog(buffer)) {
			return 0;
		}
		
		// if outfile file was specified, use that
		//
		outfilename = buffer;
	}
	else {
		return 0;
	}



	// initialise the maya library - This basically starts up Maya
	// 
	MLibrary::initialize(argv[0]);



	// open the file that the user specified by using the static methods contained
	// within the MFileIO class. 
	//
	if( MS::kSuccess == MFileIO::open( infilename.c_str(),	// file to load
									   NULL,	// allows you to specify the file type, ie "ma", "mb" etc (i think!)
									   true) )	// if false will not allow unsaved files to be deleted, but we don't care so always force it to open
	{

		// create an output file stream
		//
		std::ofstream ofs(outfilename.c_str());

		// make sure it opened OK
		//
		if (!ofs) {
			std::cerr << "Could not open output file " << outfilename.c_str() << std::endl;

			// cleanup the Maya library data before exiting
			MLibrary::cleanup();
			return 1;
		}




		// iterate through the mesh nodes in the scene
		//
		MItDependencyNodes itDep(MFn::kMesh);

		// we have to keep iterating until we get through all of the nodes in the scene
		//
		while (!itDep.isDone()) 
		{
			// Write the mesh data using the function defined in Mesh.cpp
			OutputMesh(ofs,itDep.item());

			// write a new line for the hell of it ;)
			ofs << std::endl;
			
			itDep.next();
		}
		
		// BUG_FIX:
		// for some reason the loader code crashes on linux when the eof char gets
		// read with the stdc++ libs? This is probably the hacky linux build we're 
		// using. To get around the problem I've added a newline to the end of the file
		ofs << "\n";

		// close the output file
		ofs.close();
	}
	else
	{
		std::cerr << "Could not open file " << infilename.c_str() << std::endl;
	}


	// cleanup maya library before exiting

	MLibrary::cleanup();

	return 0;
}
