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
			virtual ~Model( ){ }

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilename ) = 0;
			virtual void Render( ) = 0;
			virtual void Update( const ZED_FLOAT32 p_Delta,
				const Arithmetic::Matrix4x4 &p_ViewProjection ) = 0;

			virtual void SetPosition(
				const Arithmetic::Vector3 &p_Position ) = 0;
			virtual void SetScale( const Arithmetic::Vector3 &p_Scale ) = 0;
			virtual void SetOrientation(
				const Arithmetic::Quaternion &p_Orientation ) = 0;

#ifdef ZED_BUILD_DEBUG
			virtual void SetWireframeColour( const ZED_COLOUR &p_Colour ) = 0;

			virtual void ToggleWireframe( ) = 0;
			virtual void ToggleBones( ) = 0;
			virtual void ToggleFaceNormals( ) = 0;
			virtual void ToggleVertexNormals( ) = 0;
#endif

		protected:
			// Make sure this is a ZED Model file
			virtual ZED_UINT32 LoadHeader( ) = 0;

			// Loads in the meshes.  Each mesh has its own vertices and
			// indices.
			virtual ZED_UINT32 LoadMeshes( const ZED_UINT64 p_Size ) = 0;

			// Animations are documented in the Renderer SDK.  Basically,
			// they're comprised of the bind pose + sampled animations.
			virtual ZED_UINT32 LoadAnimation( const ZED_UINT64 p_Size ) = 0;

			// Little helper!
			virtual void ReadChunk( ZED_UINT16 &p_Type,	
				ZED_UINT64 &p_Size ) = 0;
		};
	}
}

#endif

