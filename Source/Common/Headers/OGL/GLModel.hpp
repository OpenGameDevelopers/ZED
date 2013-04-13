#ifndef __ZED_RENDERER_OGL_GLMODEL_HPP__
#define __ZED_RENDERER_OGL_GLMODEL_HPP__

#include <Model.hpp>
#include <OGL/GLVertexCacheManager.hpp>
#include <Quaternion.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLModel : public Model
		{
		public:
			/**
				\brief GLModel constructor

				GL Models need to use a Renderer to ship the vertices down to
				the GPU.
			*/
			GLModel( const Renderer *p_pRenderer );
			~GLModel( );

			/**
				\brief Loads the file passed in

				When the model is loaded, the buffers are populated for
				processing and shipping to the GPU.
			*/
			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilename );

			/**
				\brief Sends the buffer's contents to the GPU

				Calls m_pRenderer's Render function, which passes through the
				Vertex Cache Manager.
			*/
			virtual void Render( );
			/**
				\brief Any animations currently in motion are */
			virtual void Update( const ZED_FLOAT32 p_Delta,
				const Arithmetic::Matrix4x4 &p_ViewProjection );

			/**
				\brief Sets the world-relative position of the model
			*/
			virtual void SetPosition( const Arithmetic::Vector3 &p_Position );
			/**
				\brief Non-uniform scaling of the model
			*/
			virtual void SetScale( const Arithmetic::Vector3 &p_Scale );
			/**
				\brief Sets the model's current orientation
			*/
			virtual void SetOrientation(
				const Arithmetic::Quaternion &p_Orientation );

#ifdef ZED_BUILD_DEBUG
			/**
				\brief [DEBUG] Set the colour for the wireframe representation

				For OpenGL versions that support shaders, the wireframe view
				will be implemented in a shader.  Otherwise, the OpenGL
				state machine will be used for setting the shading mode.
			*/
			virtual void SetWireframeColour( const ZED_COLOUR &p_Colour );

			/**
				\brief [DEBUG] Show/Hide the wireframe of the mesh.
			*/
			virtual void ToggleWireframe( );
			/**
				\brief [DEBUG] Show/Hide the normals for each vertex.
			*/
			virtual void ToggleVertexNormals( );
			/**
				\brief [DEBUG] Show/Hide the normals for each face.
			*/
			virtual void ToggleFaceNormals( );
			/**
				\brief [DEBUG] Show/Hide the bones.
			*/
			virtual void ToggleBones( );
#endif

		protected:
			/**
				\brief A helper method for loading the model header.
			*/
			virtual ZED_UINT32 LoadHeader( );

			/**
				\brief A helper method for loading mesh data.
			*/
			virtual ZED_UINT32 LoadMeshes( const ZED_UINT64 p_Size );

			/**
				\brief A helper method for loading animation data.
			*/
			virtual ZED_UINT32 LoadAnimation( const ZED_UINT64 p_Size );

			/**
				\brief A helper method for reading model chunks.
			*/
			virtual void ReadChunk( ZED_UINT16 &p_Type, ZED_UINT64 &p_Size );

			FILE *m_pFile;
			
			// In case the model is saved in big endian and loaded in little
			// endian or vise versa.
			ZED_BOOL m_SwapBytes;

			ZED_UINT32	m_MeshCount;
			Mesh		*m_pMesh;

			// The animation data
			Arithmetic::Vector3 	*m_pJointBindPosition;
			Arithmetic::Quaternion 	*m_pJointBindOrientation;
			
			// The total joints in the skeleton and the array of parents for
			// each joint
			ZED_BYTE	m_JointCount;
			ZED_BYTE	*m_pJointParents;
			
			// The flag bits are described in the Renderer documentation
			ZED_UINT32	m_Flags;

			Arithmetic::Matrix4x4	m_World;
			Arithmetic::Vector3		m_Position;
			Arithmetic::Vector3		m_Scale;
			Arithmetic::Quaternion	m_Orientation;

			LPJOINT_V2	m_pJoints;

			// Use the renderer's vertex cache manager
			Renderer	*m_pRenderer;

#ifdef ZED_BUILD_DEBUG
			// Render various debug constructs
			ZED_BOOL	m_RenderWireframe;
			ZED_BOOL	m_RenderVNormals;
			ZED_BOOL	m_RenderFNormals;
			ZED_BOOL	m_RenderBones;

			// The colours to use for the various debug constructs
			ZED_COLOUR	m_MeshWireColour;
			ZED_COLOUR	m_VNormalColour;
			ZED_COLOUR	m_FNormalColour;
			ZED_COLOUR	m_BoneColour;

			// Debug mesh data
			ZED_MEMSIZE m_BoneVCount;
			ZED_MEMSIZE m_BoneICount;
			ZED_BYTE	*m_pBoneVertices;
			ZED_UINT16	*m_pBoneIndices;
#endif

			// Disable implicit copy and clone
			GLModel( const GLModel &p_Copy ){ }
			GLModel &operator=( const GLModel &p_Clone ){ return *this; }
		};
	}
}

#endif

