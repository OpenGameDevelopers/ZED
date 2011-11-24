#include <Model.hpp>
#include <xgraphics.h>

namespace ZED
{
	namespace Renderer
	{
		Model::Model( )
		{
		}

		Model::~Model( )
		{
		}

		ZED_UINT32 Model::Load( const char *p_pFilename )
		{
			HANDLE ModelFile = ZED_NULL;

			ModelFile = CreateFile( p_pFilename, GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, ZED_NULL );

			if( ModelFile == ZED_NULL )
			{
				zedTrace( "Failed to load model: %s\n", p_pFilename );
				return ZED_FAIL;
			}

			// TEMP!
			// Don't care about the header's correctness
			SetFilePointer( ModelFile, 87, ZED_NULL, FILE_CURRENT );
			// !TEMP

			// Get the amount of vertices to process
			ZED_UINT64 RawVertCount = 0;
			DWORD ReadIn = 0;
			ReadFile( ModelFile, &RawVertCount, sizeof( ZED_UINT64 ), &ReadIn, NULL );

            ZED_UINT64 VertCount = ( ( RawVertCount/4 )/3 )/2;

			m_pVertexData = ( LPVERTEX_V1 )malloc( VertCount*sizeof( VERTEX_V1 ) );/*XPhysicalAlloc(
				VertCount*sizeof( VERTEX_V1 ),
				MAXULONG_PTR, 0, PAGE_READWRITE );*/

			ReadFile( ModelFile, m_pVertexData, VertCount*sizeof( VERTEX_V1 ),
				&ReadIn, NULL );
			
			if( ReadIn != VertCount*sizeof( VERTEX_V1 ) )
			{
				zedTrace( "[ERROR] Failed to read the vertex data for %s\n",
					p_pFilename );
				return ZED_FAIL;
			}

			// Skip over the indices and face normal header
			SetFilePointer( ModelFile, 12, ZED_NULL, FILE_CURRENT );

			// NOT WORKING!!
			ZED_UINT64 RawFaceCount = 0; // Should be 268272 for GraveGuy.zed
			ReadFile( ModelFile, &RawFaceCount, sizeof( ZED_UINT64 ), &ReadIn, NULL );

			ZED_UINT64 FaceCount = ( RawFaceCount/12 )/3;

			m_pFaceData = ( LPFACE_V1 )malloc( sizeof( LPFACE_V1 )*FaceCount );/*XPhysicalAlloc(
				sizeof( ZED_UINT16 )*FaceCount, MAXULONG_PTR,
				MAXULONG_PTR, PAGE_READWRITE );*/

			ReadFile( ModelFile, m_pFaceData, sizeof( FACE_V1 )*FaceCount, &ReadIn, NULL );

			if( ReadIn != sizeof( FACE_V1 )*FaceCount )
			{
				zedTrace( "[ERROR] Failed to read the face data for %s\n",
					p_pFilename );
			}

			CloseHandle( ModelFile );

			// Set up the index and vertex buffers
			m_pVertexBuffer = new IDirect3DVertexBuffer8( );
			m_pIndexBuffer = new IDirect3DIndexBuffer8( );

			XGSetVertexBufferHeader( 0, 0, 0, 0, m_pVertexBuffer, 0 );
			void *pVertData = XPhysicalAlloc( RawVertCount, MAXULONG_PTR, MAXULONG_PTR, PAGE_READWRITE );
			m_pVertexBuffer->Register( pVertData );
			memcpy( pVertData, m_pVertexData, RawVertCount );

			XGSetIndexBufferHeader( 0, 0, D3DFMT_INDEX16, 0, m_pIndexBuffer, 0 );

			return ZED_OK;
		}
	}
}
