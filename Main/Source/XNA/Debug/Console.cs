using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace ZED
{
    namespace Debug
    {
        public struct Function
        {
            public List<string> AcceptedArgs;
            public string FunctionName;
            public Function.ActualFunction Functor;
            public string Namespace;

            public delegate uint ActualFunction(int argc, List<string> args);
        }

        public class Console : DrawableGameComponent
        {
            // Region of the console (always centred)
            private Rectangle m_ConsoleRegion;

            // Game to associate console with
            private Game m_Game;

            // The functions for execution
            List< Function > m_Functions;

            // Console background
            private Texture2D m_Background;

            public Console( Game GameIn, string PathToFont,
                string PathToBackground, Color FontColour,
                List< Keys > ConsoleActivation, double DisplayDelay,
                int Width, int Height,
                int ConsoleBuffer ) : base( GameIn )
            {
                // Initialise all lists
                m_Functions = new List< Function >( );
            }

            public void AddFunction( Function Func )
            {
                m_Functions.Add( Func );
            }

            public uint Execute( string Func )
            {
                return Global.ZED_OK;
            }

            public void PushString( string Message )
            {
            }

            protected override void LoadContent( )
            {
                base.LoadContent( );
            }

            public override void Draw(GameTime Time)
            {
                base.Draw( Time );
            }

            public override void Update( GameTime Time )
            {
                base.Update( Time );
            }

            #region Accessors
            public int Height
            {
                get
                {
                    return m_ConsoleRegion.Height;
                }
                set
                {
                    // Re-calculate the vertical position
                    m_ConsoleRegion.Height = value;
                    m_ConsoleRegion.Y =
                        ( m_Game.GraphicsDevice.PresentationParameters.BackBufferHeight / 2 ) -
                        ( m_ConsoleRegion.Height / 2 );
                }
            }
            public int Width
            {
                get
                {
                    return m_ConsoleRegion.Width;
                }
                set
                {
                    // Re-calculate the horizontal position
                    m_ConsoleRegion.Width = value;
                    m_ConsoleRegion.X =
                        ( m_Game.GraphicsDevice.PresentationParameters.BackBufferWidth / 2 ) -
                        ( m_ConsoleRegion.Width / 2 );
                }
            }
            #endregion
        }
    }
}
