using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline.Processors;
using Microsoft.Xna.Framework.Content.Pipeline.Serialization.Compiler;

namespace ZEDContent
{
    public class ZEDGeometryContent
    {
        // The file header, to verify that this is valid
        private FileHeader m_FileHeader;
        // Models need to have vertices, faces and meshes
        private List< Vertex > m_Vertices;
        private List< Face > m_Faces;
        private List< Mesh > m_Meshes;

        public ZEDGeometryContent( )
        {
            m_Vertices = new List< Vertex >( );
            m_Faces = new List< Face >( );
            m_Meshes = new List< Mesh >( );
        }

        // Helper functions to process all elements at once
        // Amazingly, it just passes the same information around
        public void AddHeader( FileHeader p_FileHeader )
        {
            m_FileHeader = p_FileHeader;
        }

        public void AddVertex( Vertex p_Vertex )
        {
            m_Vertices.Add( p_Vertex );
        }

        public void AddFace( Face p_Face )
        {
            m_Faces.Add( p_Face );
        }

        public void AddMesh( Mesh p_Mesh )
        {
            m_Meshes.Add( p_Mesh );
        }

        public FileHeader GetFileHeader( )
        {
            return m_FileHeader;
        }

        public Vertex GetVertex( Int32 p_Index )
        {
            return m_Vertices[ p_Index ];
        }

        public Face GetFace( Int32 p_Index )
        {
            return m_Faces[ p_Index ];
        }

        public Mesh GetMesh( Int32 p_Index )
        {
            return m_Meshes[ p_Index ];
        }

        // Write out the content
        public void Write( ContentWriter p_Out )
        {
            // Still a chunk-based format
            Chunk TempChunk;
            TempChunk.ID = Global.R1_VERTEX;
            TempChunk.Size = ( ulong )m_Vertices.Count;
            p_Out.Write( TempChunk.ID );
            p_Out.Write( TempChunk.Size );

            // Write out all types, vertices, faces and meshes
            for( int i = 0; i < m_Vertices.Count; i++ )
            {
                p_Out.Write( m_Vertices[ i ].Position.X );
                p_Out.Write( m_Vertices[ i ].Position.Y );
                p_Out.Write( m_Vertices[ i ].Position.Z );

                p_Out.Write( m_Vertices[ i ].Normal.X );
                p_Out.Write( m_Vertices[ i ].Normal.Y );
                p_Out.Write( m_Vertices[ i ].Normal.Z );

                p_Out.Write( m_Vertices[ i ].UVs.X );
                p_Out.Write( m_Vertices[ i ].UVs.Y );
            }
            // Vertex ouput end
            TempChunk.ID = Global.R1_END;
            TempChunk.Size = 0L;
            p_Out.Write( TempChunk.ID );
            p_Out.Write( TempChunk.Size );

            // Face data
            TempChunk.ID = Global.R1_FACE;
            TempChunk.Size = ( ulong )m_Faces.Count;
            p_Out.Write( TempChunk.ID );
            p_Out.Write( TempChunk.Size );
            
            Face TempFace;

            for( int i = 0; i < m_Faces.Count; i++ )
            {
                TempFace = m_Faces[ i ];
                unsafe
                {
                    p_Out.Write( TempFace.Indicies[ 0 ] );
                    p_Out.Write( TempFace.Indicies[ 1 ] );
                    p_Out.Write( TempFace.Indicies[ 2 ] );

                    p_Out.Write( TempFace.Normal[ 0 ] );
                    p_Out.Write( TempFace.Normal[ 1 ] );
                    p_Out.Write( TempFace.Normal[ 2 ] );

                    p_Out.Write( TempFace.MeshID );
                    p_Out.Write( TempFace.MaterialID );
                }
            }

            // Face ouput end
            TempChunk.ID = Global.R1_END;
            TempChunk.Size = 0L;
            p_Out.Write( TempChunk.ID );
            p_Out.Write( TempChunk.Size );
        }

        // Accessors for statistical data
        public Int32 VertexCount
        {
            get
            {
                return m_Vertices.Count( );
            }
        }

        public Int32 FaceCount
        {
            get
            {
                return m_Faces.Count( );
            }
        }
    }
}
