#ifndef __ZED_SYSTEM_FILESYSTEM_HPP__
#define __ZED_SYSTEM_FILESYSTEM_HPP__

#include <System/DataTypes.hpp>
#include <vector>
#include <string>
#include <map>

namespace ZED
{
	namespace System
	{
		class FileContainer;
		class File;
		class VirtualFile;

		// The file system itself is just an intermediary between what the
		// user wants, and what the user gets
		class FileSystem
		{
		public:
			FileSystem( );
			~FileSystem( );

			// Files are opened by using URIs to reference the file on disc,
			// there is only one built-in type, with more modular ones to
			// follow:
			// native:	Native file system
			//
			// When a file from mass storage is opened, all the files within
			// are expaned and are available via the file system, for example;
			// if there is a file contained in a ZED file archive called
			// "Audio/Weapons/Colt Python/Hammer Back.ogg", the ZED archive
			// would first be opened, then the file can be accessed:
			// FileSyste.OpenFile( "zed://Content/WeaponAudio.zed" );
			// File *pColtPythonHammerBack = FileSystem.OpenFile(
			// 	"Audio/Weapons/Colt Python/Hammer Back.ogg" );
			//
			// With no URI present, the file names are used as if they are
			// resident at a single directory level
			File *OpenFile( const ZED_CHAR8 *p_pFileName );
			void CloseFile( File *p_pFile );

			ZED_UINT32 RegisterFileContainer(
				FileContainer *p_pFileContainer );
			
		private:
			FileSystem( const FileSystem &p_Copy );
			FileSystem &operator=( const FileSystem &p_Clone );

			std::vector< FileContainer * >			m_FileContainerArray;
			std::map< std::string, VirtualFile * >	m_FileMap;
		};
	}
}

#endif // __ZED_SYSTM_FILESYSTEM_HPP__

