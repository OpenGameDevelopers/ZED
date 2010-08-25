using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ZED.Renderer
{
#if ZED_PLATFORM_XNA_XBOX360
    public class Xbox360Renderer : Renderer
    {
        public override uint Create( ref CanvasDescription p_Canvas )
        {
            UInt32 ReturnStatus = Global.ZED_FAIL;
            return ReturnStatus;
        }
    }
#endif
}