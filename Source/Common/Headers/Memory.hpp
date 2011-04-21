#ifndef __ZEDMEMORY_HPP__
#define __ZEDMEMORY_HPP__

#include <DataTypes.hpp>
#include <MemoryStructs.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_UINT32	ZED_MEMORY_MAGIC32	=	0x600DC0DE;
		const ZED_BYTE		ZED_MEMORY_MAGIC8		=	0xA9;

		// Safe[r] memory copy wrapper
		void *zedMemCopy( void *p_pDest, const void *p_pSource,
			ZED_MEMSIZE p_Size );

		// Wrappers around mem* functions
		void *zedMemMove( void *p_pDest, const void *p_pSource,
			ZED_MEMSIZE p_Size );
		void *zedMemSet( void *p_pSource, ZED_INT32 p_Char,
			ZED_MEMSIZE p_Num );
		ZED_INT32 zedMemCmp( const void *p_pSource1, const void *p_pSource2,
			ZED_MEMSIZE p_Num );
		void *zedMemChr( const void *p_pSource, ZED_INT32 p_Character,
			ZED_MEMSIZE p_Num );

		class Memory
		{
		public:
			Memory( void *p_pPtr, ZED_MEMSIZE p_Size, ZED_UINT32 p_Align );

			void *Alloc( ZED_MEMSIZE p_Size, const char *p_pFileName,
				 ZED_UINT32 p_LineNumber );
			void Free( void *p_pPtr );

			BlockHeader	*GetFreeBlock( ZED_MEMSIZE p_Size );
			// Convert a data pointer into a block pointer
			BlockHeader *GetHeader( void *p_pPtr );

			void	ReInitialise( );
			void	ListBlocks( );
			void	GarbageCollect( );

			ZED_MEMSIZE	GetFreeMemory( );
			ZED_MEMSIZE GetUsedMemory( );

			void	LockMemory( void *p_pPtr );
			void	UnlockMemory( void *p_pPtr );

		private:
			// No default constructor allowed!
			ZED_INLINE Memory( ){ }

			BlockHeader	*m_pFirstBlock;
			ZED_UINT32	m_Alignment;
			ZED_UINT32	m_StructAlignment;
			ZED_UINT32	m_PaddedHeaderSize;
			ZED_UINT32	m_AllocatedSize;
			void		*m_pAllocatedBlock;
		};
	}
}

#endif
