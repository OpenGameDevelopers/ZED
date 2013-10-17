#ifndef __ZED_RENDERER_GLES2RENDERTYPES_HPP__
#define __ZED_RENDERER_GLES2RENDERTYPES_HPP__

typedef struct __ZED_SHADER_VERTEXATTRIBUTE_GLES2 :
	__ZED_SHADER_VERTEXATTRIBUTE
{
	char	*pName;
}ZED_SHADER_VERTEXATTRIBUTES_GLES2;

typedef struct __ZED_GLES2VERSION
{
	ZED_SINT32	Major;
	ZED_SINT32	Minor;
}ZED_GLES2VERSION;

typedef GLenum ZED_RENDERPRIMITIVETYPE;

const GLenum ZED_TRIANGLE_LIST	= GL_TRIANGLES;
const GLenum ZED_TRIANGLE_FAN	= GL_TRIANGLE_FAN;
const GLenum ZED_TRIANGLE_STRIP	= GL_TRIANGLE_STRIP;
const GLenum ZED_POINTS			= GL_POINTS;
const GLenum ZED_LINE_LIST		= GL_LINES;
const GLenum ZED_LINE_LOOP		= GL_LINE_LOOP;
const GLenum ZED_LINE_STRIP		= GL_LINE_STRIP;

#endif

