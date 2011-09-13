#ifndef __ZED_RENDERER_GLSHADER_HPP__
#define __ZED_RENDERER_GLSHADER_HPP__

#include <Shader.hpp>
#include <GL/GL.h>

namespace ZED
{
	namespace Renderer
	{
		class GLShader : public Shader
		{
		public:
			GLShader( );
			GLShader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
				const ZED_BOOL p_Geometry );
			virtual ZED_INLINE ~GLShader( ){ Delete( ); }

			virtual ZED_UINT32 Compile( const ZED_CHAR8 **p_ppData,
				const ZED_SHADER_TYPE p_Type,
				const ZED_BOOL p_FromFile );
			
			virtual ZED_UINT32 Create( const ZED_UCHAR8 *p_pData,
				const ZED_SHADER_TYPE p_Type,
				const ZED_BOOL p_FromFile );

			virtual ZED_UINT32 Activate( );
			virtual void Deactivate( );

			virtual void AddType( const ZED_SHADER_TYPE p_Type );

			virtual void Delete( );

			virtual ZED_UINT32 Save( const ZED_UCHAR8 *p_pFile,
				const ZED_BOOL p_HLSL );

			virtual ZED_UINT32 SetVertexAttributeTypes(
				const ZED_SHADER_VA_MAP *p_pVAMap,
				const ZED_MEMSIZE p_Count );

			virtual ZED_UINT32 SetVariableTypes(
				const ZED_SHADER_INPUT_MAP *p_pTypes,
				const ZED_UINT32 p_Count );
			
			virtual ZED_UINT32 SetVariable( const ZED_UINT32 p_Index,
				const void *p_pValue );

			ZED_INT32 GetLocation( );

			ZED_UINT32 Link( );

			ZED_UINT32 AttachShaders( );

		private:
			ZED_INT32 m_VertexID, m_FragmentID, m_GeometryID;
			ZED_INT32 m_ProgramID;
			ZED_INT32 *m_pLocationsID;

			// Keep the shader input map around.
			ZED_SHADER_INPUT_MAP	*m_pInputMap;
			ZED_SHADER_VA_MAP		*m_pVAMap;

			ZED_BOOL	m_Linked;
			ZED_BOOL	m_ShadersAttached;

			// Store the source for debugging
#ifdef ZED_BUILD_DEBUG
			ZED_CHAR8 **m_ppVertexSrc;
			ZED_CHAR8 **m_ppFragmentSrc;
			ZED_CHAR8 **m_ppGeometrySrc;
#endif

			// 0 == Vertex
			// 1 == Fragment
			// 2 == Geometry
			ZED_BOOL m_pInitShader[ 3 ];
		};
	}
}

#endif
