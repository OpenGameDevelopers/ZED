#include <XboxRenderer.hpp>
#include <Debugger.hpp>
#include <Matrix3x3.hpp>

namespace ZED
{
	namespace Renderer
	{
		ZED_MEMSIZE GetBPP( const ZED_FORMAT p_Format )
		{
			switch( p_Format )
			{
			case ZED_FORMAT_RGB565:
			case ZED_FORMAT_ARGB1555:
				return 16;
			case ZED_FORMAT_ARGB8:
				return 32;
			default:
				return 0;
			}
		}

		ZED_UCHAR8 *FormatToString( const ZED_FORMAT p_Format )
		{
			switch( p_Format )
			{
			case ZED_FORMAT_RGB565:
				return ( ZED_UCHAR8 * )"ZED_FORMAT_RGB565";
			case ZED_FORMAT_ARGB8:
				return ( ZED_UCHAR8 * )"ZED_FORMAT_ARGB8";
			default:
				return ( ZED_UCHAR8 * )"UNKNOWN";
			}
		}

		char *FlagsToString( const ZED_UINT32 p_Flags )
		{
			char pFlags[ 1024 ] = { '\0' };

			switch( p_Flags )
			{
			// D3D Presentation Params flags
			case D3DPRESENTFLAG_WIDESCREEN:
				strcat( pFlags, "D3DPRESENTFLAG_WIDESCREEN " );
			case D3DPRESENTFLAG_INTERLACED:
				strcat( pFlags, "D3DPRESENTFLAG_INTERLACED " );
			case D3DPRESENTFLAG_PROGRESSIVE:
				strcat( pFlags, "D3DPRESENTFLAG_PROGRESSIVE " );
				break;
			default:
				break;
			}

			return pFlags;
		}

		XboxRenderer::XboxRenderer( )
		{
			// Just set everything to zero
			m_pDevice = NULL;
			m_pD3D = NULL;

			memset( &m_Canvas, 0, sizeof( CanvasDescription ) );
			memset( &m_PresentParams, 0, sizeof( D3DPRESENT_PARAMETERS ) );
			m_AspectRatio = 0.0f;
			m_Width = m_Height = 0;
		}

		XboxRenderer::~XboxRenderer( )
		{
			Release( );
		}

		ZED_UINT32 XboxRenderer::Create( GraphicsAdapter *p_Adapter,
			const CanvasDescription &p_Canvas )
		{
			ZED_UINT32 ReturnStatus = ZED_FAIL;

			if( NULL ==( m_pD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
			{
				return ZED_FAIL;
			}

			m_Canvas = p_Canvas;

			m_PresentParams.BackBufferWidth = m_Canvas.GetWidth( );
			m_PresentParams.BackBufferHeight = m_Canvas.GetHeight( );
			m_PresentParams.BackBufferCount = m_Canvas.GetBackBufferCount( );
			m_PresentParams.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

			// Get the colour
			switch( m_Canvas.GetBPP( ) )
			{
			case ZED_FORMAT_ARGB8:
				{
					m_PresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
					ReturnStatus = ZED_OK;
					break;
				}
			default:
				{
					zedTrace( "Failed to set the back buffer format" );
					return ZED_FAIL;
				}
			}

			// Get the Depth/Stencil
			switch( m_Canvas.GetDepthStencil( ) )
			{
			case ZED_FORMAT_D24S8:
				{
					m_PresentParams.EnableAutoDepthStencil = ZED_TRUE;
					m_PresentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
					ReturnStatus = ZED_OK;
					break;
				}
			default:
				{
					zedTrace( "Failed to set the depth/stencil format" );
					return ZED_FAIL;
				}
			}

			// This needs to be exposed, but how?
			m_PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

			if( FAILED( m_pD3D->CreateDevice( 0, D3DDEVTYPE_HAL, NULL,
				D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_PresentParams,
				&m_pDevice ) ) )
			{
				zedTrace( "Failed to create device" );
				return ZED_FAIL;
			}

#ifdef ZED_BUILD_DEBUG
			ZED_UINT32 ModeCount = m_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT );
			D3DDISPLAYMODE *pModes = new D3DDISPLAYMODE[ ModeCount ];
			for( ZED_MEMSIZE i = 0; i < ModeCount; i++ )
			{
				if( FAILED( m_pD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, i,
					&pModes[ i ] ) ) )
				{
					delete pModes;
					return ZED_FAIL;
				}

				ZED_MEMSIZE Len = strlen( FlagsToString( pModes[ i ].Flags ) );
				char *pFlags;
			
				if( Len > 0 )
				{
					pFlags = new char[ Len ];
					strncpy( pFlags, FlagsToString( pModes[ i ].Flags ), Len-1 );
					pFlags[ Len-1 ] = '\0';
				}
				if( Len == 0 )
				{
					// Add one for null-termination
					Len = strlen( "NO FLAGS" )+1;
					pFlags = new char[ Len ];
					strncpy( pFlags, "NO FLAGS", Len );
				}	

				// Print out the adapter's capabilities
				zedTrace( "Mode %d:\n"
					"\tWidth: %d | Height: %d | Refresh Rate: %d | Flags: %s \n",
					i,
					pModes[ i ].Width, pModes[ i ].Height,
					pModes[ i ].RefreshRate,
					pFlags );

				delete pFlags;
			}
#endif

			return ReturnStatus;
		}

