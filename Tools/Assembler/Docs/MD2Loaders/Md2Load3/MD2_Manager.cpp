//-------------------------------------------------------------------------------------
/// \file	MD2_Manager.cpp
/// \author	Rob Bateman	[robthebloke@hotmail.com]
/// \date	30-12-2004
/// \brief	This MD2 loader is setup in a fairly non-intuitive way. The reason is to 
///			create as many animated characters as possible whilst maintaining the smallest
///			amount of memory and processor usage. hehe, i want to be able to handle
///			about 500 visibile characters, and maybe another 2000 within the scene at
///			any one time!
///
///			As far as usage goes, the Md2Instance class is all you really want to be 
///			concerned with. This is used to set the characters position and orientation;
///			and also controls which anim cycle is used. The anim switching could be
///			improved somewhat, but i'll leave that for you to do.... ;)
///
///			The Md2Manager class allows you to create new instances of models, and provides
///			global functions to query memory usage, update all model instances, and 
///			render all instances. Control is somewhat taken away from direct manipulation
///			of Md2Models, however this is done to ensure that we use some fairly hacky
///			tricks to minimise the update calculation and minimise state changes when
///			rendering the models. 
/// 
///			By providing a global manager, we can also maintain usage of textures and
///			models loaded by the game engine. ie, we can use reference counting so that
///			texture and model data gets unloaded when we no longer need it. Since this
///			process is automated, simly use Md2Manager::Load and Md2Manager::Delete
///			to your little hearts content ;)
/// 
///			Since this code does not natively support the loading of textures, a mechanism
///			is provided for you to specify a callback function to load this data. The 
///			textures loaded are those specified as skins within the actual MD2 file. Since
///			this may be set up with some odd path, first the path is checked 
///			
//-------------------------------------------------------------------------------------

#include "MD2_Manager.h"

extern unsigned int g_LastBound;

//-----------------------------------------------------------------------------------------------	class Md2Manager
//###############################################################################################
//-----------------------------------------------------------------------------------------------

Md2Manager::Md2TexLoadMap Md2Manager::m_SupportedTexFormats;
std::vector<Md2Manager::ModelRef*> Md2Manager::m_LoadedModels;
std::vector<Md2Manager::TextureRef*> Md2Manager::m_LoadedTextures;
unsigned char Md2Manager::m_UpdateStagger=1;
unsigned char Md2Manager::m_UpdateCurrent=0;
float Md2Manager::m_LastUpdates[8]={0.0f};

