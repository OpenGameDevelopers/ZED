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

class MD2 {
protected:	

	unsigned short GetNumVerts() const {
		return GetModel()->numVertices;
	}
	unsigned short GetNumTriangles() const {
		return GetModel()->numTriangles;
	}

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

protected:
	

	//-------------------------------------------------------------------------------
	// funcs that load and release the MD2 file (in a global buffer)
	// 

	/// this loads an MD2 into a single buffer
	bool Load(const char* filename);

	//-------------------------------------------------------------------------------
	// funcs that use pointer offsets to return parts of the currently loaded MD2.
	// this really does nothing else!
	// 

	const model*		 GetModel() const;
	const frame*		 GetFrame(unsigned int num) const;
	const char*			 GetSkin(unsigned int num) const;
	const triangle*		 GetTriangles() const;
	const uv*			 GetTexCoords() const;
	const glCommandList* GetCommands() const;

	unsigned char* m_data;
	unsigned int data_size;

	MD2() : m_data(0) {}
	~MD2() {
		delete [] m_data;
		m_data=0;
	}
};

#pragma pack(pop)

#endif