		void XboxRenderer::ForceClear( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth,
			const ZED_BOOL p_Stencil )
		{
			ZED_DWORD Flags = 0;

			// D3D TARGET =		1111 0000b (XBOX)
			// D3D ZBUFFER =	0000 0010b
			// D3D STENCIL =	0000 0100b

			Flags |= p_Colour;
			Flags |= ( p_Depth << 1 );
			Flags |= ( p_Stencil << 3 );
/*
			if( p_Colour == ZED_TRUE )
			{
				Flags |= D3DCLEAR_TARGET;
			}

			if( p_Depth == ZED_TRUE )
			{
				Flags |= D3DCLEAR_ZBUFFER;
			}

			if( p_Stencil == ZED_TRUE )
			{
				Flags |= D3DCLEAR_STENCIL;
			}
*/
			m_pDevice->Clear( 0L, NULL, Flags, m_Colour, 1.0f, 0L );
		}

		void XboxRenderer::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green,
			const ZED_FLOAT32 p_Blue )
		{
			m_Colour = D3DCOLOR_COLORVALUE( p_Red, p_Green, p_Blue, 1.0f );
		}

		ZED_UINT32 XboxRenderer::BeginScene( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth,
			const ZED_BOOL p_Stencil )
		{
			ZED_UINT32 ReturnStatus = ZED_OK;

			ZED_DWORD Flags = 0;
/*
			Flags |= p_Colour;
			Flags |= ( p_Depth << 1 );
			Flags |= ( p_Stencil << 2 );*/

			if( p_Colour == ZED_TRUE )
			{
				Flags |= D3DCLEAR_TARGET;
			}

			if( p_Depth == ZED_TRUE )
			{
				Flags |= D3DCLEAR_ZBUFFER;
			}

			if( p_Stencil == ZED_TRUE )
			{
				Flags |= D3DCLEAR_STENCIL;
			}

			m_pDevice->Clear( 0L, NULL, Flags, m_Colour, 1.0f, 0L );

			return ReturnStatus;
		}

		void XboxRenderer::EndScene( )
		{
			m_pDevice->Swap( D3DSWAP_DEFAULT );
		}

		ZED_BOOL XboxRenderer::ToggleFullscreen( )
		{
			// This is an Xbox... =P
			return ZED_TRUE;
		}

