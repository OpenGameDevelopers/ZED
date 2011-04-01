#include <Memory.hpp>
#include <Debugger.hpp>
#include <cstring>
#include <MemoryStructs.hpp>

namespace ZED
{
	namespace System
	{
		void *zedMemCopy( void *p_pDest, const void *p_pSource,
			ZED_MEMSIZE p_Size )
		{
			// Need to create temporary, tangible variables for the next step
			const ZED_BYTE *pSource =
				static_cast< const ZED_BYTE * >( p_pSource );
			ZED_BYTE *pDest = static_cast< ZED_BYTE * >( p_pDest );

			// If the memory doesn't step over another's toes, return memcpy,
			// otherwise, use memmove
			if( ( pSource < pDest && pSource + p_Size > pDest ) ||
				( pDest < pSource && pDest + p_Size > pSource ) )
			{
				zedTrace( "zedMemCopy overlapping.  Using zedMemMove\n" );
				return zedMemMove( p_pDest, p_pSource, p_Size );
			}
			return memcpy( p_pDest, p_pSource, p_Size );
		}

		void *zedMemMove( void *p_pDest, const void *p_pSource,
			ZED_MEMSIZE p_Size )
		{
			return memmove( p_pDest, p_pSource, p_Size );
		}

		void *zedMemSet( void *p_pSource, ZED_INT32 p_Character,
			ZED_MEMSIZE p_Num )
		{
			return memset( p_pSource, p_Character, p_Num );
		}

		ZED_INT32 zedMemCmp( const void *p_pSource1, const void *p_pSource2,
			ZED_MEMSIZE p_Num )
		{
			return memcmp( p_pSource1, p_pSource2, p_Num );
		}
		void *zedMemChr( const void *p_pSource, ZED_INT32 p_Character,
			ZED_MEMSIZE p_Num )
		{
			return memchr( p_pSource, p_Character, p_Num );
		}

#ifdef ZED_BUILD_DEBUG
		void __BlockHeader::SetDebugFilePath( const char *p_pFileName,
			ZED_UINT32 p_LineNumber )
		{
			if( p_pFileName != NULL )
			{
				strncpy( reinterpret_cast< char* >( FileName ), p_pFileName,
					sizeof( FileName ) );
				FileLine = p_LineNumber;
			}
		}
#endif

		void *__BlockHeader::GetDataPtr( )
		{
			ZED_BYTE *pData = reinterpret_cast< ZED_BYTE * >( this );

			pData += DataOffset;

			return static_cast< void * >( pData );
		}

		BlockFooter *__BlockHeader::GetFooterPtr( )
		{
			ZED_BYTE *pData = reinterpret_cast< ZED_BYTE * >( this );

			pData += DataOffset;
			pData -= sizeof( BlockFooter );

			return reinterpret_cast< BlockFooter * >( pData );
		}

		ZED_UINT32 __BlockHeader::GetBlockSize( ZED_UINT32 p_Size,
			ZED_UINT32 p_Align, ZED_UINT32 p_StructAlign )
		{
			ZED_UINT32 Start;
			ZED_UINT32 End;

			// The block size is dependent on the starting pointer
			// since the alignment of the struct and alignment of
			// the data are not identical
			Start = End = reinterpret_cast< ZED_UINT32 >( this );

			End += CalcDataOffset( p_Align );

			// The user's data
			End += Size;

			// Pad it out for the next block header
			End = zedAlign( End, p_StructAlign );

			return ( End - Start );
		}

		ZED_UINT32 __BlockHeader::GetDataSize( )
		{
			return ( Size - DataOffset );
		}

		ZED_UINT32 __BlockHeader::GetChecksum( )
		{
			ZED_UINT32	TempCRC = 0;
			ZED_BYTE	*pData;

			pData = static_cast< ZED_BYTE * >( GetDataPtr( ) );

			for( ZED_UINT32 i = 0; i < Size; ++i )
			{
				TempCRC = *pData++;
			}

			return TempCRC;
		}

