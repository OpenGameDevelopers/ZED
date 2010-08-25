using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace ZED
{
    /// <summary>
    /// This class will be instantiated by the XNA Framework Content Pipeline
    /// to write the specified data type into binary .xnb format.
    ///
    /// This should be part of a Content Pipeline Extension Library project.
    /// </summary>
    public class ZEDGeometryReader : ContentTypeReader< ZEDGeometry >
    {
        protected override ZEDGeometry Read( ContentReader p_Input,
            ZEDGeometry p_ExistingInstance )
        {
            return new ZEDGeometry( p_Input );
        }
    }
}
