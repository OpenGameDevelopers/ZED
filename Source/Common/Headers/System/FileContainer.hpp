#ifndef __ZED_SYSTEM_FILECONTAINER_HPP__
#define __ZED_SYSTEM_FILECONTAINER_HPP__

#include <System/DataTypes.hpp>
#include <vector>

namespace ZED
{
	namespace System
	{
		class VirtualFile;

		class FileContainer
		{
		public:
			virtual ~FileContainer( );

			virtual ZED_CHAR8 *GetName( ) const = 0;

			virtual ZED_UINT32 ExtractFiles(
				std::vector< VirtualFile * > p_FileArray ) = 0;

		protected:
		};
	}
}

#endif // __ZED_SYSTEM_FILECONTAINER_HPP__