		ZED_UINT32 XboxRenderer::ResizeCanvas( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			ZED_UINT32 ReturnStatus = ZED_OK;

			m_Width = p_Width;
			m_Height = p_Height;
			m_AspectRatio = static_cast< float >( p_Width / p_Height );

			m_PresentParams.BackBufferWidth = p_Width;
			m_PresentParams.BackBufferHeight = p_Height;

			// While the Xbox uses a fixed resolution, it may be beneficial for
			// debugging purposes
			if( FAILED( m_pDevice->Reset( &m_PresentParams ) ) )
			{
				ReturnStatus = ZED_FAIL;
			}

			return ReturnStatus;
		}

		void XboxRenderer::Release( )
		{
			// Even though it is an Xbox, we'll be polite about this...
			if( m_pDevice != NULL )
			{
				m_pDevice->Release( );
				m_pDevice = NULL;
			}

			if( m_pD3D != NULL )
			{
				m_pD3D->Release( );
				m_pD3D = NULL;
			}
		}

		void XboxRenderer::SetView3D( const Arithmetic::Vector3 &p_Right,
			const Arithmetic::Vector3 &p_Up,
			const Arithmetic::Vector3 &p_Dir,
			const Arithmetic::Vector3 &p_Position )
		{
			// R U D P
			// R U D P
			// R U D P
			// 0 0 0 1
			m_View3D( 3, 0 ) = m_View3D( 3, 1 ) = m_View3D( 3, 2 ) = 0.0f;
			m_View3D( 3, 3 ) = 1.0f;

			m_View3D( 0, 0 ) = p_Right[ 0 ];
			m_View3D( 1, 0 ) = p_Right[ 1 ];
			m_View3D( 2, 0 ) = p_Right[ 2 ];

			m_View3D( 0, 1 ) = p_Up[ 0 ];
			m_View3D( 1, 1 ) = p_Up[ 1 ];
			m_View3D( 2, 1 ) = p_Up[ 2 ];

			m_View3D( 0, 2 ) = p_Dir[ 0 ];
			m_View3D( 1, 2 ) = p_Dir[ 1 ];
			m_View3D( 2, 2 ) = p_Dir[ 2 ];

			m_View3D( 0, 3 ) = p_Position[ 0 ];
			m_View3D( 1, 3 ) = p_Position[ 1 ];
			m_View3D( 2, 3 ) = p_Position[ 2 ];
		}

		void XboxRenderer::SetViewLookAt(
				const Arithmetic::Vector3 &p_Position,
				const Arithmetic::Vector3 &p_Point,
				const Arithmetic::Vector3 &p_WorldUp )
		{
			// Calculate the view vectors
			Arithmetic::Vector3 ViewDir;
			Arithmetic::Vector3 ViewRight;
			Arithmetic::Vector3 ViewUp;

			ViewDir = p_Point - p_Position;
			ViewDir.Normalise( );
			ViewUp =  p_WorldUp - p_WorldUp.Dot( ViewDir )*ViewDir;
			ViewUp.Normalise( );
			ViewRight = ViewDir.Cross( ViewUp );

			// Call SetView3D to handle setting the view matrix
			return SetView3D( ViewRight, ViewUp, ViewDir, p_Position );
		}

		void XboxRenderer::CalcViewProjMatrix( )
		{
			Arithmetic::Matrix4x4 *pMatA;
			Arithmetic::Matrix4x4 *pMatB;

			if( m_Mode == ZED_VIEW_SCREEN )
			{
				pMatA = ( Arithmetic::Matrix4x4 * )&m_ProjectionScreen;
				pMatB = ( Arithmetic::Matrix4x4 * )&m_View2D;
			}
			else
			{
				pMatB = ( Arithmetic::Matrix4x4 * )&m_View3D;
				if( m_Mode == ZED_VIEW_PERSPECTIVE )
				{
					pMatA = ( Arithmetic::Matrix4x4 * ) &(
						m_ProjectionPerspective[ m_Stage ] );
				}
				else
				{
					pMatA = ( Arithmetic::Matrix4x4 * ) &(
						m_ProjectionOrthogonal[ m_Stage ] );
				}
			}

			Arithmetic::Matrix4x4 *pMat =
				( Arithmetic::Matrix4x4 * )&m_ViewProjection;

			( *pMat ).Copy( ( *pMatA )*( *pMatB ) );
		}

