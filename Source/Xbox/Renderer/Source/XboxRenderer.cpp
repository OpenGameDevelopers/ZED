#include <XboxRenderer.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Renderer
	{
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

			// Get the colour
			switch( m_Canvas.GetBPP( ) )
			{
			case ZED_FORMAT_A8R8G8B8:
				{
					m_PresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
					ReturnStatus = ZED_OK;
					break;
				}
			default:
				{
					zedTrace( "Failed to set the back buffer format" );
					return ZED_FAIL;
					// It'll never reach here if the above executes
					break;
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
					break;
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

			return ReturnStatus;
		}

		void XboxRenderer::ForceClear( ZED_BOOL p_Colour, ZED_BOOL p_Depth,
			ZED_BOOL p_Stencil )
		{
			ZED_DWORD Flags;

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

			m_pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, m_Colour, 1.0f, 0L );
		}

		void XboxRenderer::ClearColour( ZED_FLOAT32 p_Red, ZED_FLOAT32 p_Green,
			ZED_FLOAT32 p_Blue )
		{
			m_Colour = D3DCOLOR_COLORVALUE( p_Red, p_Green, p_Blue, 1.0f );
		}

		ZED_UINT32 XboxRenderer::BeginScene( ZED_BOOL p_Colour,
			ZED_BOOL p_Depth, ZED_BOOL p_Stencil )
		{
			ZED_UINT32 ReturnStatus = ZED_FAIL;

			ZED_DWORD Flags;

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

			m_pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, m_Colour, 1.0f, 0L );
			
			if( FAILED( m_pDevice->BeginScene( ) ) )
			{
				zedTrace( "Failed to begin rendering the scene" );
				ReturnStatus = ZED_FAIL;
			}

			return ReturnStatus;
		}

		void XboxRenderer::EndScene( )
		{
			m_pDevice->EndScene( );
			m_pDevice->Present( NULL, NULL, NULL, NULL );
		}

		ZED_BOOL XboxRenderer::ToggleFullscreen( )
		{
			return ZED_TRUE;
		}

		ZED_UINT32 XboxRenderer::ResizeCanvas( ZED_UINT32 p_Width,
			ZED_UINT32 p_Height )
		{
			ZED_UINT32 ReturnStatus = ZED_FAIL;

			m_Width = p_Width;
			m_Height = p_Height;
			m_AspectRatio = static_cast< float >( p_Width / p_Height );

			m_PresentParams.BackBufferWidth = p_Width;
			m_PresentParams.BackBufferHeight = p_Height;

			ReturnStatus = ZED_OK;

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
			}

			if( m_pD3D != NULL )
			{
				m_pD3D->Release( );
			}
		}
	}
}