		void __BlockHeader::RegisterBlock( ZED_BOOL p_Free,
			ZED_UINT32 p_TotalSize, ZED_UINT32 p_DataSize )
		{
			Free		= p_Free;
			Lock		= ZED_FALSE;
			Size		= p_TotalSize;
			DataOffset	= p_TotalSize - p_DataSize;
			CRC			= 0;

			// Calculate the number of padding bytes required
			ZED_BYTE *pPadding = reinterpret_cast< ZED_BYTE * >( this );
			pPadding += DataOffset;
			pPadding -= sizeof( BlockFooter );

			BlockFooter *pFooter =
				reinterpret_cast< BlockFooter * >( pPadding );
			pFooter->PadSize = DataOffset;
			pFooter->PadSize -=
				ZED_BYTE( sizeof( BlockHeader )+sizeof( BlockFooter ) );
			pFooter->Magic = ZED_MEMORY_MAGIC8;
		}

		ZED_UINT32 __BlockHeader::CalcDataOffset( ZED_UINT32 p_Align )
		{
			ZED_UINT32 Position;
			ZED_UINT32 Start;

			Start = Position = ( ZED_UINT32 )this;//reinterpret_cast< ZED_UINT32 >( this );

			Position += sizeof( BlockHeader );
			Position += sizeof( BlockFooter );
			Position = zedAlign( Position, p_Align );

			return Position - Start;
		}

		Memory::Memory( void *p_pPtr, ZED_MEMSIZE p_Size, ZED_UINT32 p_Align )
		{
			m_Alignment = p_Align;
			m_StructAlignment = sizeof( ZED_MEMSIZE );
			m_PaddedHeaderSize = zedAlign( sizeof( BlockHeader ),
				sizeof( ZED_MEMSIZE ) );
			m_pAllocatedBlock = p_pPtr;
			m_AllocatedSize = p_Size;

			// Invoke ReInitialise to clear the memory block
			ReInitialise( );
		}

		void *Memory::Alloc( ZED_MEMSIZE p_Size, const char *p_pFileName,
			ZED_UINT32 p_LineNumber )
		{
			BlockHeader *pNew;

			if( !( pNew = GetFreeBlock( p_Size ) ) )
			{
				GarbageCollect( );
				pNew = GetFreeBlock( p_Size );
			}
			if( pNew )
			{
#ifdef ZED_BUILD_DEBUG
				pNew->SetDebugFilePath( p_pFileName, p_LineNumber );
#endif
				return pNew->GetDataPtr( );
			}

			return NULL;
		}

		void Memory::Free( void *p_pPtr )
		{
			BlockHeader *pFreed = GetHeader( p_pPtr );

			zedAssert( !pFreed->Free && "Block is already free" );

			pFreed->Free = ZED_TRUE;
		}

		BlockHeader *Memory::GetFreeBlock( ZED_MEMSIZE p_Size )
		{
			BlockHeader *pHeader = m_pFirstBlock;
			BlockHeader *pNew = NULL;
			ZED_MEMSIZE TotalSize = 0;

			while( pHeader )
			{/*
				if( !pHeader->Free )
				{
					// No free RAM available
					continue;
				}
				TotalSize = pHeader->GetBlockSize(
					p_Size, m_Alignment, m_StructAlignment );

				if( TotalSize >= p_Size )*/
				if( pHeader->Free &&
					( TotalSize = pHeader->GetBlockSize( p_Size, m_Alignment,
					m_StructAlignment ) ) >= TotalSize )
				{
					pNew = //( BlockHeader * )( ( ( ZED_BYTE * )pHeader ) + TotalSize );
					reinterpret_cast< BlockHeader * >(
						reinterpret_cast< ZED_BYTE * >( pHeader )+TotalSize );

					// Second half is a new, empty block
					ZED_MEMSIZE FreeTotalSize = pHeader->Size - TotalSize;
					ZED_MEMSIZE FreeOffset;
					ZED_MEMSIZE FreeSize;

					FreeOffset = pNew->CalcDataOffset( m_Alignment );
					FreeSize = FreeTotalSize - FreeOffset;

					if( FreeSize > 0 )
					{
						pNew->RegisterBlock( ZED_TRUE, FreeTotalSize,
							FreeSize );
						pNew->pNext = NULL;

						// First half is used
						pHeader->RegisterBlock( ZED_FALSE, TotalSize, p_Size );
						/*pHeader->pNext = pNew;*/
					}
					else
					{
						// Not enough space for another block!
						pHeader->Free = ZED_FALSE;
					}

					return pHeader;
				}
				pHeader = pHeader->pNext;
			}

			return NULL;
		}