		void XboxRenderer::CalcWorldViewProjMatrix( )
		{
			Arithmetic::Matrix4x4 *pProjection;
			Arithmetic::Matrix4x4 *pView;
			Arithmetic::Matrix4x4 *pWorld;

			pWorld = ( Arithmetic::Matrix4x4 * )&m_World;

			if( m_Mode == ZED_VIEW_SCREEN )
			{
				pProjection = ( Arithmetic::Matrix4x4 * )&m_ProjectionScreen;
				pView = ( Arithmetic::Matrix4x4 * )&m_View2D;
			}
			else
			{
				pView = ( Arithmetic::Matrix4x4 * )&m_View3D;

				if( m_Mode == ZED_VIEW_PERSPECTIVE )
				{
					pProjection = ( Arithmetic::Matrix4x4 * )&(
						m_ProjectionPerspective[ m_Stage ] );
				}
				else
				{
					pProjection = ( Arithmetic::Matrix4x4 * )&(
						m_ProjectionOrthogonal[ m_Stage ] );
				}
			}

			Arithmetic::Matrix4x4 *pMatrix =
				( Arithmetic::Matrix4x4 * )&m_WorldViewProjection;

			( *pMatrix ).Copy( ( ( *pWorld )*( *pView ) )*( *pProjection ) );
		}

		void XboxRenderer::SetClippingPlanes( const ZED_FLOAT32 p_Near,
			const ZED_FLOAT32 p_Far )
		{
			m_Near = p_Near;
			m_Far = p_Far;

			if( m_Near <= ZED_Epsilon )
			{
				m_Near = 0.01f;
			}
			if( m_Far <= 1.0f )
			{
				m_Far = 1.0f;
			}

			if( m_Near >= m_Far )
			{
				m_Near = m_Far;
				m_Far = m_Near + 1.0f;
			}

			Prepare2D( );

			// Adjust orthogonal projection
			ZED_FLOAT32 FarFactor = 1.0f / ( m_Far - m_Near );
			ZED_FLOAT32 NearFactor = -FarFactor*m_Near;

			m_ProjectionOrthogonal[ 0 ]( 2, 2 ) =
				m_ProjectionOrthogonal[ 1 ]( 2, 2 ) =
				m_ProjectionOrthogonal[ 2 ]( 2, 2 ) =
				m_ProjectionOrthogonal[ 3 ]( 2, 2 ) = FarFactor;
			m_ProjectionOrthogonal[ 0 ]( 3, 2 ) =
				m_ProjectionOrthogonal[ 1 ]( 3, 2 ) =
				m_ProjectionOrthogonal[ 2 ]( 3, 2 ) =
				m_ProjectionOrthogonal[ 3 ]( 3, 2 ) = NearFactor;

			// Re-assign the near and far factors for perspective
			FarFactor *= m_Far;
			NearFactor = -FarFactor*m_Near;

			m_ProjectionPerspective[ 0 ]( 2, 2 ) =
				m_ProjectionPerspective[ 1 ]( 2, 2 ) =
				m_ProjectionPerspective[ 2 ]( 2, 2 ) =
				m_ProjectionPerspective[ 3 ]( 2, 2 ) = FarFactor;
			m_ProjectionPerspective[ 0 ]( 3, 2 ) =
				m_ProjectionPerspective[ 1 ]( 3, 2 ) =
				m_ProjectionPerspective[ 2 ]( 3, 2 ) =
				m_ProjectionPerspective[ 3 ]( 3, 2 ) = NearFactor;
		}

