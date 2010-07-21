// Use this at your own risk. For non-commercial use only. (c) Rob Bateman 2005
//
#ifdef WIN32
	#pragma comment(lib,"Image.lib")
	#pragma comment(lib,"OpenMaya.lib")
	#pragma comment(lib,"Foundation.lib")
	#pragma comment(lib,"OpenMayaAnim.lib")
	#pragma comment(lib,"lua.lib")
	#pragma comment(lib,"lualib.lib")
#endif
#include <stdlib.h>
#include <fstream>
#include "Md2Default.h"
#include "Md2Mesh.h"
#include "Md2Texture.h"
#include "FileDialog.h"
extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}
#include <maya/MLibrary.h>
#include <maya/MFileIO.h>
#include <maya/MFnMesh.h>

int Linfo(lua_State* L);
int Lskins(lua_State* L);
int Lmesh_list(lua_State* L);
int Lanim(lua_State* L);

struct AnimRef {
	AnimRef() 
		: name(""),file(""),start(1),end(24)
	{}
	AnimRef(const AnimRef& ref) 
		: name(ref.name),file(ref.file),start(ref.start),end(ref.end) 
	{}
	std::string name;
	std::string file;
	int start;
	int end;
};

std::vector<AnimRef> g_Anims;
MStringArray g_Meshes;
std::string g_OutputMd2="";
lua_State* g_LUA=0;
Md2Mesh g_OutMesh;
Md2AnimList g_OutAnims;

