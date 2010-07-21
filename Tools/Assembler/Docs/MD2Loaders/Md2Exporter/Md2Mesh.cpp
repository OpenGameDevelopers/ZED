// Use this at your own risk. For non-commercial use only. (c) Rob Bateman 2005
//
#include "Md2Mesh.h"
#include "Md2Texture.h"
#include <maya/MFnTransform.h>
#include <maya/MMatrix.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
unsigned int g_FrameStep=1;

Md2Anim::Md2Anim()
: name(), 
m_Frames()
{}

Md2Anim::~Md2Anim() {
	std::vector<Md2Frame*>::iterator it = m_Frames.begin();
	for( ; it != m_Frames.end(); ++it ) {
		delete *it;
	}
}

void Md2Anim::WriteAnim(FILE* fp) const {
	std::vector<Md2Frame*>::const_iterator it = m_Frames.begin();
	for( unsigned int i=0; it != m_Frames.end(); ++it, ++i ) {
		fwrite((*it)->scale,3,sizeof(float),fp);
		fwrite((*it)->translate,3,sizeof(float),fp);

		char temp[16] = {'\0'};
		if(m_Frames.size()>10) {
			sprintf(temp,"%s%2d",name.c_str(),i);
		} else {
			sprintf(temp,"%s%d",name.c_str(),i);
		}
		fwrite(temp,16,1,fp);

		std::vector<Md2Vertex>::iterator itp = (*it)->m_Points.begin();
		for( ; itp!=(*it)->m_Points.end(); ++itp ){
			unsigned char pad=0x00;
			fwrite(itp->vertex,3,1,fp);
			fwrite(&pad,1,1,fp);
		}
	}
}

void Md2Anim::AddFrame(MPointArray& points) {
	Md2Frame* ptr = new Md2Frame;
	ptr->Assign(points);
	m_Frames.push_back(ptr);
}

void Md2Frame::Assign(MPointArray& points) {
	float minp[3] = {999999.9f,999999.9f,999999.9f};
	float maxp[3] = {-999999.9f,-999999.9f,-999999.9f};

	for(unsigned int i=0;i!=points.length();++i) {
		if(minp[0]>points[i].x) minp[0] = points[i].x;
		if(minp[1]>points[i].y) minp[1] = points[i].y;
		if(minp[2]>points[i].z) minp[2] = points[i].z;

		if(maxp[0]<points[i].x) maxp[0] = points[i].x;
		if(maxp[1]<points[i].y) maxp[1] = points[i].y;
		if(maxp[2]<points[i].z) maxp[2] = points[i].z;
	}
	#define EP 0.1f
	minp[0]-=EP;
	minp[1]-=EP;
	minp[2]-=EP;

	maxp[0]+=EP;
	maxp[1]+=EP;
	maxp[2]+=EP;

	translate[0] = minp[0];
	translate[1] = minp[1];
	translate[2] = minp[2];

	scale[0] = (maxp[0]-minp[0])/255.0f;
	scale[1] = (maxp[1]-minp[1])/255.0f;
	scale[2] = (maxp[2]-minp[2])/255.0f;

	for(unsigned int i=0;i!=points.length();++i) {
		Md2Vertex v;
		v.vertex[0] = (points[i].x - translate[0])/scale[0];
		v.vertex[2] = (points[i].y - translate[1])/scale[1];
		v.vertex[1] = (points[i].z - translate[2])/scale[2];
		m_Points.push_back(v);
	}
	float t = translate[2];
	translate[2] = translate[1];
	translate[1] = t;

	t = scale[2];
	scale[2] = scale[1];
	scale[1] = t;
}

void Md2Mesh::AddMesh(MFnMesh& mesh,unsigned int iw,unsigned int ih) {
	MFloatArray u;
	MFloatArray v;
	int uvStart=m_Uvs.size();
	mesh.getUVs(u,v);
	for(unsigned int i=0;i<u.length();++i) {
		m_Uvs.push_back( Md2TexCoord(u[i],v[i],iw,ih) );
	}
	MItMeshPolygon it(mesh.object());
	for( ;!it.isDone();it.next()) {
		unsigned int nt = it.polygonVertexCount()-2;
		int tindex=0;
		Md2Tri tri;
		tri.v[0] = m_NumVerts + it.vertexIndex(0);
		it.getUVIndex(0,tindex);
		tri.t[0] = uvStart+tindex;
		for(unsigned int i=0;i<nt;++i) {
			tri.v[1] = m_NumVerts + it.vertexIndex(i+1);
			tri.v[2] = m_NumVerts + it.vertexIndex(i+2);
			it.getUVIndex(i+1,tindex);
			tri.t[1] = uvStart+tindex;
			it.getUVIndex(i+2,tindex);
			tri.t[2] = uvStart+tindex;
			m_Faces.push_back(tri);
		}
	}
	m_NumVerts += mesh.numVertices();
}

