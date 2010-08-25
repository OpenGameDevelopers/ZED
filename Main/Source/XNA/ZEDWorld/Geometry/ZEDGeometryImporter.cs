using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content.Pipeline;
using Microsoft.Xna.Framework.Content.Pipeline.Graphics;
using System.Runtime.InteropServices;

namespace ZEDContent
{
    /// <summary>
    /// This class will be instantiated by the XNA Framework Content Pipeline
    /// to import a file from disk into the specified type, TImport.
    /// 
    /// This should be part of a Content Pipeline Extension Library project.
    /// 
    /// TODO: change the ContentImporter attribute to specify the correct file
    /// extension, display name, and default processor for this importer.
    /// </summary>
    [ ContentImporter( ".zedg", DisplayName = "ZED Geometry Importer",
        DefaultProcessor = "ZEDGeometryProcessor" ) ]
    public class ZEDGeometryImporter : ContentImporter< ZEDGeometryContent >
    {
        private ZEDGeometryContent m_RawModel = new ZEDGeometryContent( );
        private BinaryReader m_Reader;

        public override ZEDGeometryContent Import( string p_Filename,
            ContentImporterContext p_Context)
        {
            // Go through each chunk and write out the data in it's raw,
            // naked form
            m_Reader = new BinaryReader( File.Open( p_Filename, FileMode.Open,
                FileAccess.Read ) );

            // Check for the header, ensure it's correct
            Chunk TempChunk;
            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_HEADER )
            {
                throw new InvalidContentException(
                    "Content header chunk not found" );
            }

            FileHeader TempHeader;
            string IDTest = string.Empty;
            unsafe
            {
                for( UInt16 i = 0; i < 32; i++ )
                {
                    TempHeader.Name[ i ] = m_Reader.ReadChar( );
                }
                for( UInt16 i = 0; i < 3; i++ )
                {
                    TempHeader.Version[ i ] = m_Reader.ReadUInt32( );
                }
                for( UInt16 i = 0; i < 3; i++ )
                {
                    TempHeader.ID[ i ] = m_Reader.ReadChar( );
                }
                TempHeader.Type = m_Reader.ReadChar( );
                TempHeader.Endianness = m_Reader.ReadChar( );

                IDTest = TempHeader.ID[ 0 ].ToString( ) +
                         TempHeader.ID[ 1 ].ToString( ) +
                         TempHeader.ID[ 2 ].ToString( ) + TempHeader.Type;
            }

            if( IDTest != "ZEDG" )
            {
                throw new InvalidContentException(
                    "Content header is incorrect" );
            }

            // Need an end chunk
            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_END )
            {
                throw new InvalidContentException(
                    "Content header has no end" );
            }

            // Now read the vertices in.  Simply dividing by the vertex count
            // by the vertex structure should work, hopefully, no one has
            // mangled the .zedg file
            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_VERTEX )
            {
                throw new InvalidContentException(
                    "Content has no vertices" );
            }

            Vertex TempVertex = new Vertex( );
            UInt64 TempCount = TempChunk.Size /
                ( ulong )Marshal.SizeOf( TempVertex );

            for( UInt64 i = 0; i < TempCount; i++ )
            {
                TempVertex.Position.X = m_Reader.ReadSingle( );
                TempVertex.Position.Y = m_Reader.ReadSingle( );
                TempVertex.Position.Z= m_Reader.ReadSingle( );

                TempVertex.Normal.X = m_Reader.ReadSingle( );
                TempVertex.Normal.Y = m_Reader.ReadSingle( );
                TempVertex.Normal.Z = m_Reader.ReadSingle( );

                TempVertex.UVs.X = m_Reader.ReadSingle( );
                TempVertex.UVs.Y = m_Reader.ReadSingle( );

                m_RawModel.AddVertex( TempVertex );
            }

            // Endless checking for end chunks =D
            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_END )
            {
                throw new InvalidContentException(
                    "Content vertex chunk has no end" );
            }

            // Now get the faces
            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_FACE )
            {
                throw new InvalidContentException(
                    "Content has no faces" );
            }

            Face TempFace = new Face( );
            TempCount = TempChunk.Size / 48;
            
            for( UInt64 i = 0; i < TempCount; i++ )
            {
                unsafe
                {
                    TempFace.Indicies[ 0 ] = m_Reader.ReadUInt64( );
                    TempFace.Indicies[ 1 ] = m_Reader.ReadUInt64( );
                    TempFace.Indicies[ 2 ] = m_Reader.ReadUInt64( );

                    TempFace.Normal[ 0 ] = m_Reader.ReadSingle( );
                    TempFace.Normal[ 1 ] = m_Reader.ReadSingle( );
                    TempFace.Normal[ 2 ] = m_Reader.ReadSingle( );

                    TempFace.MeshID = m_Reader.ReadUInt64( );
                    TempFace.MaterialID = m_Reader.ReadUInt32( );

                    m_RawModel.AddFace( TempFace );
                }
            }

            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_END )
            {
                throw new InvalidContentException(
                    "Content face chunk has no end" );
            }

            // Check for one last end chunk
            TempChunk.ID = m_Reader.ReadUInt16( );
            TempChunk.Size = m_Reader.ReadUInt64( );

            if( TempChunk.ID != Global.R1_END )
            {
                throw new InvalidContentException(
                    "Content has no final end chunk" );
            }

            return m_RawModel;
        }
    }
}
