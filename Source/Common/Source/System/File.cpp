#include <System/File.hpp>

namespace ZED
{
	namespace System
	{
		File::File( ) :
			m_Open( ZED_FALSE )
		{
		}

		File::~File( )
		{
		}

		ZED_BOOL File::IsOpen( ) const
		{
			return m_Open;
		}
	}
}


