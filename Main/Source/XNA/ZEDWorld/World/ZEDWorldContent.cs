using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Processors;
using Microsoft.Xna.Framework.Content.Pipeline.Serialization.Compiler;

namespace ZEDContent
{
    /// <summary>
    /// ZEDWorld contains the hierarchy of data within a .zedw file, geometry
    /// that is not referenced via attributes is exported directly into a mesh
    /// which can be rendered and culled away as the meshes are exported
    /// individually, not as a huge model
    /// </summary>
    public class ZEDWorldContent
    {
        // World parts are locations, and world geometry is shipped off to the
        // geometry processor, well, it's written out as geometry content and
        // re-imported later.
        struct LocationNode
        {
            public string   Name;
            public Vector3  Position;
            public float    Rotation;
        }

        private List< LocationNode > m_Nodes;

        public ZEDWorldContent( string p_FileName )
        {
        }
    }
}
