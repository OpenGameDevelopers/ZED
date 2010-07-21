// Use this at your own risk. For non-commercial use only. (c) Rob Bateman 2005
//
#ifndef __MD2_TEXTURE__H__
#define __MD2_TEXTURE__H__
	#include <vector>
	#include <string>
	extern int g_SkinWidth;
	extern int g_SkinHeight;
	extern std::string g_FileExtension;
	extern std::vector<std::string> g_SkinNames;
	bool DoTextures(const std::string &fullpath);
#endif
