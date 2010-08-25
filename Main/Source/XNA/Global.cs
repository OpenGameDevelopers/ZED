using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

// Enclose in a ZED namespace?
public class Global
{
    public const UInt32 ZED_OK      = 0x00000001;
    public const UInt32 ZED_FAIL    = 0x7FFFFFFF;

    public static Color Goom = new Color( 0.2f, 0.0f, 0.2f );

    // Model import types
    public const ushort R1_HEADER   = 0x0100;
    public const ushort R1_VERTEX   = 0x0200;
    public const ushort R1_FACE     = 0x0300;
    public const ushort R1_MESH     = 0x0400;
    public const ushort R1_MATERIAL = 0x0500;
    public const ushort R1_END      = 0xFFFF;
}

// World handling
[ StructLayout( LayoutKind.Sequential ) ]
public unsafe struct Location
{
    public fixed char GeoType[ 32 ];
    public Vector2 Position;
    public float Rotation;
}

[ StructLayout( LayoutKind.Sequential ) ]
public struct Chunk
{
    public ulong Size;
    public ushort ID;
}

[ StructLayout( LayoutKind.Sequential ) ]
public unsafe struct Header
{
    public fixed char ID[ 4 ];
    public ulong NumLocations;
}

[ StructLayout( LayoutKind.Sequential ) ]
public unsafe struct FileHeader
{
    public fixed char Name[ 32 ];
    public fixed UInt32 Version[ 3 ];
    public fixed char ID[ 3 ];
    public char Type;
    public char Endianness;
}

[ StructLayout( LayoutKind.Sequential ) ]
public struct Vertex
{
    public Vector3 Position;
    public Vector3 Normal;
    public Vector2 UVs;
}

[ StructLayout( LayoutKind.Sequential ) ]
public unsafe struct Face
{
    public fixed UInt64 Indicies[ 3 ];
    public fixed float Normal[ 3 ];
    public UInt64 MeshID;
    public UInt32 MaterialID;
}

[ StructLayout( LayoutKind.Sequential ) ]
public unsafe struct Mesh
{
    public fixed char Name[ 32 ];
    UInt64 NumFaces;
    UInt32 MaterialID;
}
