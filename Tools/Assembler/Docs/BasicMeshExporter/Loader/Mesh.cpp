///-----------------------------------------------------------------------------
/// 
/// \file	Mesh.cpp
/// \author	Rob Bateman
/// \brief	a simple file loader for a simple file... This loader basically 
///			uses streaming operators to read and write the file. The data is 
///			stored in a set os basic classes, XVertex, XNormal, XTexCoord and
///			XFace.
/// 
///			The XMesh class then holds the data within arrays, these array types 
///			are typedefs of std::vector<XVertex> etc.
/// 
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

#include "Mesh.h"
#include <assert.h>

//-------------------------------------------------------------------------------	XFace


//-------------------------------------------------------------------------------
/// \brief	This function sets the number of indices within a face
/// \param	numPoints	-	the number of points within the face
/// 
void XFace::SetSize(unsigned int numPoints) 
{
	// ensure a minimum of 3 points
	if(numPoints<3)
		numPoints = 3;

	// create array of points
	XPointIndex* temp = new XPointIndex[numPoints];

	// copy over old data. Amount to copy depends on whether
	// array has shrunk or increased
	if(m_Points) {
		if (m_NumPoints<numPoints) {
			memcpy(temp,m_Points,sizeof(XVertex)*m_NumPoints);
		} else {
			memcpy(temp,m_Points,sizeof(XVertex)*numPoints);
		}

	}
	m_NumPoints = numPoints;

	// delete old data
	delete [] m_Points;

	// store reference to allocated memory
	m_Points = temp;
}

//-------------------------------------------------------------------------------
/// srteaming operator for the face
std::istream &operator>>(std::istream& ifs,XFace& f)	
{
	char buffer[10];
	int vcount;
	ifs >> buffer >> vcount;
	f.SetSize(vcount);

	XPointIndex *p = f.m_Points;
	XPointIndex *end = p + vcount;
	for(;p!=end;++p) 
	{
		ifs >> *p;
	}
	return ifs;
}

//-------------------------------------------------------------------------------	XMesh

//-------------------------------------------------------------------------------
/// \brief	this function draws all the faces in the mesh
///
void XMesh::gl() {
	
	XFaceIt it = m_Faces.begin();
	for( ; it != m_Faces.end() ; ++it ) 
	{
		assert(it->m_Points);

		XPointIndex *p = it->m_Points;
		XPointIndex *end = p + it->m_NumPoints;

		glBegin(GL_POLYGON);
		for(;p!=end;++p) 
		{
			m_TexCoords[p->t].gl();
			m_Normals  [p->n].gl();
			m_Vertices [p->v].gl();
		}
		glEnd();
	}
}

//-------------------------------------------------------------------------------
/// \brief	stream insertion operator for the mesh class
/// 
std::istream &operator>>(std::istream& ifs,XMesh& m)
{
	char buffer[256];
	int temp;

	// read mesh name
	ifs >> buffer;

	// read vertex count
	ifs >> buffer >> temp;
	m.m_Vertices.resize(temp);

	// read normal count
	ifs >> buffer >> temp;
	m.m_Normals.resize(temp);

	// read tex coord count
	ifs >> buffer >> temp;
	m.m_TexCoords.resize(temp);

	// read face count
	ifs >> buffer >> temp;
	m.m_Faces.resize(temp);

	// read vertices
	ifs >> buffer;
	for( XVertIt it = m.m_Vertices.begin(); it != m.m_Vertices.end() ; ++it ) 
		ifs >> *it;

	// read normals
	ifs >> buffer;
	for( XNormIt it = m.m_Normals.begin(); it != m.m_Normals.end() ; ++it ) 
		ifs >> *it;

	// read texture coords
	ifs >> buffer;
	for( XTexIt it = m.m_TexCoords.begin(); it != m.m_TexCoords.end() ; ++it ) 
		ifs >> *it;

	// read the faces
	for( XFaceIt it = m.m_Faces.begin(); it != m.m_Faces.end() ; ++it ) 
		ifs >> *it;

	return ifs;
}

//-------------------------------------------------------------------------------
/// \brief	This function loads the specified file and stores all of the objects
///			within the Meshes vector
/// \param	filename	-	the name of the file to load
/// \param	Meshes	-	the output arra of meshes
/// \return	true if OK
/// 
bool LoadMeshFile(const char* const filename,std::vector<XMesh*>& Meshes)
{
	char buffer[256] = {0};
	std::ifstream ifs(filename);

	if (ifs) {

		while (!ifs.eof()) 
		{
			ifs >> buffer;

			if (buffer[0] == 'M' &&
				buffer[1] == 'E' &&
				buffer[2] == 'S' &&
				buffer[3] == 'H') 
			{
				XMesh* pm = new XMesh;
				assert (pm);
				ifs >> *pm;
				Meshes.push_back(pm);
			}
			buffer[0] = '\0';
		}

	}
	return true;
}


//-------------------------------------------------------------------------------
/// \brief	This function draws the array of mesh pointers
/// \param	Meshes	-	a vector of mesh pointers
/// 
void DrawMeshes(std::vector<XMesh*>& Meshes)
{
	std::vector<XMesh*>::iterator it = Meshes.begin();
	for( ; it != Meshes.end(); ++it )
	{
		(*it)->gl();
	}
}







