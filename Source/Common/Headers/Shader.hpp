#ifndef __ZED_RENDERER_SHADER_HPP__
#define __ZED_RENDERER_SHADER_HPP__

#include <Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		///////////////////////////////////////////////////////////////////////
		// CLASSES ////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		class Shader
		{
		public:
			ZED_INLINE Shader( ){ }
			Shader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
				const ZED_BOOL p_Geometry );
			virtual ~Shader( );

			// Compile the shader either from file or from memory
			virtual ZED_UINT32 Compile( const ZED_UCHAR8 *p_pData,
				const ZED_SHADER_TYPE p_Type,
				const ZED_BOOL p_FromFile ) = 0;

			// Just create a shader from MicroCode
			virtual ZED_UINT32 Create( const ZED_UCHAR8 *p_pData,
				const ZED_SHADER_TYPE p_Type,
				const ZED_BOOL p_FromFile ) = 0;

			// Set this shader as the active shader
			virtual ZED_UINT32 Activate( ) = 0;

			// Stop using this shader, but don't get rid of its resources
			virtual void Deactivate( ) = 0;

			// Add a shader type (if not using the setup contructor)
			virtual void AddType( const ZED_SHADER_TYPE p_Type ) = 0;

			// Clean up the shader and its resources
			virtual void Delete( ) = 0;

			// Save in-memory shader to file for modification or retrival
			// Saves as either HLSL or MicroCode
			virtual ZED_UINT32 Save( const ZED_UCHAR8 *p_pFile,
				const ZED_BOOL p_HLSL ) = 0;

			// Set the types to use for input
			virtual ZED_UINT32 SetTypes( ZED_SHADER_INPUT_MAP *p_pTypes ) = 0;

			// Set one of the input variables
			virtual ZED_UINT32 SetInput( const ZED_SHADER_INPUT_TYPE p_Type,
				const void *p_pValue ) = 0;
		};
	}
}

#endif
