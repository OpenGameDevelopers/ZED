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
    public const uint ZED_OK     = 0x00000001;

    public static Color Goom = new Color( 0.2f, 0.0f, 0.2f );

    // Model import types
    public const ushort R1_HEADER =    0x0001;
    public const ushort R1_LOCATION =  0x0002;
    public const ushort R1_END =       0x9999;
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