		void XboxRenderer::Prepare2D( )
		{
			// Make the 2D projection matrix an identity matrix
			m_ProjectionScreen.Identity( );
			m_ViewScreen.Identity( );

			m_ProjectionScreen( 0, 0 ) =
				2.0f/static_cast< ZED_FLOAT32 >( m_Canvas.GetWidth( ) );
			m_ProjectionScreen( 1, 1 ) =
				2.0f/static_cast< ZED_FLOAT32 >( m_Canvas.GetHeight( ) );
			m_ProjectionScreen( 2, 2 ) =
				1.0f/( m_Far-m_Near );
			m_ProjectionScreen( 3, 2 ) =
				-m_Near*( 1.0f/( m_Far-m_Near ) );

			// 2D view matrix
			ZED_FLOAT32 TX, TY, TZ;
			TX = -( static_cast< ZED_INT32 >( m_Canvas.GetWidth( ) ) ) +
				( m_Canvas.GetWidth( )*0.5f );
			TY = m_Canvas.GetHeight( ) - ( m_Canvas.GetHeight( )*0.5f );
			TZ = m_Near + 0.1f;

			m_View2D( 1, 1 ) = -1.0f;
			m_View2D( 0, 3 ) = TX;
			m_View2D( 1, 3 ) = TY;
			m_View2D( 2, 3 ) = TZ;
		}

		ZED_UINT32 XboxRenderer::CalcPerspProjMatrix( const ZED_FLOAT32 p_FOV,
			const ZED_FLOAT32 p_AspectRatio, Arithmetic::Matrix4x4 *p_pMatrix )
		{
			if( Arithmetic::Absolute( m_Far - m_Near ) < ZED_Epsilon )
			{
				return ZED_FAIL;
			}

			ZED_FLOAT32 SinFOV2, CosFOV2;
			Arithmetic::SinCos( ( p_FOV / 2.0f ), SinFOV2, CosFOV2 );

			if( Arithmetic::Absolute( SinFOV2 ) < ZED_Epsilon )
			{
				return ZED_FAIL;
			}

			ZED_FLOAT32 Width = p_AspectRatio * ( CosFOV2 / SinFOV2 );
			ZED_FLOAT32 Height = 1.0f * ( CosFOV2 / SinFOV2 );
			ZED_FLOAT32 Q = m_Far / ( m_Far - m_Near );

			memset( p_pMatrix, 0, sizeof( Arithmetic::Matrix4x4 ) );

			( *p_pMatrix )( 0, 0 ) = Width;
			( *p_pMatrix )( 1, 1 ) = Height;
			( *p_pMatrix )( 2, 2 ) = Q;
			( *p_pMatrix )( 3, 2 ) = 1.0f;
			( *p_pMatrix )( 2, 3 ) = -Q*m_Near;

			return ZED_OK;
		}

