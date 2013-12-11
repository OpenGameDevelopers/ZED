#ifndef __ZED_RENDERER_MESH_HPP__
#define __ZED_RENDERER_MESH_HPP__

#include <Renderer/ModelStructs.hpp>
#include <Arithmetic/Vector2.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Renderer/Shader.hpp>
#include <Arithmetic/Matrix3x3.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <cstring>
#include <Arithmetic/AABB.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Mesh
		{
		public:
			ZED_EXPLICIT Mesh( );

			ZED_EXPLICIT Mesh( const ZED_UINT32 p_StripCount,
				const ZED_UINT32 p_ListCount,
				const ZED_UINT32 p_FanCount );

			~Mesh( );

			ZED_UINT32 SetVertices( const ZED_BYTE *p_pVertices,
				const ZED_UINT32 p_VertexCount );

			ZED_BYTE *GetVertices( ) const;

			ZED_UINT32 SetStripIndices( const ZED_UINT16 *p_pStrip,
				const ZED_MEMSIZE p_Index, const ZED_UINT32 p_Count );

			ZED_UINT32 SetListIndices( const ZED_UINT16 *p_pList,
				const ZED_MEMSIZE p_Index, const ZED_UINT32 p_Count );
			
			ZED_UINT32 SetFanIndices( const ZED_UINT16 *p_pFan,
				const ZED_MEMSIZE p_Index, const ZED_UINT32 p_Count );

			ZED_UINT32 GetStripIndices( const ZED_MEMSIZE p_Index,
				ZED_UINT16 **p_ppStripIndices ) const;

			ZED_UINT32 GetListIndices( const ZED_MEMSIZE p_Index,
				ZED_UINT16 **p_ppListIndices ) const;

			ZED_UINT32 GetFanIndices( const ZED_MEMSIZE p_Index,
				ZED_UINT16 **p_ppFanIndices ) const;

			ZED_UINT32 CopyStripIndices( const ZED_MEMSIZE p_Index,
				ZED_UINT16 **p_ppStripIndices ) const;
			
			ZED_UINT32 CopyListIndices( const ZED_MEMSIZE p_Index,
				ZED_UINT16 **p_ppListIndices ) const;

			ZED_UINT32 CopyFanIndices( const ZED_MEMSIZE p_Index,
				ZED_UINT16 **p_ppFanIndices ) const;

			void SetStripCount( const ZED_UINT32 p_StripCount );

			void SetListCount( const ZED_UINT32 p_ListCount );
			
			void SetFanCount( const ZED_UINT32 p_FanCount );

			void CalculateBoundingBox( );

			ZED_UINT32 GetBoundingBox(
				ZED::Arithmetic::AABB *p_pBoundingBox ) const;

			ZED_UINT32 GetStripCount( ) const;
			ZED_UINT32 GetListCount( ) const;
			ZED_UINT32 GetFanCount( ) const;

			ZED_UINT32 GetStripIndexCount( const ZED_MEMSIZE p_Index ) const;
			ZED_UINT32 GetListIndexCount(const ZED_MEMSIZE p_Index ) const;
			ZED_UINT32 GetFanIndexCount( const ZED_MEMSIZE p_Index ) const;

			ZED_UINT32 GetVertexCount( ) const;
			void SetAttributes( const ZED_UINT64 p_Attributes );
			ZED_UINT64 GetAttributes( ) const;
			void SetMaterialID( const ZED_UINT32 p_MaterialID );
			ZED_UINT32 GetMaterialID( ) const;

		private:
			// Indices
			ZED_UINT16				**m_ppStripIndices;
			ZED_UINT16				**m_ppListIndices;
			ZED_UINT16				**m_ppFanIndices;
			// Type count
			ZED_UINT32				m_StripCount;
			ZED_UINT32				m_ListCount;
			ZED_UINT32				m_FanCount;
			// Index count
			ZED_UINT32				*m_pStripIndexCount;
			ZED_UINT32				*m_pListIndexCount;
			ZED_UINT32				*m_pFanIndexCount;

			ZED_BYTE				*m_pVertices;
			ZED_UINT32				m_VertexCount;
			ZED_UINT64				m_Attributes;
			ZED_UINT32				m_MaterialID;
			ZED::Arithmetic::AABB	m_BoundingBox;
		};
	}
}

#endif // __ZED_RENDERER_MESH_HPP__

