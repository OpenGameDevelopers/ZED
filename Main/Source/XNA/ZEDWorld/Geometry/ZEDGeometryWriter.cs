using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline;
using Microsoft.Xna.Framework.Content.Pipeline.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Processors;
using Microsoft.Xna.Framework.Content.Pipeline.Serialization.Compiler;

namespace ZEDContent
{
    /// <summary>
    /// This class will be instantiated by the XNA Framework Content Pipeline
    /// to write the specified data type into binary .xnb format.
    ///
    /// This should be part of a Content Pipeline Extension Library project.
    /// </summary>
    [ ContentTypeWriter ]
    public class ZEDGeometryWriter : ContentTypeWriter< ZEDGeometryContent >
    {
        protected override void Write( ContentWriter p_Output,
            ZEDGeometryContent p_Value )
        {
            p_Value.Write( p_Output );
        }

        public override string GetRuntimeReader(
            TargetPlatform p_TargetPlatform )
        {
            return "ZED.ZEDGeometryReader, ZED";
        }

        public override string GetRuntimeType(
            TargetPlatform p_TargetPlatform )
        {
            return "ZED.ZEDGeometry, ZED";
        }
    }
}
