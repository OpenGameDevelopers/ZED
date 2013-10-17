#include <Renderer/PandoraLinuxRenderer.hpp>

namespace ZED
{
	namespace Renderer
	{
		PandoraLinuxRenderer::PandoraLinuxRenderer( )
		{
		}

		PandoraLinuxRenderer::~PandoraLinuxRenderer( )
		{
			if( m_WindowData.eglDisplay )
			{
				eglMakeCurrent( m_WindowData.eglDisplay, EGL_NO_SURFACE,
					EGL_NO_SURFACE, EGL_NO_CONTEXT );
				if( m_EGLContext )
				{
					eglDestroyContext( m_WindowData.eglDisplay, m_EGLContext );
				}
			}
		}

		ZED_UINT32 PandoraLinuxRenderer::Create(
			const CanvasDescription &p_Canvas,
			const ZED::System::Window &p_Window )
		{
			m_WindowData = p_Window.WindowData( );

			if( m_WindowData.pX11Display == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::PandoraLinuxRenderer] <ERROR> "
					"Display not initialised\n" );

				return ZED_FAIL;
			}

			m_Canvas = p_Canvas;

			ZED_SINT32 Depth = 0, Stencil = 0, A = 0, R = 0, G = 0, B = 0,
				BPP = 0, SampleBuffer = 0, SampleCount = 0;

			switch( m_Canvas.DepthStencilFormat( ) )
			{
				case ZED_FORMAT_D24S8:
				{
					Depth = 24;
					Stencil = 8;
					break;
				}
				default:
				{
					Depth = 24;
					Stencil = 8;
					break;
				}
			}

			switch( m_Canvas.ColourFormat( ) )
			{
				case ZED_FORMAT_ARGB8:
				{
					A = R = G = B = 8;
					BPP = 32;
					break;
				}
				case ZED_FORMAT_XRGB8:
				{
					A = 0;
					R = G = B = 8;
					BPP = 24;
				}
				case ZED_FORMAT_RGB565:
				{
					R = B = 5;
					G = 1;
					BPP = 16;
				}
				default:
				{
					break;
				}
			}

			switch( m_Canvas.AntiAliasingType( ) )
			{
				case ZED_SAMPLE_TYPE_MSAA_4:
				{
					SampleBuffer = 1;
					SampleCount = 4;
					break;
				}
			}

			EGLint Attributes[ ] =
			{
				EGL_RED_SIZE,		R,
				EGL_GREEN_SIZE,		G,
				EGL_BLUE_SIZE,		B,
				EGL_ALPHA_SIZE,		A,
				EGL_BUFFER_SIZE,	BPP,
				EGL_DEPTH_SIZE,		Depth,
				EGL_STENCIL_SIZE,	Stencil,
				EGL_SAMPLE_BUFFERS,	SampleBuffer,
				EGL_SAMPLES,		SampleCount,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_NONE
			};
			
			m_WindowData.eglDisplay = eglGetDisplay(
				( EGLNativeDisplayType )m_WindowData.pX11Display );

			if( m_WindowData.eglDisplay == EGL_NO_DISPLAY )
			{
				zedTrace( "[ZED::Renderer::PandoraLinuxRenderer::Create] "
					"<ERROR> EGL display invalid\n" );

				return ZED_FAIL;
			}


			EGLint Major = 0, Minor = 0;
			if( !eglInitialize( m_WindowData.eglDisplay, &Major, &Minor ) )
			{
				return ZED_FAIL;
			}

			EGLint ConfigCount = 0;

			return ZED_OK;
		}

		void PandoraLinuxRenderer::ForceClear( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
		}

		void PandoraLinuxRenderer::ClearColour( const ZED_FLOAT32 p_Red,
			const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue )
		{
		}

		ZED_UINT32 PandoraLinuxRenderer::BeginScene( const ZED_BOOL p_Colour,
			const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil )
		{
		}

		void PandoraLinuxRenderer::EndScene( )
		{
		}
		
		ZED_UINT32 PandoraLinuxRenderer::ResizeCanvas(
			const ZED_UINT32 p_Width, const ZED_UINT32 p_Height )
		{
			return ZED_OK;
		}
		
		ZED_BOOL PandoraLinuxRenderer::ShaderSupport( )
		{
			return ZED_TRUE;
		}

		ZED_UINT32 PandoraLinuxRenderer::Render(
			const ZED_MEMSIZE p_VertexCount, const ZED_BYTE *p_pVertices,
			const ZED_MEMSIZE p_IndexCount, const ZED_UINT16 *p_pIndices,
			const ZED_UINT64 p_Attributes, const ZED_UINT32 p_MaterialID,
			const ZED_RENDERPRIMITIVETYPE p_PrimitiveType )
		{
			return ZED_OK;
		}

		void PandoraLinuxRenderer::RenderState( const ZED_RENDERSTATE p_State,
			const ZED_UINT32 p_Value )
		{
		}
	}
}