void Md2Mesh::Write(FILE* fp) const {
	
	std::vector<Md2TexCoord>::const_iterator itv = m_Uvs.begin();
	for( ; itv != m_Uvs.end(); ++itv ) {
		fwrite( itv->data,sizeof(short),2,fp);
	}
	std::cout << "offsetFaces " << ftell(fp) << std::endl;
	std::vector<Md2Tri>::const_iterator itf = m_Faces.begin();
	for( ; itf != m_Faces.end(); ++itf ) {
		fwrite( itf->v,sizeof(short),3,fp);
		fwrite( itf->t,sizeof(short),3,fp);
	}
}

MMatrix BuildTransform(MObject obj)
{
	MFnTransform fn(obj);
	MMatrix M = fn.transformationMatrix();
	if (fn.parentCount()>0) {
		MObject _obj = fn.parent(0);
		return M* BuildTransform(_obj) ;
	}
	return M;
}

bool GetMesh(MFnMesh& fn,MString name) {
	MSelectionList sl;
	MGlobal::getSelectionListByName(name,sl);
	if(sl.length()==0)
		return false;
	MObject obj;
	sl.getDependNode(0,obj);
	if(obj.hasFn(MFn::kMesh)) {
		fn.setObject(obj);
		return true;
	}
	else
	if(obj.hasFn(MFn::kTransform)){
		MFnTransform fnT(obj);
		for(int i=0;i<fnT.childCount();++i) {
			if(fnT.child(i).hasFn(MFn::kMesh)) {
				fn.setObject(fnT.child(i));
				return true;
			}
		}
	}
	return false;
}
bool Md2Mesh::Build(const MStringArray& mlist) {
	for(int i=0;i!=mlist.length();++i) {
		MFnMesh fn;
		if(!GetMesh(fn,mlist[i])) {
			std::cerr << "Unable to find \"" << mlist[i].asChar() << "\" when building initial mesh data for md2.\n"
					  << "[Aborting] :(\n";
			return false;
		}
		AddMesh(fn,g_SkinWidth,g_SkinHeight);

		if(m_NumVerts>2048)
		{
			std::cerr << "Unable to add mesh to output data - \"" << mlist[i].asChar() << "\" [Aborting] :(\n";
			return false;
		}
	}
	return true;
}

bool Md2AnimList::AddAnim(const char* name,int start,int end,MStringArray& meshNames)
{
	Md2Anim* panim = new Md2Anim();
	panim->name = name;

	
	// loop through cycle
	for( ; start < end; start += g_FrameStep ) {

		MGlobal::viewFrame(start);
		
		MPointArray finalpoints;

		// get world space points for all meshes 
		for(int i=0;i!=meshNames.length();++i) {
			MFnMesh fn;
			if(!GetMesh(fn,meshNames[i])) {
				std::cerr << "Could not find mesh " << meshNames[i] << std::endl;
				delete panim;
				return false;
			}

			MPointArray origpoints;
			fn.getPoints(origpoints);

			for(int j=0;j<fn.parentCount();j++) {
				// get xform for parent
				MMatrix mat = BuildTransform(fn.parent(j));

				// transform points
				for(int k=0;k<origpoints.length();++k)
				{
					finalpoints.append( origpoints[k] * mat );
				}
			}
		}

		if(finalpoints.length()>2048) {
			std::cerr << "Max allowable vertex count exceeded. Unable to write Md2\n";
				delete panim;
			return false;
		}

		panim->AddFrame(finalpoints);
	}

	if(m_Anims.size()>0) {
		if(panim->m_Frames.size()>0) {
			if( panim->m_Frames[0]->m_Points.size() != m_Anims[0]->m_Frames[0]->m_Points.size() )
			{
				std::cerr << "The vertex counts of the anim file does not match the first \"" << name << "\"\n";
				delete panim;
				return false;
			}
		}
	}

	m_Anims.push_back(panim);

	return true;
}
