using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Processors;
using Microsoft.Xna.Framework.Content.Pipeline.Serialization.Compiler;

namespace ZED
{
    /// <summary>
    /// ZEDWorld contains the hierarchy of data within a .zedw file, geometry
    /// that is not referenced via attributes is exported directly into a mesh
    /// which can be rendered and culled away as the meshes are exported
    /// individually, not as a huge model
    /// </summary>
    public class ZEDWorld
    {
        private ModelMeshPart m_MeshParts;

        public ZEDWorld( string p_FileName )
        {
        }
    }
}
