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
    public class ZEDWorldReader : ContentTypeReader< ZEDGeometry >
    {
        protected override ZEDGeometry Read( ContentReader input,
            ZEDGeometry existingInstance )
        {
            throw new NotImplementedException();
        }
    }
}
