#include <System/File.hpp>

namespace ZED
{
	namespace System
	{
		File::File( ) :
			m_Open( ZED_FALSE ),
			m_Size( 0 ),
			m_FileDescriptor( ZED_INVALID_FILE_DESCRIPTOR )
		{
		}

		File::~File( )
		{
		}

		ZED_MEMSIZE File::GetSize( ) const
		{
			return m_Size;
		}

		ZED_BOOL File::IsOpen( ) const
		{
			return m_Open;
		}

		ZED_FILE_DESCRIPTOR File::GetFileDescriptor( ) const
		{
			return m_FileDescriptor;
		}
	}
}


