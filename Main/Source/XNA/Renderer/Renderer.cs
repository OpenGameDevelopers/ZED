using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ZED.Renderer
{
    public abstract class Renderer
    {
        public abstract UInt32 Create( ref CanvasDescription p_Canvas );
    }
}