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

			virtual ZED_UINT32 Compile( const ZED_UCHAR8 *p_pData,
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

			virtual ZED_UINT32 SetTypes( ZED_SHADER_INPUT_MAP *p_pTypes );

			virtual ZED_UINT32 SetInput( const ZED_SHADER_INPUT_TYPE p_Type,
				const void *p_pValue );

			ZED_INT32 GetLocation( );

		private:
			ZED_INT32 m_VertexID, m_FragmentID, m_GeometryID;
			ZED_INT32 *m_pLocationsID;
		};
	}
}

#endif
