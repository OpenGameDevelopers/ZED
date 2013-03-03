#ifndef __ZED_RENDERER_GLRENDERTYPES_HPP__
#define __ZED_RENDERER_GLRENDERTYPES_HPP__

typedef struct __ZED_SHADER_VERTEXATTRIBUTE_GL : __ZED_SHADER_VERTEXATTRIBUTE
{
	char			*pName;
}ZED_SHADER_VERTEXATTRIBUTE_GL;

typedef struct __ZED_GLVERSION
{
	ZED_SINT32	Major;
	ZED_SINT32	Minor;
}ZED_GLVERSION;

#endif

