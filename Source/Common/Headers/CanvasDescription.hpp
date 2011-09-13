// Desc:	The Canvas holds the information about the drawing region; width,
//			height, BPP, stencil/depth, etc.
// Type:	Abstract class

#ifndef __ZEDCANVASDESCRIPTION_HPP__
#define __ZEDCANVASDESCRIPTION_HPP__

// For the data types, mainly
//#include <Renderer.hpp>
#include <RenderTypes.hpp>

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
			ZED_INLINE void SetWidth( ZED_UINT32 p_Width )
				{ m_Width = p_Width; }
			ZED_INLINE void SetHeight( ZED_UINT32 p_Height )
				{ m_Height = p_Height; }

			ZED_UINT32 SetBPP( const ZED_FORMAT &p_BPP );
			ZED_UINT32 SetDepthStencil( const ZED_FORMAT &p_DS );

			ZED_INLINE void SetBackBufferCount( ZED_UINT32 p_BBC )
				{ m_BackBufferCount = p_BBC; }

			ZED_INLINE void SetAspectRatio( ZED_FLOAT32 p_AspectRatio )
				{ m_AspectRatio = p_AspectRatio; }
			
			// Getters
			ZED_INLINE ZED_FORMAT GetBPP( ) const
				{ return m_Colour; }
			ZED_INLINE ZED_FORMAT GetDepthStencil( ) const
				{ return m_DepthStencil; }
			ZED_INLINE ZED_UINT32 GetWidth( ) const
				{ return m_Width; }
			ZED_INLINE ZED_UINT32 GetHeight( ) const
				{ return m_Height; }
			ZED_INLINE ZED_UINT32 GetBackBufferCount( ) const
				{ return m_BackBufferCount; }
			ZED_INLINE ZED_FLOAT32 GetAspectRatio( ) const
				{ return m_AspectRatio; }

		private:
			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;
			ZED_UINT32	m_BackBufferCount;
			ZED_FLOAT32	m_AspectRatio;
			ZED_FORMAT	m_Colour;
			ZED_FORMAT	m_DepthStencil;
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
