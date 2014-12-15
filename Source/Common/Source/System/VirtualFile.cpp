#include <System/VirtualFile.hpp>
#include <System/FileContainer.hpp>

namespace ZED
{
	namespace System
	{
		VirtualFile::VirtualFile( FileContainer *p_pFileContainer,
			const ZED_MEMSIZE p_StartingPosition,
			const ZED_MEMSIZE p_FileSize )
		{
			m_StartingPosition = p_StartingPosition;
			m_Size = p_FileSize;
		}

		VirtualFile::~VirtualFile( )
		{
		}

		ZED_UINT32 VirtualFile::Open( const ZED_CHAR8 *p_pFileName,
			const ZED_UINT32 p_Access )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::Close( )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::Seek( const ZED_MEMSIZE p_Offset,
			const FILE_SEEK p_Start )
		{
			return ZED_FAIL;
		}

		ZED_MEMSIZE VirtualFile::GetPosition( ) const
		{
			return 0;
		}

		ZED_UINT32 VirtualFile::Rewind( )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::WriteByte( const ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::WriteUInt32( const ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::WriteString( const ZED_CHAR8 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::ReadByte( ZED_BYTE *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::ReadUInt32( ZED_UINT32 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			return ZED_FAIL;
		}

		ZED_UINT32 VirtualFile::ReadString( ZED_CHAR8 *p_pData,
			const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead )
		{
			return ZED_FAIL;
		}

		ZED_CHAR8 *VirtualFile::GetName( ) const
		{
			return m_pFileName;
		}
	}
}

