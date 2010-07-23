#ifndef __ZEDTOOLASSEMBLER_MAYAASSEMBLER_HPP__
#define __ZEDTOOLASSEMBLER_MAYAASSEMBLER_HPP__

#include <ModelTypes.hpp>
#include <AssemblerBase.hpp>
#include <maya/MObject.h>

class MayaAssembler : public Assembler
{
public:
private:
	MFnMesh m_Mesh;
};

#endif
