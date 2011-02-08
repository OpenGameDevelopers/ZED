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
		public:
			ZED_INLINE void SetWidth( ZED_UINT32 p_Width )
				{ m_Width = p_Width; }
			ZED_INLINE void SetHeight( ZED_UINT32 p_Height )
				{ m_Height = p_Height; }

			ZED_UINT32 SetBPP( const ZED_FORMAT &p_BPP );
			ZED_UINT32 SetDepthStencil( const ZED_FORMAT &p_DS );

			ZED_INLINE void SetBackBufferCount( ZED_UINT32 p_BBC )
				{ m_BackBufferCount = p_BBC; }
			
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

		private:
			ZED_UINT32 m_Width;
			ZED_UINT32 m_Height;
			ZED_UINT32 m_BackBufferCount;
			ZED_FORMAT m_Colour;
			ZED_FORMAT m_DepthStencil;
		};
	}
}

#endif