//-----------------------------------------------------------------------------------------------	Md2Manager :: CurrentModelFps
//
float Md2Manager::CurrentModelFps() {
	float frame_time=0;
	for(unsigned int i=0;i!=m_UpdateStagger;++i)
		frame_time += m_LastUpdates[i];
	return 1.0f/frame_time;
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: GetStagger
//
unsigned char Md2Manager::GetStagger() {
	return m_UpdateStagger;
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: GetMemoryUsage
//
unsigned int Md2Manager::GetMemoryUsage(Md2MemoryType type) {
	unsigned int sz=0;
	if(type == MEM_GLOBAL || type == MEM_ALL )
	{
		sz += (MD2_BUFFER_SIZE+2048)*sizeof(Md2VertexNormal) +
				sizeof(float)*8 +	2 +
				(sizeof(ModelRef)+sizeof(ModelRef*))*m_LoadedModels.size()  + 
				(sizeof(TextureRef)+sizeof(TextureRef*))*m_LoadedTextures.size() +
				sizeof(std::vector<ModelRef*>) + 
				sizeof(std::vector<TextureRef*>) +
				sizeof(Md2TexLoadMap);
	}
	if(type == MEM_TEXTURES || type == MEM_ALL ) {
		std::vector<Md2Manager::TextureRef*>::iterator it = Md2Manager::m_LoadedTextures.begin();
		for( ; it != Md2Manager::m_LoadedTextures.end(); ++it ) {
			sz += (*it)->Size;
		}
	}
	std::vector<Md2Manager::ModelRef*>::iterator it = m_LoadedModels.begin();
	for( ; it != m_LoadedModels.end(); ++it ) {
		sz += (*it)->pModel->GetDataSize(type);
	}
	return sz;
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: IsValid
//
bool Md2Manager::IsValid(const Md2Instance* ptr) {
	std::vector<Md2Manager::ModelRef*>::const_iterator it = m_LoadedModels.begin();
	for( ; it != m_LoadedModels.end(); ++it )
	{
		unsigned int i=0;
		for( ; i != (*it)->pModel->m_Instances.size(); ++i )
		{
			if(ptr == (*it)->pModel->m_Instances[i])
				return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: Load
//
Md2Instance* Md2Manager::Load(const char* filename,unsigned short fps) {
	
	std::vector<ModelRef*>::iterator it = m_LoadedModels.begin();
	for( ; it != m_LoadedModels.end(); ++it )
	{
		if( (*it)->Filename == filename ) {
			return (*it)->pModel->CreateInstance();
		}
	}
	ModelRef* pmref = new ModelRef;
	assert(pmref);
	pmref->pModel = new Md2Model;
	assert(pmref->pModel);
	pmref->Filename = filename;

	if(!pmref->pModel->Load(filename)) {
		delete pmref->pModel;
		delete pmref;
		return 0;
	}

	pmref->pModel->SetFps(fps);

	m_LoadedModels.push_back(pmref);

	return pmref->pModel->CreateInstance();
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: RegisterTexLoader
//
bool Md2Manager::RegisterTexLoader(const char* ext,Md2TexLoadFunc func) {
	if( m_SupportedTexFormats.find(ext) == m_SupportedTexFormats.end() )
	{
		m_SupportedTexFormats.insert( std::make_pair( std::string(ext),func) );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: Release
//
void Md2Manager::Release() {
	std::vector<Md2Manager::ModelRef*>::iterator it = m_LoadedModels.begin();
	for( ; it != m_LoadedModels.end(); ++it ) {
		delete (*it)->pModel;
		delete *it;
	}
}

//-----------------------------------------------------------------------------------------------	Md2Manager :: Render
//
void Md2Manager::Render() {

	glEnable(GL_TEXTURE_2D);

	#if MD2_USE_NORMALS
		glEnable(GL_NORMALIZE);
		glEnableClientState(GL_NORMAL_ARRAY);
	#endif

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// render all model instances
	std::vector<ModelRef*>::iterator it = m_LoadedModels.begin();
	for( ; it != m_LoadedModels.end(); ++it )
	{
		(*it)->pModel->Render();
	}

	#if MD2_USE_NORMALS
		glDisable(GL_NORMALIZE);
		glDisableClientState(GL_NORMAL_ARRAY);
	#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// set the last bound texture to invalid. Should prevent errors between frames
	// if someone binds a different texture for say the level
	g_LastBound = 0;
}


	//---------------------------------------------------------------------------------
	/// this function allows you to stagger the updates for the models. ie, 2 would 
	///	indicate that it should update every other frame, thus only half the models are
	/// updated each frame.
	///
	void Md2Manager::SetStagger(unsigned char num) {
		if(num>8)
			num = 8;
		if(!num)
			num=1;
		m_UpdateStagger = num;
	}

//-----------------------------------------------------------------------------------------------	Md2Manager :: Update
//
void Md2Manager::Update(float dt) {
	float ddt = 0.0f;
	m_LastUpdates[ m_UpdateCurrent ] = dt;
	for( unsigned char c = 0; c != m_UpdateStagger; ++c ) 
		ddt += m_LastUpdates[c];

	std::vector<ModelRef*>::iterator it = m_LoadedModels.begin();
	for( unsigned char i=m_UpdateCurrent; it != m_LoadedModels.end(); ++it, ++i )
	{
		(*it)->pModel->Update(ddt,i%m_UpdateStagger,m_UpdateStagger);
	}

	if( (++m_UpdateCurrent) >= m_UpdateStagger )
		m_UpdateCurrent = 0;
}










//-----------------------------------------------------------------------------------------------	Md2ReleaseTex
//
void Md2ReleaseTex(unsigned int tex)
{
	std::vector<Md2Manager::TextureRef*>::iterator it = Md2Manager::m_LoadedTextures.begin();
	for( ; it != Md2Manager::m_LoadedTextures.end(); ++it ) {
		if( (*it)->TexID == tex ) {

			unsigned int num=0;


			std::vector<Md2Manager::ModelRef*>::iterator itm = Md2Manager::m_LoadedModels.begin();
			for( ; itm != Md2Manager::m_LoadedModels.end(); ++itm ) {
				unsigned int ii=0;
				for( ; ii != (*itm)->pModel->m_Instances.size(); ++ii ) {
					if( tex == (*itm)->pModel->m_Instances[ii]->m_CurrentSkin ) {
						++num;
					}
				}
			}

			if(num<2) {
				glDeleteTextures( 1, &tex );
				delete *it;
				Md2Manager::m_LoadedTextures.erase(it);
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------	Md2CheckTexSize
//
unsigned int Md2CheckTexSize(unsigned int tsize) {
	if(tsize>=1024)
		return 1024;
	if(tsize>=512)
		return 512;
	if(tsize>=256)
		return 256;
	if(tsize>=128)
		return 128;
	if(tsize>=64)
		return 64;
	if(tsize>=32)
		return 32;
	if(tsize>=16)
		return 16;
	if(tsize>=8)
		return 8;
	if(tsize>=4)
		return 4;
	return 2;
}

//-----------------------------------------------------------------------------------------------	Md2LoadTex
//
unsigned int Md2LoadTex(const char* filename) {

	// check loaded textures first
	{
		std::vector<Md2Manager::TextureRef*>::iterator it = Md2Manager::m_LoadedTextures.begin();
		for( ; it != Md2Manager::m_LoadedTextures.end(); ++it ) {
			if( (*it)->Filename == filename ) {
				return (*it)->TexID;
			}
		}
	}
	// strip extension
	int len= static_cast<int>( strlen(filename) );
	while( len > 0 && filename[--len] != '.' ) /* do nothing */;

	std::string ext = "";
	if(len!=0) {
		ext = filename + len + 1;
	}
	
	// find filename without path
	len = static_cast<int>( strlen(filename) );
	while( len > 0 && filename[len] != '\\' && filename[len] != '/' ) {
		--len;
	}
	const char* minus_path = filename+len+1;

	unsigned int w=0,h=0,bpp=0;
	unsigned char* pixels=0;

	std::string final_tex="";

	// if no extension exists. try the image name with all supported extensions, with and without its path
	if(ext == "") {

		Md2Manager::Md2TexLoadMap::iterator it = Md2Manager::m_SupportedTexFormats.begin();
		for( ; it != Md2Manager::m_SupportedTexFormats.end(); ++it ) {
			Md2TexLoadFunc func = it->second;
			ext = it->first;
			assert(func);

			std::string s = filename;
			s += ".";
			s += ext;
			
			if(func(s.c_str(),&pixels,&w,&h,&bpp)) {
				final_tex = s;
				goto make_texture;
			}
			s = minus_path;
			s += ".";
			s += ext;
			if(func(s.c_str(),&pixels,&w,&h,&bpp)) {
				final_tex = s;
				goto make_texture;
			}
		}

	}
	// otherwise
	else {

		Md2Manager::Md2TexLoadMap::iterator it = Md2Manager::m_SupportedTexFormats.find(ext);
		if( it != Md2Manager::m_SupportedTexFormats.end() ) {
			Md2TexLoadFunc func = it->second;
			assert(func);

			if(func(filename,&pixels,&w,&h,&bpp)) {
				final_tex = filename;
				goto make_texture;
			}
			if(func(minus_path,&pixels,&w,&h,&bpp)) {
				final_tex = minus_path;
				goto make_texture;
			}
		}
	}
	return 0;

make_texture:

	GLenum infmt,outfmt;
	switch(bpp) {
	case 3:
		infmt = GL_RGB;
		#if	!MD2_USE_COMPRESSED_TEXTURES
			outfmt = GL_RGB;
		#else
			outfmt = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		#endif
		break;
	case 4:
		infmt = GL_RGBA;
		#if	!MD2_USE_COMPRESSED_TEXTURES
			outfmt = GL_RGBA;
		#else
			outfmt = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		#endif
		break;
	case 1:
		infmt = outfmt = GL_ALPHA;
		break;
	case 2:
		infmt = outfmt = GL_RGB5_A1;
		break;
	default:
		free(pixels);
		return 0;
	}
	
	unsigned int nw = Md2CheckTexSize(w);
	unsigned int nh = Md2CheckTexSize(h);

	if(nw != w || nh != h ) {
		unsigned char *newPixels = (unsigned char*)malloc(nw*nh*bpp);

		// this might go wrong ;)
		gluScaleImage(infmt,w,h,GL_UNSIGNED_BYTE,pixels,nw,nh,GL_UNSIGNED_BYTE,newPixels);
		free(pixels);
		pixels = newPixels;
		w=nw;
		h=nh;
	}

	unsigned int tex_obj=0;
	glGenTextures(1,&tex_obj);


	glBindTexture (GL_TEXTURE_2D, tex_obj);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,outfmt,w,h,0,infmt,GL_UNSIGNED_BYTE,pixels);

	free(pixels);
	
	Md2Manager::TextureRef* tref = new Md2Manager::TextureRef;
	
	#if	MD2_USE_COMPRESSED_TEXTURES
		int compressed_size=0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB,&compressed_size);
	tref->Size = compressed_size;
	#else
		tref->Size = w*h*bpp;		
	#endif

	tref->Filename = filename;
	tref->TexID = tex_obj;

	Md2Manager::m_LoadedTextures.push_back(tref);

	return tex_obj;
}

