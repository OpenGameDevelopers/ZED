using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace ZED
{
    public class ZEDGeometry
    {
        // Models are made out of faces, which in turn are created from
        // vertices, naturally
        private List< Vertex > m_Vertices;
        private List< Face > m_Faces;
        private List< Mesh > m_Meshes;
        private Effect m_Shader;
        private Matrix m_World, m_View, m_Projection;
        private Vector3 m_Position = Vector3.Zero;
        private GraphicsDevice m_Device;
        private VertexDeclaration m_VertexDeclaration;
        private VertexBuffer m_VertexBuffer;
        private IndexBuffer m_IndexBuffer;
        private int m_IndexOffset = 0;

        public ZEDGeometry( )
        {
            m_Vertices = new List< Vertex >( );
            m_Faces = new List< Face >( );
            m_Meshes = new List< Mesh >( );
        }

        internal ZEDGeometry( ContentReader p_Reader )
        {
            // Get the vertex data
            Chunk TempChunk;
            TempChunk.ID = p_Reader.ReadUInt16( );
            TempChunk.Size = p_Reader.ReadUInt64( );

            Vertex TempVertex = new Vertex( );

            m_Vertices = new List< Vertex >( );

            UInt64 VertCount = TempChunk.Size;

            for( UInt64 i = 0; i < VertCount; i++ )
            {
                TempVertex.Position.X = p_Reader.ReadSingle( );
                TempVertex.Position.Y = p_Reader.ReadSingle( );
                TempVertex.Position.Z= p_Reader.ReadSingle( );

                TempVertex.Normal.X = p_Reader.ReadSingle( );
                TempVertex.Normal.Y = p_Reader.ReadSingle( );
                TempVertex.Normal.Z = p_Reader.ReadSingle( );

                TempVertex.UVs.X = p_Reader.ReadSingle( );
                TempVertex.UVs.Y = p_Reader.ReadSingle( );

                m_Vertices.Add( TempVertex );
            }

            // Read the end chunk, but don't store it
            TempChunk.ID = p_Reader.ReadUInt16( );
            TempChunk.Size = p_Reader.ReadUInt64( );


            // Get the face chunk
            TempChunk.ID = p_Reader.ReadUInt16( );
            TempChunk.Size = p_Reader.ReadUInt64( );

            Face TempFace = new Face( );
            m_Faces = new List< Face >( );

            UInt64 FaceCount = TempChunk.Size;
            for( UInt64 i = 0; i < FaceCount; i++ )
            {
                unsafe
                {
                    TempFace.Indicies[ 0 ] = p_Reader.ReadUInt64( );
                    TempFace.Indicies[ 1 ] = p_Reader.ReadUInt64( );
                    TempFace.Indicies[ 2 ] = p_Reader.ReadUInt64( );

                    TempFace.Normal[ 0 ] = p_Reader.ReadSingle( );
                    TempFace.Normal[ 1 ] = p_Reader.ReadSingle( );
                    TempFace.Normal[ 2 ] = p_Reader.ReadSingle( );

                    TempFace.MeshID = p_Reader.ReadUInt64( );
                    TempFace.MaterialID = p_Reader.ReadUInt32( );

                    m_Faces.Add( TempFace );
                }
            }
        }

        public void InsertVertex( Vertex p_Vertex )
        {
            m_Vertices.Add( p_Vertex );
        }

        public void InsertFace( Face p_Face )
        {
            m_Faces.Add( p_Face );
        }

        public void InsertMesh( )
        {
        }

        public void SetPosition( ref Vector3 p_Position )
        {
            m_Position = p_Position;
        }

        public void SetProjection( ref Matrix p_Projection )
        {
            m_Projection = p_Projection;
            m_Shader.Parameters[ "Projection" ].SetValue( m_Projection );
        }

        public void Initialise( GraphicsDevice p_Device )
        {
            m_Device = p_Device;
            m_VertexDeclaration = new VertexDeclaration( m_Device,
                VertexPositionNormalTexture.VertexElements );
            // Too much for a simple model...
            Vertex VertSize = new Vertex( );
            int VertexSize = Marshal.SizeOf( VertSize );
            Vertex [ ] VerticesToCopy = m_Vertices.ToArray( );

            m_VertexBuffer = new VertexBuffer( m_Device, m_Vertices.Count *
                VertexPositionNormalTexture.SizeInBytes, BufferUsage.WriteOnly );
            m_VertexBuffer.SetData< Vertex >( 0, VerticesToCopy, 0, m_Vertices.Count,
                VertexSize );

            int[ ] Indices = new int[ m_Faces.Count * 3 ];
            Face LocalFace;
            for( int Row = 0; Row < 3; Row++ )
            {
                for( int Column = 0; Column < m_Faces.Count; Column++ )
                {
                    LocalFace = m_Faces[ Column ];
                    unsafe
                    {
                        Indices[ Row + ( 3*Column ) ] =
                            ( int )LocalFace.Indicies[ Row ];
                    }
                }
            }
            
            m_IndexBuffer = new IndexBuffer( m_Device, typeof( int ),
                Indices.Length, BufferUsage.WriteOnly );
            m_IndexBuffer.SetData< int >( Indices );

        }

        public void Update( ref Matrix p_View )
        {
            m_World = Matrix.CreateTranslation( m_Position );
            m_View = p_View;

            m_Shader.Parameters[ "World" ].SetValue( m_World );
            m_Shader.Parameters[ "View" ].SetValue( m_View );
        }

        public void Render( )
        {
            // Assuming the shader has been updated correctly...
            // Replace this with a foreach on the mesh list

            m_IndexOffset++;

            if( m_IndexOffset > m_Faces.Count )
            {
                m_IndexOffset = 0;
            }

            m_Shader.Begin( );
            m_Device.RenderState.CullMode = CullMode.CullClockwiseFace;
            m_Device.RenderState.FillMode = FillMode.WireFrame;
            foreach( EffectPass Pass in m_Shader.CurrentTechnique.Passes )
            {
                Pass.Begin( );
                {
                    m_Device = m_Shader.GraphicsDevice;
                    m_Device.VertexDeclaration = m_VertexDeclaration;
                    m_Device.Vertices[ 0 ].SetSource( m_VertexBuffer, 0,
                        VertexPositionNormalTexture.SizeInBytes );
                    m_Device.Indices = m_IndexBuffer;
                    m_Device.DrawIndexedPrimitives( PrimitiveType.TriangleList,
                        0, 0, m_Vertices.Count, 0, m_Faces.Count );
                }
                Pass.End( );
            }
            m_Shader.End( );
        }

        public void SetShader( ref Effect p_Shader )
        {
            m_Shader = p_Shader;
        }
    }
}