#ifndef __ZED_RENDERER_MODEL_HPP__
#define __ZED_RENDERER_MODEL_HPP__

#include <ModelStructs.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Shader.hpp>
#include <Matrix3x3.hpp>
#include <Matrix4x4.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Model
		{
		public:
			Model( );
			~Model( );

			ZED_UINT32 Load( const char *p_pFilename );
			void Render( );
			void Update( const ZED_FLOAT32 p_Delta,
				const Arithmetic::Matrix4x4 &p_ViewProjection );

			void SetPosition( const Arithmetic::Vector3 &p_Position );
			void SetScale( const Arithmetic::Vector3 &p_Scale );
			void SetOrientation( const Arithmetic::Matrix3x3 &p_Orientation );

			// Debug functions
#ifdef ZED_BUILD_DEBUG
			// Set the wireframe colour for debugging
			void SetWireframeColour( const ZED_COLOUR &p_Colour );

			void ToggleWireframe( );
			void ToggleBones( );
			void ToggleFaceNormals( );
			void ToggleVertexNormals( );
#endif

		private:
			// Make sure this is a ZED Model file
			ZED_UINT32 LoadHeader( );

			// Grab the vertices
			ZED_UINT32 LoadVertices( ZED_UINT64 p_Size );
			ZED_UINT32 LoadFaces( ZED_UINT64 p_Size );
			ZED_UINT32 LoadMeshes( ZED_UINT64 p_Size );

			// Little helper!
			void ReadChunk( ZED_UINT16 &p_Type, ZED_UINT64 &p_Size );

			// Model data
			LPVERTEX_V1 m_pVertexData;

			// Faces for defining triangles and indices for doing so
			LPFACE_V1	m_pFaceData;

			ZED_UINT16	*m_pIndices;
			ZED_FLOAT32 *m_pVertices;
			ZED_FLOAT32 *m_pVertexNormals;
			ZED_FLOAT32 *m_pVertexUVs;
			ZED_FLOAT32 **m_ppFaceNormals;
			ZED_UINT16	**m_ppFaceIndices;

			// Vertex data and index count
			ZED_MEMSIZE m_VertexCount;
			ZED_MEMSIZE m_FaceCount;
			ZED_MEMSIZE m_IndexCount;
			// The total amount of meshes that create the model
			ZED_UINT32 m_MeshCount;

			FILE *m_pFile;

			// In case a big endian is loading little endian and
			// vise-versa
			ZED_BOOL m_SwapBytes;

			// The type of model [Character|Geometry]
			ZED_UCHAR8 m_Type;

			// For each mesh, there is a shader (unless the shader is shared)
			Shader	*m_pShader;

			Arithmetic::Matrix4x4 m_WorldMatrix;

			Arithmetic::Vector3		m_Position;
			Arithmetic::Vector3		m_Scale;
			Arithmetic::Matrix3x3	m_Orientation;

			// The model's name, for debugging
#ifdef ZED_BUILD_DEBUG
			ZED_UCHAR8	m_Name[ 32 ];
			ZED_UCHAR8	**m_ppMeshNames;
			// Basically, the debug shader is a colour-only, non-lit
			// shader
			Shader	*m_pDebugShader;

			ZED_BOOL	m_RenderWireframe;
			ZED_BOOL	m_RenderVertexNormals;
			ZED_BOOL	m_RenderFaceNormals;
			ZED_BOOL	m_RenderBones;

#ifdef ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86
			GLuint		m_DebugVertexArrayID[ 4 ];
			GLuint		m_DebugVBO[ 4 ];
#endif

			// Data for the different debug colours
			ZED_FLOAT32	*m_pDebugWireColour;
			ZED_FLOAT32	*m_pDebugBoneColour;
			ZED_FLOAT32	*m_pDebugFaceNormalColour;
			ZED_FLOAT32	*m_pDebugVertNormalColour;

			// The wireframe indices to use
			ZED_UINT16	*m_pDebugWireframeIndices;

			ZED_COLOUR	m_WireColour;
#endif

			// VERY OPENGL SPECIFIC, RECTIFY!
#ifdef ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86
			GLuint m_pVertexArrayID[ 1 ];
			GLuint m_VertexBufferObjectID[ 3 ];
#endif
#ifdef ZED_PLATFORM_XBOX
		public:
			IDirect3DVertexBuffer8	*m_pVertexBuffer;
			IDirect3DIndexBuffer8	*m_pIndexBuffer;
#endif
		};
	}
}

#endif
