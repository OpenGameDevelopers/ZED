using System;
using Microsoft.Xna.Framework.Graphics;

namespace LightShow
{
    public class GraphicsDeviceService : IGraphicsDeviceService
    {
        public event EventHandler<EventArgs> DeviceCreated;
        public event EventHandler<EventArgs> DeviceDisposing;
        public event EventHandler<EventArgs> DeviceReset;
        public event EventHandler<EventArgs> DeviceResetting;

        private PresentationParameters m_PresentParams;
        private GraphicsDevice m_Device;

        public GraphicsDeviceService( GraphicsProfile p_Profile )
        {
            m_PresentParams = new PresentationParameters( );
            float width = (float)GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height*
                ( 16.0f / 9.0f );
            m_PresentParams.BackBufferWidth =
                GraphicsAdapter.DefaultAdapter.IsWideScreen ?            
                (int)width : GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
            
            m_PresentParams.BackBufferHeight =
                GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;

            //m_PresentParams.BackBufferCount = 1;
            //m_PresentParams.SwapEffect = SwapEffect.Discard;
            m_PresentParams.RenderTargetUsage = RenderTargetUsage.DiscardContents;
            m_PresentParams.DepthStencilFormat = DepthFormat.Depth24Stencil8;
            m_PresentParams.PresentationInterval = PresentInterval.Immediate;
            m_PresentParams.RenderTargetUsage = RenderTargetUsage.DiscardContents;

            // MSAA, enable it @4x for the Xbox 360
#if XBOX360
            m_PresentParams.MultiSampleCount = 4;
#else

#endif

            m_Device = new GraphicsDevice( GraphicsAdapter.DefaultAdapter,
                p_Profile, m_PresentParams );
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