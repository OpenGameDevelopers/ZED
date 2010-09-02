using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;

namespace ZED.Renderer
{
    class GraphicsDeviceService : IGraphicsDeviceService
    {
        public event EventHandler DeviceCreated;
        public event EventHandler DeviceDisposing;
        public event EventHandler DeviceReset;
        public event EventHandler DeviceResetting;

        private PresentationParameters m_PresentParams;
        private GraphicsDevice m_Device;

        public GraphicsDeviceService( IntPtr p_WindowHandle )
        {
            m_PresentParams = new PresentationParameters( );
            float width = (float)GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height*
                ( 16.0f / 9.0f );
            m_PresentParams.BackBufferWidth =
                GraphicsAdapter.DefaultAdapter.IsWideScreen ?            
                (int)width : GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
            
            m_PresentParams.BackBufferHeight =
                GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;

            m_PresentParams.BackBufferCount = 1;
            m_PresentParams.SwapEffect = SwapEffect.Discard;
            m_PresentParams.RenderTargetUsage = RenderTargetUsage.DiscardContents;
            m_PresentParams.AutoDepthStencilFormat = DepthFormat.Depth24Stencil8;
            m_PresentParams.PresentationInterval = PresentInterval.Immediate;
            m_PresentParams.RenderTargetUsage = RenderTargetUsage.DiscardContents;

#if XBOX360
            m_PresentParams.MultiSampleQuality = 0;
            m_PresentParams.MultiSampleType = MultiSampleType.FourSamples;
#else

#endif

            m_Device = new GraphicsDevice( GraphicsAdapter.DefaultAdapter,
                DeviceType.Hardware,
                p_WindowHandle, m_PresentParams );
        }


        public GraphicsDevice GraphicsDevice
        {
            get
            {
                return m_Device;
            }
        }
    }
}