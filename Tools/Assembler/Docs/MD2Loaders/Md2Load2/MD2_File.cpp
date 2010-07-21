#include "MD2_File.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>

MD2::RefList MD2::m_Refs;

MD2::RefIter MD2::FindMd2(const char* name) {
	for(MD2::RefIter i=m_Refs.begin();i!=MD2::m_Refs.end();++i) {
		if( i->name == name ) {
			return i;
		}
	}
	return m_Refs.end();
}

MD2::RefIter MD2::FindMd2(const MD2* ptr) {
	for(MD2::RefIter i=m_Refs.begin();i!=m_Refs.end();++i) {
		if( i->ptr == ptr ) {
			return i;
		}
	}
	return m_Refs.end();
}

unsigned int MD2::GetDataSize() {
	unsigned int sz=0;
	for(MD2::RefIter i=m_Refs.begin();i!=m_Refs.end();++i) {
		sz += i->ptr->DataSize();
	}
	return sz;
}

MD2* MD2::Load(const char* filename) {
	MD2::RefIter it = FindMd2(filename);
	if(it == m_Refs.end()) {
		MD2* ptr = new MD2;
		if(!ptr->InternalLoad(filename)) {
			delete ptr;
			return false;
		}
		m_Refs.push_back( MD2Ref(ptr,filename) );
		return ptr;
	}
	it->ref++;
	return it->ptr;
}

void MD2::Release(MD2* ptr) {
	MD2::RefIter it = FindMd2(ptr);
	if(it != m_Refs.end() ) {
		if(--it->ref == 0 ) {
			delete it->ptr;
			m_Refs.erase(it);
		}
	}
}

MD2::MD2()
	: m_data(0),data_size(0) 
{}

MD2::~MD2() {
	delete [] m_data;
	m_data=0;
	m_Anims.clear();
}

const MD2::model* MD2::GetModel() const {
	if(!m_data) return 0;
	void *p=m_data;
	return reinterpret_cast<model*>( p );
}

const MD2::frame* MD2::GetFrame(unsigned int num) const {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetFrames + (num*pm->frameSize);
	return reinterpret_cast<frame*>(ptr);
}

const MD2::triangle* MD2::GetTriangles() const {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetTriangles;
	return reinterpret_cast<triangle*>(ptr);
}

const MD2::uv* MD2::GetTexCoords() const {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetTexCoords;
	return reinterpret_cast<uv*>(ptr);
}

unsigned char* MD2::GetCommandsStart() {
	if(!m_data) return 0;
	const model* pm = GetModel();
	return m_data + pm->offsetGlCommands;
}

unsigned char* MD2::GetCommandsEnd() {
	if(!m_data) return 0;
	const model* pm = GetModel();
	return m_data + pm->offsetGlCommands + pm->numGlCommands*4;
}

const char*	MD2::GetSkin(unsigned int num) const {
	if(!m_data) return 0;
	void* p = m_data + GetModel()->offsetSkins + num*64;
	return reinterpret_cast<char*>(p);
}

// this loads an MD2 into a single buffer
bool MD2::InternalLoad(const char* filename) {
	FILE* fp = fopen(filename,"rb");
	if(!fp) {
		std::cerr << "[ERROR] \"" 
				  << filename 
				  << "\" could not be opened"
				  << std::cerr;
		return false;
	}
	fseek(fp,0,SEEK_END);
	data_size = ftell(fp);
	m_data = new unsigned char[data_size];
	assert(m_data);
#if 0
	fclose(fp);
	fp = fopen(filename,"rb");
#else
	rewind(fp);
#endif
	fread(m_data,sizeof(unsigned char),data_size,fp);
	fclose(fp);

	// ensure format is valid
	if( GetModel()->magic != 0x32504449 ||
		GetModel()->version != 8 ) {
			delete [] m_data;
			m_data  = 0;
			std::cerr << "[ERROR] \"" 
					  << filename 
					  << "\" does not contain 'IDP2' flag"
					  << std::cerr;
			return false;
	}

	
	// get number of frames
	unsigned int nf = GetModel()->numFrames;

	// quick name lookup for frame - num
	std::string firstName="";

	// used to build up references to animation cycles
	AnimRef ref;
	ref.m_FrameStart=0;
	ref.m_FrameCount=0;

	// loop through all frames to find counts
	for( unsigned int i=0 ; i != nf; ++i )
	{
		const frame* thisFrame = GetFrame(i);

		if(firstName=="" || strncmp(firstName.c_str(),thisFrame->name,firstName.size()) != 0 )
		{
			firstName = thisFrame->name;
			if(firstName[firstName.size()-2]=='0')
				firstName.resize(firstName.size()-2);
			else 
				firstName.resize(firstName.size()-1);

			if(ref.m_FrameCount!=0) {
				m_Anims.push_back(ref);
				ref.m_FrameStart=i;
				ref.m_FrameCount=0;
			}
		}
		++ref.m_FrameCount;
	}

	// make sure we don't miss the last cycle
	if(ref.m_FrameCount!=0) 
		m_Anims.push_back(ref);


	return true;	
}
