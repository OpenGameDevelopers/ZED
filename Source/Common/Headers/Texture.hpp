#ifndef __ZED_RENDERER_TEXTURE_HPP__
#define __ZED_RENDERER_TEXTURE_HPP__

#include <Renderer.hpp>

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
#pragma pack( )

		// PNG CRC calculation
		// Ergh, a global!
		static ZED_BOOL		g_PNGCRCTableInit = ZED_FALSE;
		static ZED_ULONG	g_PNGCRCTable[ 256 ];

		// Create the CRC table [http://www.w3.org/TR/PNG/#D-CRCAppendix]
		void PNGCRCTableCreate( );
		// Update a running CRC with the buffer's bytes
		ZED_ULONG PNGUpdateCRC( ZED_ULONG p_CRC, ZED_BYTE *p_pBuffer,
			ZED_INT32 p_Length );
		ZED_ULONG PNGCRC( ZED_BYTE *p_pBuffer, ZED_INT32 p_Length );

		class Texture
		{
		public:
			Texture( );
			~Texture( );

			ZED_UINT32 Load( const ZED_CHAR16 *p_pFilename );

			void SetData( const void *p_pData );
			void SetFormat( const ZED_FORMAT *p_Format );
			void SetWidth( const ZED_UINT32 p_Width );
			void SetHeight( const ZED_UINT32 p_Heght );

			void		*GetData( ) const;
			ZED_FORMAT	GetFormat( ) const;
			ZED_UINT32	GetWidth( ) const;
			ZED_UINT32	GetHeight( ) const;

		private:
			void		*m_pData;
			ZED_UCHAR8	*m_pName;
			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;
			ZED_UINT32	m_FileType;
			ZED_FORMAT	m_Format;
		};
	}
}

#endif
