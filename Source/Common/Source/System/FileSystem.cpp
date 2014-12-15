#include <System/FileSystem.hpp>
#include <System/File.hpp>
#include <System/FileContainer.hpp>
#include <string>

namespace ZED
{
	namespace System
	{
		FileSystem::FileSystem( )
		{
		}

		FileSystem::~FileSystem( )
		{
		}

		File *FileSystem::OpenFile( const ZED_CHAR8 *p_pFileName )
		{
			if( p_pFileName == ZED_NULL )
			{
				return ZED_NULL;
			}

			std::string FileName = p_pFileName;

			// Extract the URI (if there is one)
			ZED_MEMSIZE ColonLocation = FileName.find_first_of( ':' );

			if( ColonLocation == std::string::npos )
			{
				// Try and load one of the files in the file system
			}

			std::vector< FileContainer * >::iterator Container 
				= m_FileContainerArray.begin( );

			std::string URI = FileName.substr( 0, ColonLocation );

			while( Container != m_FileContainerArray.end( ) )
			{
				if( URI.compare( ( *Container )->GetName( ) ) == 0 )
				{
					break;
				}
				++Container;
			}

			if( Container == m_FileContainerArray.end( ) )
			{
				return ZED_NULL;
			}

			std::vector< VirtualFile * > FileArray;

			( *Container )->ExtractFiles( FileArray );

			std::vector< VirtualFile * >::iterator FileItr =
				FileArray.begin( );

			while( FileItr != FileArray.end( ) )
			{
				++FileItr;
			}
			/*

			for each file in FileArray
				compare file names and their checksums
					if the checksums match, continue
					else return null, as duplicate file names with different data are not permitted
			*/
		}
	}
}

