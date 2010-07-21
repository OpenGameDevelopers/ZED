#include <GLExtender.hpp>
#include <Renderer.hpp>
#include <Debugger.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLExtender::GLExtender( HDC p_HDC ) :
			m_HDC( p_HDC )
		{
			// Check that m_HDC is valid
			if( !m_HDC )
			{
				zedTrace( "GLExtender: HDC was not set in constructor" );
			}

			RegisterBaseWGLExtensions( );
			
			// OPT!
			// Here comes the slow part!

			// Get the list of supported extensions
			const char *WGLExtensions = wglGetExtensionsStringARB( m_HDC );

			ZED_BOOL Loop = ZED_TRUE;

			// Let's hope that no extension exceeds 64 characters!
			char CurrentExtension[ 64 ] = { 0 };
			ZED_UINT64 CharCount = 0;
			ZED_UINT64 Position = 0;

			// Read all the Extensions into m_Extensions
			do
			{
				CurrentExtension[ Position++ ] = WGLExtensions[ CharCount ];

				// Check for the space character
				if( WGLExtensions[ CharCount + 1 ] == 0x20 )
				{
					// The CurrentExtension has bad data at the end, cull it
					std::string CopyString;
					CopyString.insert( 0, CurrentExtension, Position );

					m_Extensions.push_back( CopyString );

					// Reset the position to copy into
					Position = 0;

					// Increment the lost char
					CharCount++;
				}

				CharCount++;

				// End of the line
				if( WGLExtensions[ CharCount ] == 0x00 )
				{
					Loop = ZED_FALSE;
				}
				
			}while( Loop );
			// !OPT
		}

		GLExtender::~GLExtender( )
		{
		}

		ZED_BOOL GLExtender::IsGLExtSupported( const char *p_Extension )
		{
			// Check that the HDC is valid
			if( m_HDC == NULL )
			{
				return ZED_FALSE;
			}
			
			ZED_BOOL Return = ZED_FALSE;

			// OPT!
			// Check that the extension can be found
			std::list< std::string >::iterator ExtItr = m_Extensions.begin( );

			// Maybe this should be hashed to speed up the finding?
			while( ( Return == ZED_FALSE ) &&
				( ExtItr != m_Extensions.end( ) ) )
			{
				if( ( *ExtItr ).compare( p_Extension ) == 0 )
				{
					Return = ZED_TRUE;
				}

				ExtItr++;
			}
			// !OPT

			return Return;
		}

		void GLExtender::RegisterBaseWGLExtensions( )
		{
			wglGetExtensionsStringARB =
				( PFNWGLGETEXTENSIONSSTRINGARBPROC )
				wglGetProcAddress( "wglGetExtensionsStringARB" );

			wglCreateContextAttribsARB =
				( PFNWGLCREATECONTEXTATTRIBSARBPROC )
				wglGetProcAddress( "wglCreateContextAttribsARB" );
		}
	}
}



