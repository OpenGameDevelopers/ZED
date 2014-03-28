#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLFont.hpp>
#include <System/NativeFile.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLFont::GLFont( )
		{
		}

		GLFont::~GLFont( )
		{
		}

		ZED_UINT32 GLFont::Load( const ZED_CHAR8 *p_pFilePath )
		{
			ZED::System::NativeFile FontFile;

			if( FontFile.Open( p_pFilePath,
				ZED::System::FILE_ACCESS_READ |
					ZED::System::FILE_ACCESS_BINARY ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			if( FontFile.GetSize( ) < sizeof( FONTHEADER ) )
			{
				FontFile.Close( );
				return ZED_FAIL;
			}

			FONTHEADER FontHeader;
			ZED_MEMSIZE ReadSize;

			if( FontFile.ReadByte(
				reinterpret_cast< ZED_BYTE * >( &FontHeader ),
				sizeof( FontHeader ), &ReadSize ) != ZED_OK )
			{
				FontFile.Close( );
				return ZED_FAIL;
			}

			if( ReadSize != sizeof( FontHeader ) )
			{
				FontFile.Close( );
				return ZED_FAIL;
			}

			// Get each chunk from the file and interpret it...
			// The last chunk should be ZED_FILE_END_CHUNK
			
			FontFile.Close( );

			return ZED_OK;
		}
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

