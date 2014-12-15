#ifndef __ZED_SYSTEM_VIRTUALFILE_HPP__
#define __ZED_SYSTEM_VIRTUALFILE_HPP__

#include <System/File.hpp>

namespace ZED
{
	namespace System
	{
		class FileContainer;

		class VirtualFile : public File
		{
		public:
			VirtualFile( FileContainer *p_pFileContainer,
				const ZED_MEMSIZE p_StartingPosition,
				const ZED_MEMSIZE p_FileSize );
			virtual ~VirtualFile( );

			virtual ZED_UINT32 Open( const ZED_CHAR8 *p_pFileName,
				const ZED_UINT32 p_Access );
			virtual ZED_UINT32 Close( );

			virtual ZED_UINT32 Seek( const ZED_MEMSIZE p_Offset,
				const FILE_SEEK p_Start );
			virtual ZED_MEMSIZE GetPosition( ) const;
			virtual ZED_UINT32 Rewind( );

			virtual ZED_UINT32 WriteByte( const ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			virtual ZED_UINT32 WriteUInt32( const ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );
			virtual ZED_UINT32 WriteString( const ZED_CHAR8 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pWritten );

			virtual ZED_UINT32 ReadByte( ZED_BYTE *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );
			virtual ZED_UINT32 ReadUInt32( ZED_UINT32 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );
			virtual ZED_UINT32 ReadString( ZED_CHAR8 *p_pData,
				const ZED_MEMSIZE p_Count, ZED_MEMSIZE *p_pRead );

			ZED_CHAR8 *GetName( ) const;

		private:
			ZED_MEMSIZE		m_StartingPosition;
			FileContainer	*m_pFileContainer;
			ZED_CHAR8		*m_pFileName;
		};
	}
}


#endif // __ZED_SYSTEM_VIRTUALFILE_HPP__

