using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;

namespace ZED.Renderer
{
    /// <summary>
    /// The CanvasDescription contains the opaque descriptor for the screen
    /// to render to
    /// </summary>
    public class CanvasDescription
    {
        private UInt32 m_Width;
        private UInt32 m_Height;
        private UInt32 m_BackBufferCount;

        public UInt32 Width
        {
            get
            {
                return m_Width;
            }
            set
            {
                m_Width = value;
            }
        }

        public UInt32 Height
        {
            get
            {
                return m_Height;
            }
            set
            {
                m_Height = value;
            }
        }

        public UInt32 BackBufferCount
        {
            get
            {
                return m_BackBufferCount;
            }
            set
            {
                m_BackBufferCount = value;
            }
        }
    }
}
