#ifndef __ZED_RENDERER_GLMODEL_HPP__
#define __ZED_RENDERER_GLMODEL_HPP__

#include <Model.hpp>
#include <GLVertexCacheManager.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLModel : public Model
		{
		public:
			GLModel( const Renderer *p_pRenderer );
			~GLModel( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilename );
			virtual void Render( );
			virtual void Update( const ZED_FLOAT32 p_Delta,
				const Arithmetic::Matrix4x4 &p_ViewProjection );

			virtual void SetPosition( const Arithmetic::Vector3 &p_Position );
			virtual void SetScale( const Arithmetic::Vector3 &p_Scale );
			virtual void SetOrientation(
				const Arithmetic::Matrix3x3 &p_Orientation );

#ifdef ZED_BUILD_DEBUG
			virtual void SetWireframeColour( const ZED_COLOUR &p_Colour );

			virtual void ToggleWireframe( );
			virtual void ToggleVertexNormals( );
			virtual void ToggleFaceNormals( );
			virtual void ToggleBones( );
#endif

		protected:
			virtual ZED_UINT32 LoadHeader( );

			virtual ZED_UINT32 LoadMeshes( const ZED_UINT64 p_Size );

			virtual void ReadChunk( ZED_UINT16 &p_Type, ZED_UINT64 &p_Size );

			FILE *m_pFile;
			
			// The vertices are in the same arrangement as the attributes
			ZED_UINT16	**m_ppIndices;
			ZED_BYTE	**m_ppVertices;

			ZED_UINT64	*m_pAttributes;
			ZED_UINT32	*m_pMaterialID;
			ZED_BYTE	*m_pStride;

			ZED_UINT16	*m_pIndexCount;
			ZED_UINT16	*m_pVertexCount;
			ZED_BYTE	m_MeshCount;
			ZED_BYTE	m_JointCount;
			// The flag bits are described in the Renderer documentation
			ZED_UINT32	m_Flags;

			Arithmetic::Matrix4x4 m_World;

			// Use the renderer's vertex cache manager
			Renderer	*m_pRenderer;

#ifdef ZED_BUILD_DEBUG
			ZED_BOOL	m_RenderWireframe;
			ZED_BOOL	m_RenderVNormals;
			ZED_BOOL	m_RenderFNormals;
			ZED_BOOL	m_RenderBones;

			ZED_COLOUR	m_MeshWireColour;
			ZED_COLOUR	m_VNormalColour;
			ZED_COLOUR	m_FNormalColour;
			ZED_COLOUR	m_BoneColour;
#endif
			ZED_INLINE GLModel operator=( const GLModel &p_Clone ){ }
		};
	}
}

#endif

