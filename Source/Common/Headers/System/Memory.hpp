#ifndef __ZED_SYSTEM_MEMORY_HPP__
#define __ZED_SYSTEM_MEMORY_HPP__

#include <System/DataTypes.hpp>
#include <System/MemoryStructs.hpp>

#define zedSafeDelete( p_pVar )\
	{ if( p_pVar ){ delete p_pVar; p_pVar = ZED_NULL; } }
#define zedSafeDeleteArray( p_pVar )\
	{ if( p_pVar ){ delete [ ] p_pVar; p_pVar = ZED_NULL; } }

namespace ZED
{
	namespace System
	{
		const ZED_UINT32	ZED_MEMORY_MAGIC32	=	0x600DC0DE;
		const ZED_BYTE		ZED_MEMORY_MAGIC8	=	0xA9;

		// Safe[r] memory copy wrapper
		void *zedMemCopy( void *p_pDest, const void *p_pSource,
			ZED_MEMSIZE p_Size );

		// Wrappers around mem* functions
		void *zedMemMove( void *p_pDest, const void *p_pSource,
			ZED_MEMSIZE p_Size );
		void *zedMemSet( void *p_pSource, ZED_SINT32 p_Char,
			ZED_MEMSIZE p_Num );
		ZED_SINT32 zedMemCmp( const void *p_pSource1, const void *p_pSource2,
			ZED_MEMSIZE p_Num );
		void *zedMemChr( const void *p_pSource, ZED_SINT32 p_Character,
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
			Memory( );
			Memory( const Memory &p_Assign );
			Memory &operator=( const Memory &p_Copy );

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
