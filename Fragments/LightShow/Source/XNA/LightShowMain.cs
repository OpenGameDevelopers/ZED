using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Content;

namespace LightShow
{
    static class LightShowMain
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main( string[ ] args )
        {
            string MSAAStr = " ";
            string m_FPSString = "[ ?? ]";
            Texture2D m_XNAGS;
            GraphicsAdapter m_Adapter = GraphicsAdapter.DefaultAdapter;
            m_Adapter.GetCapabilities( DeviceType.Hardware );
            IntPtr m_WindowHandle = new IntPtr( );
            Rectangle m_TitleSafe;
            bool True = true;
            double m_FrameTime = 0.0d;
            System.Diagnostics.Stopwatch m_Watch = new System.Diagnostics.Stopwatch( );

            GraphicsDeviceService m_GDS = new GraphicsDeviceService( m_WindowHandle );

            GameServiceContainer m_GSC = new GameServiceContainer( );
            UInt32 m_FPS = 0;

            m_GSC.AddService( typeof( IGraphicsDeviceService ), m_GDS );

            ContentManager m_ContentMan = new ContentManager( m_GSC, "Content" );

            //GamerServicesDispatcher.WindowHandle = m_WindowHandle;
            //GamerServicesDispatcher.Initialize( m_GSC );

            GameTime m_Time = new GameTime( TimeSpan.Zero, TimeSpan.Zero, TimeSpan.Zero, TimeSpan.Zero );

            SpriteBatch m_Batch = new SpriteBatch( m_GDS.GraphicsDevice );
            SpriteFont m_Font = m_ContentMan.Load< SpriteFont >( "Debug/Fonts/Console" );
            m_XNAGS = m_ContentMan.Load< Texture2D >( "Debug/Platform/XNA" );

            
            m_TitleSafe.X = ( m_GDS.GraphicsDevice.PresentationParameters.BackBufferWidth/100 )*10;
            m_TitleSafe.Y = ( m_GDS.GraphicsDevice.PresentationParameters.BackBufferHeight/100 )*10;
            m_TitleSafe.Width = m_GDS.GraphicsDevice.PresentationParameters.BackBufferWidth - ( m_TitleSafe.X * 2 );
            m_TitleSafe.Height = m_GDS.GraphicsDevice.PresentationParameters.BackBufferHeight - ( m_TitleSafe.Y * 2 );
            
            float AspectRatio = m_GDS.GraphicsDevice.PresentationParameters.BackBufferWidth / 
                m_GDS.GraphicsDevice.PresentationParameters.BackBufferHeight;
            Vector2 m_XNAGSScale = new Vector2( ( m_XNAGS.Width / 2.0f )*AspectRatio, m_XNAGS.Height / 2.0f );
            Rectangle m_XNAGSRect = new Rectangle( 
                    ( m_TitleSafe.X + m_TitleSafe.Width )-
                    ( m_XNAGS.Width / 2 ),
                    ( m_TitleSafe.Y + m_TitleSafe.Height )-
                    ( m_XNAGS.Height / 2 ), m_XNAGS.Width / 2, m_XNAGS.Height / 2 );
            Vector2 m_AllocatedPosition = new Vector2( m_TitleSafe.X, m_TitleSafe.Y );
            if( m_GDS.GraphicsDevice.PresentationParameters.MultiSampleType == MultiSampleType.FourSamples )
            {
                MSAAStr = "4xMSAA";
            }
            else if( m_GDS.GraphicsDevice.PresentationParameters.MultiSampleType == MultiSampleType.TwoSamples )
            {
                MSAAStr = "2xMSAA";
            }
            else
            {
                MSAAStr = "?xMSAA";
            }
            
            string DisplayInfo = "GPU: " + m_Adapter.Description + " [ " +
                    m_GDS.GraphicsDevice.PresentationParameters.BackBufferWidth.ToString( ) + "x" +
                    m_GDS.GraphicsDevice.PresentationParameters.BackBufferHeight.ToString( ) + "@" +
                    GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.RefreshRate.ToString( ) + " ] " + MSAAStr;

            while( True )
            {
                m_Watch = System.Diagnostics.Stopwatch.StartNew( );
                if( GamePad.GetState( PlayerIndex.One ).Buttons.Back == ButtonState.Pressed )
                {
                    True = false;
                }

                //GamerServicesDispatcher.Update( );

                /*foreach( Gamer gamer in Gamer.SignedInGamers )
                {
                    System.Diagnostics.Debug.WriteLine( gamer.Gamertag );
                }*/
                m_GDS.GraphicsDevice.Clear( ClearOptions.Target, //|
                    //ClearOptions.Stencil | ClearOptions.Target,
                    new Color( 0.13f, 0.0f, 0.13f ), 1.0f, 0 );

                m_Batch.Begin( );
                m_Batch.Draw( m_XNAGS, m_XNAGSRect, Color.White );
                m_Batch.DrawString( m_Font, "Bytes Allocated " +
                    GC.GetTotalMemory( false ).ToString( ),
                    m_AllocatedPosition, Color.Yellow );
                m_Batch.DrawString( m_Font, 
                    DisplayInfo,
                    new Vector2( m_AllocatedPosition.X, m_AllocatedPosition.Y+20 ),
                    Color.Yellow );
                if( m_FrameTime > 1000.0d )
                {
                    m_FPSString = "[ " + m_FPS.ToString( ) + " ]";
                    m_FrameTime = 0.0d;
                    m_FPS = 0;
                }

                m_Batch.DrawString( m_Font, m_FPSString,
                        new Vector2( m_AllocatedPosition.X,
                            m_TitleSafe.Y +
                            m_TitleSafe.Height-20 ),
                        Color.Yellow );
                m_Batch.End( );

                m_GDS.GraphicsDevice.Present( null, null, m_WindowHandle );

                m_FrameTime += m_Watch.ElapsedMilliseconds;

                // Increment the framerate counter
                m_FPS++;
            }

            m_ContentMan.Dispose( );
        }
    }
}