#pragma pack(push,1)
struct model {
	model() {
		memset(this,sizeof(model),0);
		magic = 0x32504449;
		version = 8;
		skinWidth = g_SkinWidth;
		skinHeight = g_SkinHeight;
	}
	int magic;
	int version;
	int skinWidth;
	int skinHeight;
	int frameSize;
	int numSkins;
	int numVertices;
	int numTexCoords;
	int numTriangles;
	int numGlCommands;
	int numFrames;
	int offsetSkins;
	int offsetTexCoords;
	int offsetTriangles;
	int offsetFrames;
	int offsetGlCommands;
	int offsetEnd;
	friend std::ostream& operator <<(std::ostream& os,const model& m) {
		os << m.magic << "\n"
		<< m.version << "\n"
		<< m.skinWidth << "\n"
		<< m.skinHeight << "\n"
		<< m.frameSize << "\n"
		<< m.numSkins << "\n"
		<< m.numVertices << "\n"
		<< m.numTexCoords << "\n"
		<< m.numTriangles << "\n"
		<< m.numGlCommands << "\n"
		<< m.numFrames << "\n"
		<< m.offsetSkins << "\n"
		<< m.offsetTexCoords << "\n"
		<< m.offsetTriangles << "\n"
		<< m.offsetFrames << "\n"
		<< m.offsetGlCommands << "\n";
		return os;
	}
};
#pragma pack(pop)
bool DoExport(std::string path) {
	std::cout << "[WRITING MD2 FILE]" << std::endl;
	model m;
	m.frameSize = g_OutMesh.m_NumVerts*4 + 16 + 6*sizeof(float);
	m.numSkins = g_SkinNames.size();
	m.numVertices = g_OutMesh.m_NumVerts;
	m.numTexCoords = g_OutMesh.m_Uvs.size();
	m.numTriangles = g_OutMesh.m_Faces.size();
	m.numGlCommands = 0;
	m.numFrames = g_OutAnims.GetNumFrames();
	m.offsetSkins = sizeof(model);
	m.offsetTexCoords = m.offsetSkins + 64 * m.numSkins;
	m.offsetTriangles = m.offsetTexCoords + 4*m.numTexCoords;
	m.offsetFrames = m.offsetTriangles + 12*m.numTriangles;
	m.offsetGlCommands = m.offsetFrames + m.numFrames*m.frameSize;
	m.offsetEnd = m.offsetGlCommands;
	std::cout << m;
	std::string outfile = path;
	outfile +=g_OutputMd2;
	FILE* fp = fopen(outfile.c_str(),"wb");
	if(!fp) {
		std::cerr << "Could not open file. Exiting\n";
		return 0;
	}
	fwrite(&m,sizeof(model),1,fp);
	std::cout << "offsetSkins " << ftell(fp) << std::endl;
	std::vector<std::string>::iterator it = g_SkinNames.begin();
	for( ; it != g_SkinNames.end(); ++it ) {
		char buffer[64] = {0};
		strcpy(buffer,it->c_str());
		fwrite(buffer,64,1,fp);
	}
	std::cout << "offsetTexCoords " << ftell(fp) << std::endl;
	g_OutMesh.Write(fp);
	std::cout << "offsetFrames " << ftell(fp) << std::endl;
	g_OutAnims.Write(fp);
	std::cout << "offsetGlCommands " << ftell(fp) << std::endl;
	std::cout << "[MD2 FILE OK]" << std::endl;
	std::cout << "\n[ *** MY GOD! IT WORKED! *** ]" << std::endl << std::endl;
	fclose(fp);
	return true;
}
bool BuildData(std::string outPath) {
	std::vector<AnimRef>::iterator it = g_Anims.begin();
	for( ; it != g_Anims.end(); ++it ) {
		if(MS::kSuccess != MFileIO::open(it->file.c_str(),0,true)) {
			std::string fname = outPath;
			fname += it->file.c_str();
			if(MS::kSuccess != MFileIO::open(fname.c_str(),0,true)) {
				std::cerr << "Unable to open input file. Tried to load both \""
						  << it->file.c_str() << "\" & \""
						  << fname.c_str() << "\" [aborting export] .... :(\n";
				return false;
			}
		}
		std::cout << "[INFO: file = \"" << it->file.c_str() << "\"]\n";

		if( it == g_Anims.begin() ) {
			std::cout << "[BUILDING MD2 MESH DATA]" << std::endl;
			if(!g_OutMesh.Build(g_Meshes)) {
				return false;
			}
			std::cout << "[MD2 MESH DATA OK]" << std::endl;
			std::cout << "[BUILDING MD2 SKIN DATA]" << std::endl;
			if(!DoTextures(outPath)) {
				std::cerr << "Skins err, didn't work. Bugger. Continuing with rest of export anyway....\n";
				std::cout << "[MD2 SKIN DATA F**KED]" << std::endl;
			}
			else
				std::cout << "[MD2 SKIN DATA OK]" << std::endl;
			std::cout << "[BUILDING MD2 ANIM DATA]" << std::endl;
		}
		if( !g_OutAnims.AddAnim(it->name.c_str(),it->start,it->end,g_Meshes) ) {
			std::cout << " Could not add anim file \"" << it->name.c_str() << "\"\n"
					 << "[aborting] :(\n";
			return false;
		}
	}
	std::cout << "[MD2 ANIM DATA OK]" << std::endl << std::endl;
	std::cout << "[ *** ALL MD2 DATA OK *** ]" << std::endl << std::endl;

	return true;
}
#ifdef WIN32
int main(int argc,char** argv) {
#else
int g_argc;
char** g_argv;
int main(int argc,char** argv) {
	g_argc = argc;
	g_argv = argv;
#endif
	std::cout << g_OutputInfo << std::endl;
	if(argc>1) {
		if(strcmp("-g",argv[1])==0) {
			char buffer[512];
			if( SaveFileDialog(buffer) ) {
				std::ofstream ofs(buffer);
				ofs << g_DefLuaScript;
				ofs.close();
			}
			else {
				std::cerr << "No output file specified. lua script could not be generated\n";
			}
			return 0;
		}
		if(strcmp("-gq2",argv[1])==0) {
			char buffer[512];
			if( SaveFileDialog(buffer) ) {
				std::ofstream ofs(buffer);
				ofs << g_Q2LuaScript;
				ofs.close();
			}
			else {
				std::cerr << "No output file specified. lua script could not be generated\n";
			}
			return 0;
		}
		if(strcmp("-h",argv[1])==0) {
			std::cerr << g_HelpString;
			return 0;
		}
		std::cerr << "Unknown command line option\n";
		return 0;
	}
	g_LUA = lua_open();
	if(!g_LUA) {
		std::cerr << "[ERROR] could not create a LUA Virtual Machine\n";
		return 0;
	}
	if(!MLibrary::initialize(argv[0])) {
		std::cerr << "Could not initialize maya\n";
		lua_close(g_LUA);
		return 0;
	}
	luaopen_base(g_LUA);
	luaopen_table(g_LUA);
	luaopen_io(g_LUA);
	luaopen_string(g_LUA);
	luaopen_math(g_LUA);
	lua_register(g_LUA,"info",Linfo);
	lua_register(g_LUA,"anim",Lanim);
	lua_register(g_LUA,"mesh_list",Lmesh_list);
	lua_register(g_LUA,"skins",Lskins);

	char luaScript[512];
	if(OpenFileDialog(luaScript)) {
		std::string outPath=luaScript;
		while( outPath.size() ) {
			if( *(outPath.end()-1)=='\\' ||
				*(outPath.end()-1)=='/' ) {
				break;
			}
			outPath.resize( outPath.size() - 1 );
		}
		if(!outPath.size()) {
			std::cerr << "Error finding path in which to write the file to\n";
			goto finish;
		}
		std::cout << "OutPath " << outPath.c_str() << std::endl;
		if(lua_dofile(g_LUA,luaScript)) {
			std::cerr << "Parse error in lua script. Conversion failed\n";
		}
		else {
			if(BuildData(outPath)) {
				if(!DoExport(outPath)) {
					std::cerr << "Somethings gone wrong when writing the MD2 file to disk :(\n";
				}
			}
			else std::cerr << "Could not build the required animation data\n";
		}
	}
	else std::cerr << "No lua conversion script specified\n";
finish:
	lua_close(g_LUA);
	MLibrary::cleanup(0);
	return 0;
}
int Linfo(lua_State* L) {

	if(lua_gettop(L)!=1 || !lua_istable(L,1)) {
		lua_pushstring(L,"ERROR - incorrect arg for info { }\n");
		lua_error(L);
		return 0;
	}
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		std::string key = lua_tostring(L, -2);
		if(key =="output") {
			g_OutputMd2 = lua_tostring(L,-1);
		}
		else
		if( key == "frame_step" ) {
			g_FrameStep = lua_tonumber(L,-1);
		}
		else
		if( key == "skin_width" ) {
			g_SkinWidth = lua_tonumber(L,-1);
		}
		else
		if( key == "skin_height" ) {
			g_SkinHeight = lua_tonumber(L,-1);
		}
		else
		if( key == "tex_format" ) {
			g_FileExtension = lua_tostring(L,-1);
		}
		else {
			std::cerr << "Unknown parameter for info chunk \"" << key.c_str() << "\"\n";
		}
		lua_pop(L, 1);
	}
	
	return 0;
}
int Lskins(lua_State* L) {
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		if(lua_isnumber(L,-2) && lua_isstring(L,-1))
			g_SkinNames.push_back( lua_tostring(L,-1) );
		else
			std::cerr << "Unexpected identifer in skins list\n";
		lua_pop(L, 1);
	}

	return 0;
}
int Lmesh_list(lua_State* L) {
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		if(lua_isnumber(L,-2) && lua_isstring(L,-1))
			g_Meshes.append( lua_tostring(L,-1) );
		else
			std::cerr << "Unexpected identifer in mesh_list\n";
		lua_pop(L, 1);
	}
	return 0;
}
int Lanim(lua_State* L) {
	if(  lua_gettop(L)!=1 ||
		!lua_istable(L,1) ) {
		lua_pushstring(L,"ERROR - incorrect arg for anim { }\n");
		lua_error(L);
		return 0;
	}
	AnimRef ref;
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		std::string key = lua_tostring(L, -2); 

		if(key =="name")
			ref.name = lua_tostring(L,-1);
		else
		if( key == "file" )
			ref.file = lua_tostring(L,-1);
		else
		if( key == "frame_start" )
			ref.start = lua_tonumber(L,-1);
		else
		if( key == "frame_end" )
			ref.end = lua_tonumber(L,-1);
		else
			std::cerr << "Unknown parameter for anim chunk \"" << key.c_str() << "\"\n";
		lua_pop(L, 1);
	}
	if( ref.name != "" && 
		ref.file != "") {
		g_Anims.push_back(ref);
	}
	return 0;
}
