#ifndef __ZED_SYSTEM_MEMORYSTRUCTS_HPP__
#define __ZED_SYSTEM_MEMORYSTRUCTS_HPP__

namespace ZED
{
	namespace System
	{
		typedef struct __BlockFooter
		{
			ZED_BYTE	PadSize;
			ZED_BYTE	Magic;
		}BlockFooter;

		typedef struct __BlockHeader
		{
			ZED_BYTE		Free;
			ZED_BYTE		Lock;
			ZED_UINT16		Padding;
			ZED_UINT32		Size;
			ZED_UINT32		DataOffset;
			__BlockHeader	*pNext;

#ifdef ZED_BUILD_DEBUG
			ZED_UINT32	FileLine;
			ZED_UCHAR8	FileName[ ZED_MAX_PATH ];
			ZED_UCHAR8	Padding2;
			ZED_UINT16	Padding3;
			ZED_UINT32	CRC;

			// Sometimes, the ZED_* types just won't work with the
			// libraries, no matter what kind of crazy casting is performed
			void		SetDebugFilePath( const char *p_pFileName,
						ZED_UINT32 p_LineNumber );
#endif
			void		*GetDataPtr( );
			BlockFooter	*GetFooterPtr( );
			ZED_UINT32	GetBlockSize( ZED_UINT32 p_Size,
						ZED_UINT32 p_Align, ZED_UINT32 p_StructAlign );
			ZED_UINT32	GetDataSize( );
			ZED_UINT32	GetChecksum( );
			void		RegisterBlock( ZED_BOOL p_Free,
						ZED_UINT32 p_TotalSize, ZED_UINT32 p_DataSize );
			ZED_UINT32	CalcDataOffset( ZED_UINT32 p_Align );
		}BlockHeader;
	}
}

#endif
