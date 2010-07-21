//-------------------------------------------------------------------------------
/// 
///	\file	Mesh.h
/// \author	Rob Bateman
/// \brief	This file defines the single function OutputMesh which will write the mesh
///			data to the file. 
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
#pragma once
#endif

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

	/// the includes
	/// 
	#include <maya/MObject.h>

	/// this function outputs the specified mesh list
	/// 
	extern void OutputMesh(std::ofstream&,const MObject&);


#endif

