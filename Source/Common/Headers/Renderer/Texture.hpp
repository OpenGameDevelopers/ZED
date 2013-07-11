#ifndef __ZED_RENDERER_TEXTURE_HPP__
#define __ZED_RENDERER_TEXTURE_HPP__

#include <Renderer/Renderer.hpp>

namespace ZED
{
	namespace Renderer
	{
#pragma pack( 1 )
		typedef struct __ZED_PNGHEADER
		{
			ZED_BYTE	Transmission;
			ZED_BYTE	PNGString[ 3 ];
			ZED_BYTE	LineEndDOS[ 2 ];
			ZED_BYTE	DOSStop;
			ZED_BYTE	LineEndUnix;
		}PNGHEADER,*LPPNGHEADER;

		typedef struct __ZED_PNGCHUNK
		{
			ZED_UINT32	Length;
			ZED_UINT32	Type;
		}PNGCHUNK,*LPPNGCHUNK;

		typedef struct __ZED_PNGIHDR
		{
			ZED_UINT32	Width;
			ZED_UINT32	Height;
			ZED_BYTE	BitDepth;
			ZED_BYTE	ColourType;
			ZED_BYTE	Compression;
			ZED_BYTE	Filter;
			ZED_BYTE	Interlace;
		}PNGIHDR,*LPPNGIHDR;

		// Custom ZED Texture (Ver. 1.0 is completely uncompressed)
		typedef struct __ZEDT_HEADER
		{
			// Should always be "ZEDT"
			ZED_CHAR8 ID[ 4 ];

			// Major.Minor.Revision
			ZED_UINT32 Version[ 3 ];

			// Texture width and height, naturally
			ZED_UINT32 Width;
			ZED_UINT32 Height;

			// Using the format, the BPP can be worked out pretty easily
			ZED_FORMAT Format;
		}ZEDT_HEADER,*LPZEDT_HEADER;
#pragma pack( )

		// PNG CRC calculation
		// Ergh, a global!
		static ZED_BOOL		g_PNGCRCTableInit = ZED_FALSE;
		static ZED_ULONG	g_PNGCRCTable[ 256 ];

		// Create the CRC table [http://www.w3.org/TR/PNG/#D-CRCAppendix]
		void PNGCRCTableCreate( );
		// Update a running CRC with the buffer's bytes
		ZED_ULONG PNGUpdateCRC( ZED_ULONG p_CRC, ZED_BYTE *p_pBuffer,
			ZED_SINT32 p_Length );
		ZED_ULONG PNGCRC( ZED_BYTE *p_pBuffer, ZED_SINT32 p_Length );

		class Texture
		{
		public:
			virtual ~Texture( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilename ) = 0;
/*
			virtual void SetData( const void *p_pData ) = 0;
			virtual void SetFormat( const ZED_FORMAT *p_Format ) = 0;
			virtual void SetWidth( const ZED_UINT32 p_Width ) = 0;
			virtual void SetHeight( const ZED_UINT32 p_Heght ) = 0;

			virtual void		*GetData( ) const = 0;
			virtual ZED_FORMAT	GetFormat( ) const = 0;
			virtual ZED_UINT32	GetWidth( ) const = 0;
			virtual ZED_UINT32	GetHeight( ) const = 0;
/*
		private:
			char		*m_pName;
			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;
			ZED_UINT32	m_FileType;
			ZED_FORMAT	m_Format;

			// For the actual storage of the texture
#ifdef ZED_PLATFORM_XBOX // ARGH!!!
			// The Xbox will access the texture only via a D3DTexture object
			IDirect3DTexture8 *m_pD3DTexture;
#endif
			void		*m_pData;*/
		};
	}
}

#endif
