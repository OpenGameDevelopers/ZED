#include "MD2_File.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>

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

const MD2::glCommandList* MD2::GetCommands() const  {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetGlCommands;
	return reinterpret_cast<glCommandList*>(ptr);
}

const char*	MD2::GetSkin(unsigned int num) const {
	if(!m_data) return 0;
	void* p = m_data + GetModel()->offsetSkins + num*64;
	return reinterpret_cast<char*>(p);
}

// this loads an MD2 into a single buffer
bool MD2::Load(const char* filename) {
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

	return true;	
}
