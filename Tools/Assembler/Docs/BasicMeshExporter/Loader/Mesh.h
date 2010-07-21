///-----------------------------------------------------------------------------
/// 
/// \file	Mesh.h
/// \author	Rob Bateman
/// \brief	a simple file loader for a simple file... This loader basically 
///			uses streaming operators to read and write the file. The data is 
///			stored in a set os basic classes, XVertex, XNormal, XTexCoord and
///			XFace.
/// 
///			The XMesh class then holds the data within arrays, these array types 
///			are typedefs of std::vector<XVertex> etc.
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

#ifndef XMESH__H_
#define XMESH__H_


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <GL/gl.h>

#include <vector>
#include <fstream>

//-------------------------------------------------------------------------------
///	\brief	
///
class XVector
{
	friend std::istream& operator >> (std::istream& ifs,XVector& v) {
		return ifs >> v.x >> v.y >> v.z;
	}
public:
	
	/// ctor
	XVector() :x(0),y(0),z(0) {}
	
	/// dtor
	~XVector() {};

	union {
		struct {
			float x;
			float y;
			float z;
		};
        float data[3];
	};

};

//-------------------------------------------------------------------------------
/// A class to hold a vertex
/// 
class XVertex: public XVector
{
public:
	
	/// whoa! a constructor that doesn't initialise anything?
	/// Whats the point? It will only serve to initialise all
	/// values to 0 which I will write over anyway when reading in
	/// the file.
	/// 
	XVertex(){}

	/// dtor
	~XVertex(){}

	/// a function to pass the data to opengl
	inline void gl() const {
		glVertex3f( x,y,z );
	}
};

//-------------------------------------------------------------------------------

// typedefs for readabilities sake
typedef std::vector<XVertex> XVertList;
typedef XVertList::iterator XVertIt;


//-------------------------------------------------------------------------------
/// a class to hold a normal
/// 
class XNormal: public XVector
{
public:

	/// ctor
	XNormal(){}

	/// dtor
	~XNormal(){}
			
	inline void gl() const {
		glNormal3f( x,y,z );
	}
};
//-------------------------------------------------------------------------------

// typedefs for readabilities sake
typedef std::vector<XNormal> XNormList;
typedef XNormList::iterator XNormIt;

//-------------------------------------------------------------------------------
/// a class to hold a uv texture coord
/// 
class XTexCoord
{
	friend std::istream &operator>>(std::istream& ifs,XTexCoord& xt) {
		return ifs >> xt.u >> xt.v;
	}

public:

	/// ctor
	XTexCoord(){}

	/// dtor
	~XTexCoord(){}

	/// copy ctor
	XTexCoord(const XTexCoord& v_) : u(v_.u),v(v_.v) {}

	inline void gl() const {
		glTexCoord2f( u,v );
	}

	/// the tex coord data
	float u;
	float v;
};

//-------------------------------------------------------------------------------

// typedefs for readabilities sake
typedef std::vector<XTexCoord> XTexList;
typedef XTexList::iterator XTexIt;

/// this internal structure is used to package all indices
/// into 12bytes. This keeps the data access sequentual through
/// memory in the draw function
///
struct XPointIndex 
{
	/// streaming operator
	friend std::istream &operator>>(std::istream& ifs,XPointIndex& pi) {
		return ifs >> pi.v >> pi.n >> pi.t;
	}

	/// i prefer not to initialise array indices because it often 
	/// highlights problems with the code that would be otherwise
	/// undetected. ie, the below indices are probably going to crash
	/// the program if they are not initialised. If I set these to 
	/// zero then I would never be aware of this problem....
	/// 
	XPointIndex() {}

	/// the texture coordinate index
	unsigned int t;

	/// the normal index
	unsigned int n;

	/// the vertex index
	unsigned int v;
};


//-------------------------------------------------------------------------------
/// a class to hold a face
///
class XFace
{
	/// stream operator
	friend std::istream &operator>>(std::istream&,XFace&);

	/// the mesh class is a friend...
	friend class XMesh;


public:


	/// ctor - sets the pointer to NULL for that is good...
	XFace() : m_Points(NULL) {}

	/// dtor
	~XFace() {
		delete m_Points;
	}

	/// this function sets the number of indices in the face
	/// 
	void SetSize(unsigned int);

protected:

	/// the number of points in the polygon
	unsigned int m_NumPoints;

	/// an array of point indices
	XPointIndex *m_Points;
};

//-------------------------------------------------------------------------------

// typedefs for readabilities sake
typedef std::vector<XFace> XFaceList;
typedef XFaceList::iterator XFaceIt;



//-------------------------------------------------------------------------------

class XMesh
{
	friend std::istream &operator>>(std::istream&,XMesh&);

public:

	/// ctor
	XMesh() {};

	/// dtor
	~XMesh() {};

	/// this function draws the mesh data
	void gl();

protected:

	/// an array of faces
	XFaceList m_Faces;

	/// an array of texture coords
	XTexList  m_TexCoords;
	
	/// an array of normals
	XNormList m_Normals;

	/// an array of vertices
	XVertList m_Vertices;

	/// the mesh name 
	std::string m_Name;

};


extern void DrawMeshes(std::vector<XMesh*>& Meshes);
extern bool LoadMeshFile(const char* const filename,std::vector<XMesh*>& Meshes);

#endif