		ZED_UINT32 XboxRenderer::SetMode( const ZED_UINT32 p_Stage,
			const ZED_VIEWMODE p_Mode )
		{
			D3DVIEWPORT8 Viewport;

			if( !m_Running )
			{
				return ZED_FAIL;
			}

			if( ( m_Stage > 3 ) || ( m_Stage < 0 ) )
			{
				m_Stage = 0;
			}

			if( m_Mode != p_Mode )
			{
				m_Mode = p_Mode;
			}

			// Flush all Vertex Caches before the mode switch makes things
			// not work
			//UNCOMMENTm_pVertexManager->ForceFlushAll( );

			// If the mode is in screen space, use its matrices instead
			if( p_Mode == ZED_VIEW_SCREEN )
			{
				Viewport.X = 0;
				Viewport.Y = 0;
				Viewport.Width = m_Canvas.GetWidth( );
				Viewport.Height = m_Canvas.GetHeight( );
				Viewport.MinZ = 0.0f;
				Viewport.MaxZ = 1.0f;

				if( FAILED( m_pDevice->SetViewport( &Viewport ) ) )
				{
					return ZED_FAIL;
				}
			}
			// Perspective or orthogonal projection
			else
			{
				m_Stage = p_Stage;

				// Setup the viewport
				Viewport.X = m_Viewport[ p_Stage ].X;
				Viewport.Y = m_Viewport[ p_Stage ].Y;
				Viewport.Width = m_Viewport[ p_Stage ].Width;
				Viewport.Height = m_Viewport[ p_Stage ].Height;
				Viewport.MinZ = 0.0f;
				Viewport.MaxZ = 1.0f;

				if( FAILED( m_pDevice->SetViewport( &Viewport ) ) )
				{
					zedTrace(
						"XboxRenderer::SetMode - Could not set viewport for "
						"stage %d", p_Stage );
					return ZED_FAIL;
				}

				CalcViewProjMatrix( );
				CalcWorldViewProjMatrix( );
			}

			return ZED_OK;
		}
/*
		ZED_UINT32 XboxRenderer::InitStage( const ZED_FLOAT32 p_FOV,
			const ZED_VIEWPORT &p_Viewport, ZED_UINT32 p_Stage )
		{
			ZED_FLOAT32 AspectRatio;

			if( !( &p_Viewport ) )
			{
				ZED_VIEWPORT OwnVP = { 0, 0, m_Canvas.GetWidth( ),
					m_Canvas.GetHeight( ) };
				memcpy( &m_Viewport[ p_Stage ], &OwnVP,
					sizeof( ZED_VIEWPORT ) );
			}
			else
			{
				memcpy( &m_Viewport[ p_Stage ], &p_Viewport,
					sizeof( ZED_VIEWPORT ) );
			}

			if( ( p_Stage > 3 ) || ( p_Stage < 0 ) )
			{
				p_Stage = 0;
			}

			AspectRatio = ( ( ZED_FLOAT32 )( m_Viewport[ p_Stage ].Height ) ) /
				( m_Viewport[ p_Stage ].Width );

			// Perspective projection matrix
			if( FAILED( this->CalcPerspProjMatrix( p_FOV,
				AspectRatio, &m_ProjectionPerspective[ p_Stage ] ) ) )
			{
				return ZED_FAIL;
			}

			// Orthogonal projection matrix
			m_ProjectionOrthogonal[ p_Stage ].Identity( );

			m_ProjectionOrthogonal[ p_Stage ]( 0, 0 ) =
				2.0f/m_Viewport[ p_Stage ].Width;
			m_ProjectionOrthogonal[ p_Stage ]( 1, 1 ) =
				2.0f/m_Viewport[ p_Stage ].Height;
			m_ProjectionOrthogonal[ p_Stage ]( 2, 2 ) =
				1.0f/( m_Far - m_Near );
			m_ProjectionOrthogonal[ p_Stage ]( 3, 2 ) =
				-m_Near * m_ProjectionOrthogonal[ p_Stage ]( 2, 2 );
			m_ProjectionOrthogonal[ p_Stage ]( 3, 3 ) = 1.0f;

			return ZED_OK;
		}

		void XboxRenderer::GetFrustum( Arithmetic::Plane *p_Planes )
		{
			// Left
			p_Planes[ 0 ].Set(
				Arithmetic::Vector3(
					-( m_ViewProjection( 0, 3 ) + m_ViewProjection( 0, 0 ) ),
					-( m_ViewProjection( 1, 3 ) + m_ViewProjection( 1, 0 ) ),
					-( m_ViewProjection( 2, 3 ) + m_ViewProjection( 2, 0 ) ) ),
					-( m_ViewProjection( 3, 3 ) + m_ViewProjection( 3, 0 ) ) );
			// Right
			p_Planes[ 1 ].Set(
				Arithmetic::Vector3(
					-( m_ViewProjection( 0, 3 ) - m_ViewProjection( 0, 0 ) ),
					-( m_ViewProjection( 1, 3 ) - m_ViewProjection( 1, 0 ) ),
					-( m_ViewProjection( 2, 3 ) - m_ViewProjection( 2, 0 ) ) ),
					-( m_ViewProjection( 3, 3 ) - m_ViewProjection( 3, 0 ) ) );
            // Top
			p_Planes[ 2 ].Set(
				Arithmetic::Vector3(
					-( m_ViewProjection( 0, 3 ) - m_ViewProjection( 0, 1 ) ),
					-( m_ViewProjection( 1, 3 ) - m_ViewProjection( 1, 1 ) ),
					-( m_ViewProjection( 2, 3 ) - m_ViewProjection( 2, 1 ) ) ),
					-( m_ViewProjection( 3, 3 ) - m_ViewProjection( 3, 1 ) ) );
			// Bottom
			p_Planes[ 3 ].Set(
				Arithmetic::Vector3(
					-( m_ViewProjection( 0, 3 ) + m_ViewProjection( 0, 1 ) ),
					-( m_ViewProjection( 1, 3 ) + m_ViewProjection( 1, 1 ) ),
					-( m_ViewProjection( 2, 3 ) + m_ViewProjection( 2, 1 ) ) ),
					-( m_ViewProjection( 3, 3 ) + m_ViewProjection( 3, 1 ) ) );
			// Near
			p_Planes[ 4 ].Set(
				Arithmetic::Vector3(
					-( m_ViewProjection( 0, 2 ) ),
					-( m_ViewProjection( 1, 2 ) ),
					-( m_ViewProjection( 2, 2 ) ) ),
					-( m_ViewProjection( 3, 2 ) ) );
			// Far
			p_Planes[ 5 ].Set(
				Arithmetic::Vector3(
					-( m_ViewProjection( 0, 3 ) - m_ViewProjection( 0, 2 ) ),
					-( m_ViewProjection( 1, 3 ) - m_ViewProjection( 1, 2 ) ),
					-( m_ViewProjection( 2, 3 ) - m_ViewProjection( 2, 2 ) ) ),
					-( m_ViewProjection( 3, 3 ) - m_ViewProjection( 3, 2 ) ) );

			// Normalise the plane normals
			for( ZED_UINT32 i = 0; i < 6; i++ )
			{
				ZED_FLOAT32 Length = p_Planes[ i ].GetNormal( ).Magnitude( );
				p_Planes[ i ].SetNormal(
					p_Planes[ i ].GetNormal( )[ 0 ] / Length,
					p_Planes[ i ].GetNormal( )[ 1 ] / Length,
					p_Planes[ i ].GetNormal( )[ 2 ] / Length );
				p_Planes[ i ].SetDistance(
					p_Planes[ i ].GetDistance( ) / Length );
			}
		}

		void XboxRenderer::SetWorldTransform( Arithmetic::Matrix4x4 *p_pWorld )
		{
			// Make sure the V. Cache has been shipped before working in here
			//UNCOMMENTm_pVertexMan->ForceFlushAll( );

			if( !p_pWorld )
			{
				Arithmetic::Matrix4x4 Matrix;
				Matrix.Identity( );
				memcpy( &m_World, &Matrix, sizeof( Arithmetic::Matrix4x4 ) );
			}
			else
			{
				memcpy( &m_World, p_pWorld, sizeof( Arithmetic::Matrix4x4 ) );
			}

			// Recalculate the matrix
			CalcWorldViewProjMatrix( );

			Arithmetic::Matrix4x4 Transpose;
			Transpose.TransposeOf( m_WorldViewProjection );

			// Set up the vertex shader's WVP matrix
			m_pDevice->SetVertexShaderConstantFast( 0,
				( ZED_FLOAT32 * )&Transpose, 4 );
		}*/
	}
}
