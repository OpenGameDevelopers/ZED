// Desc:	The Canvas holds the information about the drawing region; width,
//			height, BPP, stencil/depth, etc.
// Type:	Abstract class

#ifndef __ZED_RENDERER_CANVASDESCRIPTION_HPP__
#define __ZED_RENDERER_CANVASDESCRIPTION_HPP__

// For the data types, mainly
#include <Renderer/RenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class CanvasDescription
		{
#if ( ZED_PLATFORM_WIN32_X86 || ZED_PLATFORM_WIN64_X86 )
			friend class WindowsRendererOGL3;
#endif
		public:
			// Setters
			ZED_INLINE void Width( ZED_UINT32 p_Width )
				{ m_Width = p_Width; }
			ZED_INLINE void Height( ZED_UINT32 p_Height )
				{ m_Height = p_Height; }

			ZED_UINT32 ColourFormat( const ZED_FORMAT &p_BPP );
			ZED_UINT32 DepthStencilFormat( const ZED_FORMAT &p_DS );

			ZED_INLINE void BackBufferCount( const ZED_UINT32 p_BBC )
				{ m_BackBufferCount = p_BBC; }

			ZED_INLINE void AspectRatio( const ZED_FLOAT32 p_AspectRatio )
				{ m_AspectRatio = p_AspectRatio; }
			
			// Getters
			ZED_INLINE ZED_FORMAT ColourFormat( ) const
				{ return m_Colour; }
			ZED_INLINE ZED_FORMAT DepthStencilFormat( ) const
				{ return m_DepthStencil; }
			ZED_INLINE ZED_UINT32 Width( ) const
				{ return m_Width; }
			ZED_INLINE ZED_UINT32 Height( ) const
				{ return m_Height; }
			ZED_INLINE ZED_UINT32 BackBufferCount( ) const
				{ return m_BackBufferCount; }
			ZED_INLINE ZED_FLOAT32 AspectRatio( ) const
				{ return m_AspectRatio; }
			ZED_INLINE ZED_SAMPLE_TYPE AntiAliasingType( ) const
				{ return m_AntiAliasingType; }

		private:
			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;
			ZED_UINT32	m_BackBufferCount;
			ZED_FLOAT32	m_AspectRatio;
			ZED_FORMAT	m_Colour;
			ZED_FORMAT	m_DepthStencil;

			ZED_SAMPLE_TYPE	m_AntiAliasingType;
		};

		// A CanvasSubSection is a separate section to render to within the
		// main canvas
		class CanvasSubSection
		{
		public:
		private:
			ZED_UINT32		m_X;
			ZED_UINT32		m_Y;
			ZED_UINT32		m_Width;
			ZED_UINT32		m_Height;
			ZED_FLOAT32		m_AspectRatio;
			ZED_FLOAT32		m_FOV;
			ZED_VIEWMODE	m_ViewMode;
		};
	}
}

#endif
