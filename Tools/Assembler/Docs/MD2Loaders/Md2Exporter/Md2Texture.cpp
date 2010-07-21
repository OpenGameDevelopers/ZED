#include "Md2Texture.h"
#ifdef WIN32
#define NT_PLUGIN
#endif
#include <maya/MImage.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>

int g_SkinWidth=256;
int g_SkinHeight=256;
#ifndef WIN32
std::string g_Path="textures/";
#else
std::string g_Path="textures\\";
#endif
std::string g_FileExtension="bmp";
std::vector<std::string> g_SkinNames;
unsigned int GetDim(unsigned int i) {
	if(i>=2048) return 2048;
	if(i>=1024) return 1024;
	if(i>=512) return 512;
	if(i>=256) return 256;
	if(i>=128) return 128;
	if(i>=64) return 64;
	if(i>=32) return 32;
	if(i>=16) return 16;
	if(i>=8) return 8;
	if(i>=4) return 4;
	if(i>=2) return 2;
	return 1;
}
#ifdef WIN32
#include <windows.h>
bool DirExists(const char* fname) {
	char buffer[512];
	GetCurrentDirectory(512,buffer);
	DWORD res = SetCurrentDirectory(fname);
	SetCurrentDirectory(buffer);
	return res == TRUE;
}
bool MakeDir(const char* fname) {
	return CreateDirectory(fname,NULL)!=0;
}
#else
#include <unistd.h>
#include <dirent.h>
bool DirExists(const char* fname) {
	DIR* pdir = opendir(fname);
	if(pdir) {
		closedir(pdir);
		return true;
	}
	return false;
}
bool MakeDir(const char* fname) {
	return -1 != mkdir(fname, S_IRWXU);
}
#endif
std::string WriteImage(const std::string &fullpath,MObject obj) {
	MFnDependencyNode fn(obj);
	MImage img;
	img.readFromTextureNode(obj);
	// check size is power of 2, if not resize image
	unsigned int w,h,nw,nh;
	img.getSize(w,h);
	nw = GetDim(w);
	nh = GetDim(h);
	if(nw != w || nh != h) {
		img.resize(nw,nh,false);
	}
	// construct a new filename
	std::string file = fullpath;
	std::string dirPath = fullpath;
	dirPath += g_Path;
	if(!DirExists(dirPath.c_str()) ) {
		if(!MakeDir(dirPath.c_str())  ) {
			img.release();
			return false;
		}
	}
	std::string nameonly = g_Path;
	nameonly += fn.name().asChar();
	nameonly += ".";
	nameonly += g_FileExtension;
	file += nameonly;
	img.writeToFile(file.c_str(),g_FileExtension.c_str());
	img.release();
	return nameonly;
}
bool GetTexNode(MObject& obj,MString name) {
	MSelectionList sl;
	MGlobal::getSelectionListByName(name,sl);
	if(sl.length()==0)
		return false;
	sl.getDependNode(0,obj);
	if(obj.hasFn(MFn::kFileTexture)) {
		return true;
	}
	return false;
}
bool DoTextures(const std::string &fullpath) {
	bool OK=true;
	bool doAll = (g_SkinNames.size()==0);
	if(doAll) {
		MItDependencyNodes it(MFn::kFileTexture);
		while(!it.isDone()) {
			std::string nameonly = WriteImage(fullpath,it.item());
			// store skin name for Md2
			g_SkinNames.push_back(nameonly);
			it.next();
		}
	}
	else {
		std::vector<std::string>::iterator itn= g_SkinNames.begin();
		for( ; itn != g_SkinNames.end();++itn ) {
			MObject obj;
			if(GetTexNode(obj,itn->c_str())) {
					*itn = WriteImage(fullpath,obj);
			}
			else {
				std::cerr << "Missing a file texture in the maya binary file \"" << itn->c_str() << "\"\n";
				OK=false;
			}
		}
	}
	return OK;
}
