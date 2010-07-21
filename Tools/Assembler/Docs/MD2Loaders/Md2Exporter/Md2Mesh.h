// Use this at your own risk. For non-commercial use only. (c) Rob Bateman 2005
// cant be bothered to comment. exporters do my nut.
// http://www.robthebloke.org
// 
#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif

#ifndef __MD2__MESH__H__
#define __MD2__MESH__H__
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFloatArray.h>
#include <maya/MPointArray.h>
#include <maya/MStringArray.h>


extern unsigned int g_FrameStep;

#pragma pack(push,1)

//------------------------------------------------------------------------------
struct Md2TexCoord {
	union {
		struct {
			short u;
			short v;
		};
		short data[2];
	};
	void Assign(float fu,float fv,unsigned int iw,unsigned int ih) {
		u = fu * iw;
		v = fv * ih;
	}
	Md2TexCoord(const Md2TexCoord& tri) {
		memcpy(this,&tri,sizeof(Md2TexCoord));
	}
	Md2TexCoord() : u(0),v(0) {}
	Md2TexCoord(float fu,float fv,unsigned int iw,unsigned int ih) {
		Assign(fu,fv,iw,ih);
	}
};

//------------------------------------------------------------------------------
struct Md2Tri {
	Md2Tri() {};
	Md2Tri(const Md2Tri& tri) {
		memcpy(this,&tri,sizeof(Md2Tri));
	}
	unsigned short v[3];
	unsigned short t[3];
};

//------------------------------------------------------------------------------
struct Md2Mesh {
	std::vector<Md2TexCoord> m_Uvs;
	std::vector<Md2Tri> m_Faces;
	unsigned int m_NumVerts;
	Md2Mesh() : m_NumVerts(0),m_Uvs(), m_Faces() {}
	void AddMesh(MFnMesh& mesh,unsigned int iw,unsigned int ih);
	bool Build(const MStringArray& mlist);
	void Write(FILE* fp) const ;
};

//------------------------------------------------------------------------------
struct Md2Vertex {
	Md2Vertex() {}
	Md2Vertex(const Md2Vertex& v) {
		vertex[0] = v.vertex[0];
		vertex[1] = v.vertex[1];
		vertex[2] = v.vertex[2];
	}
	unsigned char vertex[3];
};

//------------------------------------------------------------------------------
struct Md2Frame {
	float scale[3];
	float translate[3];
	std::vector<Md2Vertex> m_Points;
	void Assign(MPointArray& points);
};

//------------------------------------------------------------------------------
struct Md2Anim  {
	std::string name;
	std::vector<Md2Frame*> m_Frames;
	Md2Anim();
	~Md2Anim();
	void WriteAnim(FILE* fp) const ;
	void AddFrame(MPointArray& points) ;
};

//------------------------------------------------------------------------------
struct Md2AnimList {
	std::vector<Md2Anim*> m_Anims;
	bool AddAnim(const char* name,int start,int end,MStringArray& meshNames);


	unsigned int GetNumFrames() {
		unsigned int sz = 0;
		std::vector<Md2Anim*>::iterator it = m_Anims.begin();
		for( ; it != m_Anims.end(); ++it )
		{
			sz += (*it)->m_Frames.size();
		}
		return sz;
	}

	void Write(FILE* fp) {
		std::vector<Md2Anim*>::iterator it = m_Anims.begin();
		for( ; it != m_Anims.end(); ++it )
		{
			(*it)->WriteAnim(fp);
		}
	}
};

#pragma pack(pop)

#endif