		BlockHeader *Memory::GetHeader( void *p_pPtr )
		{
			ZED_BYTE *pData = static_cast< ZED_BYTE * >( p_pPtr );
			BlockFooter *pFooter;

			pFooter =// (BlockFooter*)(pData - sizeof( BlockFooter ) );
				reinterpret_cast< BlockFooter * >(
					pData - sizeof( BlockFooter ) );

			pData -= sizeof( BlockHeader );
			pData -= sizeof( BlockFooter );
			pData -= pFooter->PadSize;

			//REMOVE!
			zedTrace( "Crude debugging:\n"
				"BlockHeader size: %d\n"
				"BlockFooter size: %d\n"
				"Pad size: %d\n"
				"BlockHeader+Pad size: %d\n\n",
				sizeof( BlockHeader ),
				sizeof( BlockFooter ),
				pFooter->PadSize,
				sizeof( BlockHeader )+pFooter->PadSize );
			//!REMOVE
			return reinterpret_cast< BlockHeader * >( pData );
		}

		void Memory::ReInitialise( )
		{
			// Create a default block that crosses all of the memory space
			// desired
			m_pFirstBlock = static_cast< BlockHeader * >( m_pAllocatedBlock );

			m_pFirstBlock->Free = ZED_TRUE;
			m_pFirstBlock->Size = m_AllocatedSize;
			m_pFirstBlock->pNext = NULL;
#ifdef ZED_BUILD_DEBUG
			m_pFirstBlock->SetDebugFilePath( "Main block", 0 );
#endif
		}

		void Memory::ListBlocks( )
		{
			BlockHeader *pHeader = NULL;

			zedTrace( "[Memory Dump]\nFree: %ld | Used: %ld\n",
				GetFreeMemory( ), GetUsedMemory( ) );

			pHeader = m_pFirstBlock;

			while( pHeader )
			{
#ifdef ZED_BUILD_DEBUG
				zedTrace( "%s[%d] : %s : %8d\n", pHeader->FileName,
					pHeader->FileLine, pHeader->Free ? "FREE" : "USED",
					pHeader->GetDataSize( ) );
#endif

				pHeader = pHeader->pNext;
			}
		}

		void Memory::GarbageCollect( )
		{
			BlockHeader *pHeader = NULL;
			BlockHeader *pNext = NULL;

			pHeader = m_pFirstBlock;

			while( pHeader )
			{
				pNext = pHeader->pNext;

				if( pHeader->Free && pNext && pNext->Free )
				{
					pHeader->Size += pNext->Size;
					pHeader->pNext = pNext->pNext;
				}
				else
				{
					pHeader = pNext;
				}
			}
		}

		ZED_MEMSIZE Memory::GetFreeMemory( )
		{
			BlockHeader *pHeader = NULL;
			ZED_MEMSIZE Mem = 0;

			pHeader = m_pFirstBlock;

			while( pHeader )
			{
				if( pHeader->Free )
				{
					Mem += pHeader->GetDataSize( );
				}
				pHeader = pHeader->pNext;
			}

			return Mem;
		}

		ZED_MEMSIZE Memory::GetUsedMemory( )
		{
			BlockHeader *pHeader = NULL;
			ZED_MEMSIZE Mem = 0;

			pHeader = m_pFirstBlock;

			while( pHeader )
			{
				if( !pHeader->Free )
				{
					Mem += pHeader->GetDataSize( );
				}
				pHeader = pHeader->pNext;
			}

			return Mem;
		}

		void Memory::LockMemory( void *p_pPtr )
		{
			BlockHeader *pBlock = GetHeader( p_pPtr );

			zedAssert( pBlock->Lock == ZED_FALSE &&
				"Block is already locked" );
			pBlock->Lock = ZED_TRUE;

			// Compute the dumb CRC
			pBlock->CRC = pBlock->GetChecksum( );
		}

		void Memory::UnlockMemory( void *p_pPtr )
		{
			BlockHeader *pBlock = GetHeader( p_pPtr );
			ZED_UINT32 CRC;

			zedAssert( pBlock->Lock == ZED_TRUE &&
				"Block is not locked" );
			pBlock->Lock = ZED_FALSE;

			// See if the CRC is corrupt
			CRC = pBlock->GetChecksum( );
			zedAssert( pBlock->CRC == CRC &&
				"Memory has been changed since it was locked!" );
		}
	}
}
