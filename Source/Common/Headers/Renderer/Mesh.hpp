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

			void StripCount( const ZED_UINT32 p_StripCount )
			{
			/*	if( m_ppStrips )
				{
					for( ZED_MEMSIZE i = 0; i < m_StripCount; ++i )
					{
						delete [ ] m_ppStrips[ i ];
						m_ppStrips[ i ] = ZED_NULL;
					}
					delete [ ] m_ppStrips;
					m_ppStrips = ZED_NULL;
				}

				if( m_pStripIndexCount )
				{
					delete [ ] m_pStripIndexCount;
					m_pStripIndexCount = ZED_NULL;
				}

				m_ppStrips = new ZED_UINT16*[ p_StripCount ];
				m_pStripIndexCount = new ZED_UINT32[ p_StripCount ];
				m_StripCount = p_StripCount;*/
			}

			void ListCount( const ZED_UINT32 p_ListCount )
			{
			/*	if( m_ppLists )
				{
					for( ZED_MEMSIZE i = 0; i < m_ListCount; ++i )
					{
						delete [ ] m_ppLists[ i ];
						m_ppLists[ i ] = ZED_NULL;
					}
					delete [ ] m_ppLists;
					m_ppLists = ZED_NULL;
				}

				if( m_pListIndexCount )
				{
					delete [ ] m_pListIndexCount;
					m_pListIndexCount = ZED_NULL;
				}

				m_ppLists = new ZED_UINT16*[ p_ListCount ];
				m_pListIndexCount = new ZED_UINT32[ p_ListCount ];
				m_ListCount = p_ListCount;*/
			}

			void FanCount( const ZED_UINT32 p_FanCount )
			{/*
				if( m_ppFans )
				{
					for( ZED_MEMSIZE i = 0; i < m_FanCount; ++i )
					{
						delete [ ] m_ppFans[ i ];
						m_ppFans[ i ] = ZED_NULL;
					}
					delete [ ] m_ppFans;
					m_ppFans = ZED_NULL;
				}

				if( m_pFanIndexCount )
				{
					delete [ ] m_pFanIndexCount;
					m_pFanIndexCount = ZED_NULL;
				}

				m_ppFans = new ZED_UINT16*[ p_FanCount ];
				m_pFanIndexCount = new ZED_UINT32[ p_FanCount ];
				m_FanCount = p_FanCount;*/
			}

			void CalculateBoundingBox( )
			{
				if( m_VertexCount < 4 )
				{
					return;
				}

				VERTEX_V2 Vertices[ m_VertexCount ];

				memcpy( Vertices, m_pVertices,
					sizeof( VERTEX_V2 )*m_VertexCount );

				ZED::Arithmetic::Vector3 Min(
					Vertices[ 0 ].Position[ 0 ],
					Vertices[ 0 ].Position[ 1 ],
					Vertices[ 0 ].Position[ 2 ] );

				ZED::Arithmetic::Vector3 Max(
					Vertices[ 0 ].Position[ 0 ],
					Vertices[ 0 ].Position[ 1 ],
					Vertices[ 0 ].Position[ 2 ] );

				for( ZED_UINT32 i = 1; i < m_VertexCount; ++i )
				{
					if( Vertices[ i ].Position[ 0 ] > Max[ 0 ] )
					{
						Max[ 0 ] = Vertices[ i ].Position[ 0 ];
					}
					if( Vertices[ i ].Position[ 0 ] < Min[ 0 ] )
					{
						Min[ 0 ] = Vertices[ i ].Position[ 0 ];
					}
					if( Vertices[ i ].Position[ 1 ] > Max[ 1 ] )
					{
						Max[ 1 ] = Vertices[ i ].Position[ 1 ];
					}
					if( Vertices[ i ].Position[ 1 ] < Min[ 1 ] )
					{
						Min[ 1 ] = Vertices[ i ].Position[ 1 ];
					}
					if( Vertices[ i ].Position[ 2 ] > Max[ 2 ] )
					{
						Max[ 2 ] = Vertices[ i ].Position[ 2 ];
					}
					if( Vertices[ i ].Position[ 2 ] < Min[ 2 ] )
					{
						Min[ 2 ] = Vertices[ i ].Position[ 2 ];
					}
				}

				m_BoundingBox.Min( Min );
				m_BoundingBox.Max( Max );
			}

			void BoundingBox( ZED::Arithmetic::AABB *p_pBoundingBox )
			{
				( *p_pBoundingBox ) = m_BoundingBox;
				ZED::Arithmetic::Vector3 Min, Max;
				m_BoundingBox.Min( &Min );
				m_BoundingBox.Max( &Max );
				zedTrace( "\tMinimum: %f | %f | %f\n", Min[ 0 ], Min[ 1 ],
					Min[ 2 ] );
				zedTrace( "\tMaximum: %f | %f | %f\n", Max[ 0 ], Max[ 1 ],
					Max[ 2 ] );
			}

			ZED_INLINE ZED_UINT32 GetStripCount( ) const { return m_StripCount; }
			ZED_INLINE ZED_UINT32 GetStripIndexCount(
				const ZED_MEMSIZE p_Index ) const
					{ return m_pStripIndexCount[ p_Index ]; }
			ZED_INLINE ZED_UINT32 GetListCount( ) const { return m_ListCount; }
			ZED_INLINE ZED_UINT32 GetListIndexCount(
				const ZED_MEMSIZE p_Index ) const 
					{ return m_pListIndexCount[ p_Index ]; }
			ZED_INLINE ZED_UINT32 GetFanCount( ) const { return m_FanCount; }
			ZED_INLINE ZED_UINT32 GetFanIndexCount(
				const ZED_MEMSIZE p_Index ) const
					{ return m_pFanIndexCount[ p_Index ]; }
			ZED_INLINE ZED_UINT32 GetVertexCount( ) const
				{ return m_VertexCount; }
			ZED_INLINE void SetAttributes( const ZED_UINT64 p_Attributes )
				{ m_Attributes = p_Attributes; }
			ZED_INLINE ZED_UINT64 SetAttributes( ) const { return m_Attributes; }
			ZED_INLINE void SetMaterialID( const ZED_UINT32 p_MaterialID )
				{ m_MaterialID = p_MaterialID; }
			ZED_INLINE ZED_UINT32 GetMaterialID( ) const { return m_MaterialID; }

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

