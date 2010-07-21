//------------------------------------------------------------------------------------
///	\file	MD2_File.h
///	\author	Rob Bateman [robthebloke@hotmail.com]
/// \date	30-12-2004
///	\brief	This file provides possibly the easiest way to load an MD2 file. The entire
///			file is loaded into memory and data is accessed by simply using pointer
///			offsets into the data. This makes the final MD2Model class a little less 
///			efficient since all of the data needs to be converted when it gets updated.
/// 
//------------------------------------------------------------------------------------

#ifdef WIN32
	#pragma once
#endif

#ifndef MD2__FILE__H__
#define MD2__FILE__H__

#pragma pack(push,1)

#include <assert.h>
#include <vector>
#include <string>

class MD2 {
private:	

	/// ctor
	MD2();

	/// dtor
	~MD2();

public:

	//-------------------------------------------------------------------------------
	// MD2 data types
	// 

	struct model
	{ 
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
	};
	
	struct triangle
	{
		short vertexIndices[3];
		short textureIndices[3];
	};

	struct uv
	{
		union {
			struct {
				short s;
				short t;
			};
			short data[2];
		};
	};

	struct vertex
	{
		unsigned char vert[3];
		unsigned char normalIndex;
	};

	struct frame
	{
		float scale[3];
		float translate[3];
		char  name[16];
		vertex vertices[1];
	};

	struct glCommandVertex
	{
		union {
			struct {
				float s;
				float t;
			};
			float data[2];
		};
		int vertexIndex;
	};

	struct glCommandList
	{
		int num;
		glCommandVertex verts[1];
	};
	
	/// holds a reference to an animation within the MD2 file
	struct AnimRef {

		/// ctor
		AnimRef() {}

		/// copy ctor
		AnimRef(const AnimRef& ref) 
			:	m_FrameStart(ref.m_FrameStart),
			m_FrameCount(ref.m_FrameCount){}

		/// first frame in cycle
		unsigned short m_FrameStart;

		/// number of frames in the cycle
		unsigned short m_FrameCount;
	};
	

	/// this loads an MD2 into a single buffer
	static MD2* Load(const char* filename);

	//-------------------------------------------------------------------------------
	// funcs that use pointer offsets to return parts of the currently loaded MD2.
	// this really does nothing else!
	// 

	const model*		 GetModel() const;
	const frame*		 GetFrame(unsigned int num) const;
	const char*			 GetSkin(unsigned int num) const;
	const triangle*		 GetTriangles() const;
	const uv*			 GetTexCoords() const;
	unsigned char*		 GetCommandsStart() ;
	unsigned char*		 GetCommandsEnd() ;


	unsigned short GetNumVerts() const {
		return GetModel()->numVertices;
	}
	unsigned short GetNumTriangles() const {
		return GetModel()->numTriangles;
	}

	/// the number of anims in the md2 
	unsigned short GetNumAnims() const {
		return static_cast<unsigned short>(m_Anims.size());
	}

	const AnimRef* GetAnim(unsigned int idx) const {
		return &(m_Anims[idx]);
	}
	/// releases an MD2 file only if no longer referenced
	static void Release(MD2* ptr);

	static unsigned int GetDataSize();

private:
	
	unsigned int DataSize() const {
		return static_cast<unsigned int>(data_size + m_Anims.size() * sizeof(MD2::AnimRef) + sizeof(MD2));
	}
	
	//-------------------------------------------------------------------------------
	// funcs that load and release the MD2 file (in a global buffer)
	// 

	//-------------------------------------------------------------------------------
	// funcs that load and release the MD2 file (in a global buffer)
	// 

	/// this loads an MD2 into a single buffer
	bool InternalLoad(const char* filename);

	unsigned char* m_data;
	unsigned int data_size;


	/// quick references for the animation cycles
	std::vector<AnimRef> m_Anims;

	/// a global lookup to find any loaded md2's
	struct MD2Ref {
		MD2Ref() : ptr(0),ref(0),name() {}
		MD2Ref(MD2* p,const char* n) : ptr(p),ref(1),name(n) {}
		MD2Ref(const MD2Ref& p) : ptr(p.ptr),ref(p.ref),name(p.name) {}
		MD2* ptr;
		unsigned int ref;
		std::string name;
	};

	typedef std::vector<MD2Ref> RefList;
	typedef std::vector<MD2Ref>::iterator RefIter;
	static RefList m_Refs;

	static RefIter FindMd2(const char* name);
	static RefIter FindMd2(const MD2* ptr);


};

#pragma pack(pop)

#endif
