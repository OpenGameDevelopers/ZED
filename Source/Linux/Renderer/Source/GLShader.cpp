#include <GLShader.hpp>
#include <cstdio>
#include <cstring>
#include <GLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShader::GLShader( )
		{
			m_Flags = 0;
		}

		GLShader::GLShader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
			const ZED_BOOL p_Geometry )
		{
			m_Flags = 0;
		}

		ZED_UINT32 GLShader::Compile( const ZED_CHAR8 **p_ppData,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_FromFile )
		{
			// Ignores loading from file and does it by default
			FILE *pFile = fopen( *p_ppData, "rb" );
			GLint CompileStatus = 0;

			zedTrace( "[ZED::Renderer::GLShader::Compile] <INFO> "
				"File: %s\n", *p_ppData );

			// Read the source from the file
			ZED_MEMSIZE FileSize = 0;

			fseek( pFile, 0, SEEK_END );
			FileSize = ftell( pFile );
			rewind( pFile );

			GLchar **ppSource = new GLchar*[ 1 ];
			ppSource[ 0 ] = new GLchar[ FileSize ];

			fread( ppSource[ 0 ], sizeof( ZED_BYTE ), FileSize, pFile );

			// Print the source
			char PrintSource[ FileSize ];
			strncpy( PrintSource, ppSource[ 0 ], FileSize );
			zedTrace( "[ZED::Renderer::GLShader::Compile] <INFO> "
				"Size: %d | Source:\n%s\n", FileSize, PrintSource );

			// Depending on the type of shader, bind with vertex, fragment, or
			// geometry
			if( p_Type == ZED_VERTEX_SHADER )
			{
				// Check for a valid vertex shader
				if( !zglIsShader( m_VertexID ) )
				{
					// Okay, create a shader
					this->AddType( p_Type );
				}

				zglShaderSource( m_VertexID, 1,
					const_cast< const GLchar ** >( ppSource ),
					reinterpret_cast< const GLint *>( &FileSize ) );
				zglCompileShader( m_VertexID );

				// Get the compilation status
				zglGetShaderiv( m_VertexID, GL_COMPILE_STATUS,
					&CompileStatus );

				if( !CompileStatus )
				{
					// Something went wrong
					GLint LogLength = 0;
					GLchar *pLog = ZED_NULL;
					zglGetShaderiv( m_VertexID, GL_INFO_LOG_LENGTH,
						&LogLength );
					pLog = new GLchar[ LogLength ];

					zglGetShaderInfoLog( m_VertexID, LogLength, &LogLength,
						pLog );
					zedTrace( "[ZED::Renderer::GLShader::Compile] <ERROR> "
						"Vertex Shader Log:\n\t%s\n", pLog );

					delete [ ] pLog;

					fclose( pFile );
					pFile = ZED_NULL;

					if( ppSource[ 0 ] != ZED_NULL )
					{
						delete [ ] ppSource[ 0 ];
						delete [ ] ppSource;
					}

					return ZED_GRAPHICS_ERROR;
				}
			}

			if( p_Type == ZED_FRAGMENT_SHADER )
			{
				// Check for a valid fragment shader
				if( !zglIsShader( m_FragmentID ) )
				{
					// Create a shader ID
					this->AddType( p_Type );
				}

				zglShaderSource( m_FragmentID, 1,
					const_cast< const GLchar ** >( ppSource ),
					reinterpret_cast< const GLint *>( &FileSize ) );
				zglCompileShader( m_FragmentID );

				// Get the compilation status
				zglGetShaderiv( m_FragmentID, GL_COMPILE_STATUS,
					&CompileStatus );

				if( !CompileStatus )
				{
					// Somthing went wrong
					GLint LogLength = 0;
					GLchar *pLog = ZED_NULL;
					zglGetShaderiv( m_FragmentID, GL_INFO_LOG_LENGTH,
						&LogLength );
					pLog = new GLchar[ LogLength ];

					zglGetShaderInfoLog( m_FragmentID, LogLength, &LogLength,
						pLog );
					zedTrace( "[ZED::Renderer::GLShader::Compile] <ERROR> "
						"Fragment Shader Log:\n\t%s\n", pLog );

					delete [ ] pLog;

					fclose( pFile );
					pFile = ZED_NULL;

					if( ppSource[ 0 ] != ZED_NULL )
					{
						delete [ ] ppSource[ 0 ];
						delete [ ] ppSource;
					}

					return ZED_GRAPHICS_ERROR;
				}
			}

			if( ppSource[ 0 ] != ZED_NULL )
			{
				delete [ ] ppSource[ 0 ];
				delete [ ] ppSource;
			}

			if( pFile != ZED_NULL )
			{
				fclose( pFile );
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShader::Create( const ZED_UCHAR8 *p_pData,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_FromFile )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::Activate( )
		{
			// Check if the program has not already been linked
			if( !( m_Flags && 0x3 ) )
			{	
				m_ProgramID = zglCreateProgram( );

				if( m_ProgramID == 0 )
				{
					zedTrace( "[ZED::Renderer::GLShader::Activate] <ERROR> "
						"Failed to create program for shader.\n" );
					return ZED_GRAPHICS_ERROR;
				}

				// Attach shaders
				if( zglIsShader( m_VertexID ) )
				{
					zglAttachShader( m_ProgramID, m_VertexID );
				}
				if( zglIsShader( m_FragmentID ) )
				{
					zglAttachShader( m_ProgramID, m_FragmentID );
				}

				zglBindAttribLocation( m_ProgramID, 0, "Position" );
				zglBindAttribLocation( m_ProgramID, 1, "Normal" );
				zglBindAttribLocation( m_ProgramID, 2, "UV" );
				zglBindFragDataLocation( m_ProgramID, 0, "FragColour" );

				// Link the program
				zglLinkProgram( m_ProgramID );

				// Get the link status
				GLint Error;
				zglGetProgramiv( m_ProgramID, GL_LINK_STATUS, &Error );

				if( Error == GL_FALSE )
				{
					zedTrace( "[ZED::Renderer::GLShader::SetUniformTypes] "
						"<ERROR> Failed to link program\n" );
					GLsizei Length = 0;
					GLchar *pLog;

					zglGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH,
						&Length );
					pLog = new GLchar[ Length ];

					zglGetProgramInfoLog( m_ProgramID, Length, &Length, pLog );
					zedTrace( "%s\n", pLog );
					zedAssert( ZED_FALSE );

					delete pLog;

					return ZED_FAIL;
				}

				// Link the program
			//	zglLinkProgram( m_ProgramID );

				// Set the linked flag
				m_Flags |= 0x3;
			}

			if( zglIsProgram( m_ProgramID ) )
			{
				zglUseProgram( m_ProgramID );
			}

			return ZED_OK;
		}

		void GLShader::Deactivate( )
		{
		}

		void GLShader::AddType( const ZED_SHADER_TYPE p_Type )
		{
			if( p_Type == ZED_VERTEX_SHADER )
			{
				m_VertexID = zglCreateShader( GL_VERTEX_SHADER );
			}
			else if( p_Type == ZED_FRAGMENT_SHADER )
			{
				m_FragmentID = zglCreateShader( GL_FRAGMENT_SHADER );
			}
		}

		void GLShader::Delete( )
		{
			if( m_pUniformMap != ZED_NULL )
			{
				delete [ ] m_pUniformMap;
			}

			if( m_VertexID != 0 )
			{
				zglDeleteShader( m_VertexID );
			}
			if( m_FragmentID != 0 )
			{
				zglDeleteShader( m_FragmentID );
			}
			if( m_GeometryID != 0 )
			{
				zglDeleteShader( m_GeometryID );
			}
			if( m_ProgramID != 0 )
			{
				zglDeleteProgram( m_ProgramID );
			}
		}

		ZED_UINT32 GLShader::Save( const ZED_UCHAR8 *p_pFile,
			const ZED_BOOL p_HLSL )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVertexAttributeTypes(
			const ZED_SHADER_VA_MAP *p_pVAMap, const ZED_MEMSIZE p_Count )
		{
			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetUniformTypes(
			const ZED_SHADER_UNIFORM_MAP *p_pUniforms,
			const ZED_MEMSIZE p_Count )
		{
			// The program has to already be linked before calling!
			if( !( m_Flags && 0x3 ) )
			{
				m_ProgramID = zglCreateProgram( );

				if( m_ProgramID == 0 )
				{
					zedTrace( "[ZED::Renderer::GLShader::Activate] <ERROR> "
						"Failed to create program for shader.\n" );
					return ZED_GRAPHICS_ERROR;
				}

				// Attach shaders
				if( zglIsShader( m_VertexID ) )
				{
					zglAttachShader( m_ProgramID, m_VertexID );
				}
				if( zglIsShader( m_FragmentID ) )
				{
					zglAttachShader( m_ProgramID, m_FragmentID );
				}

				zglBindAttribLocation( m_ProgramID, 0, "Position" );
				zglBindAttribLocation( m_ProgramID, 1, "Normal" );
				zglBindAttribLocation( m_ProgramID, 2, "UV" );
				zglBindFragDataLocation( m_ProgramID, 0, "FragColour" );

				// Link the program
				zglLinkProgram( m_ProgramID );

				// Get the link status
				GLint Error;
				zglGetProgramiv( m_ProgramID, GL_LINK_STATUS, &Error );

				if( Error == GL_FALSE )
				{
					zedTrace( "[ZED::Renderer::GLShader::SetUniformTypes] "
						"<ERROR> Failed to link program\n" );
					GLsizei Length = 0;
					GLchar *pLog;

					zglGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH,
						&Length );
					pLog = new GLchar[ Length ];

					zglGetProgramInfoLog( m_ProgramID, Length, &Length, pLog );
					zedTrace( "%s\n", pLog );
					zedAssert( ZED_FALSE );

					delete pLog;

					return ZED_FAIL;
				}

				// Set the linked flag
				m_Flags |= 0x3;
			}

			m_pUniformMap = new ZED_SHADER_UNIFORM_MAP[ p_Count ];

			for( ZED_MEMSIZE i = 0; i < p_Count; i++ )
			{
				m_pUniformMap[ i ].Location = zglGetUniformLocation(
					m_ProgramID, p_pUniforms[ i ].pName );
				m_pUniformMap[ i ].Index = p_pUniforms[ i ].Index;
				m_pUniformMap[ i ].Type = p_pUniforms[ i ].Type;
#ifdef ZED_BUILD_DEBUG
				m_pUniformMap[ i ].pName = p_pUniforms[ i ].pName;
#endif
			}	

			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVariable( const ZED_UINT32 p_Index,
			const void *p_pValue )
		{
			switch( m_pUniformMap[ p_Index ].Type )
			{
			case ZED_FLOAT1:
				{
					zglUniform1f( m_pUniformMap[ p_Index ].Location,
						*( reinterpret_cast< const GLfloat * >( &p_pValue ) ) );
					break;
				}
			case ZED_FLOAT3:
				{
					zglUniform3fv( m_pUniformMap[ p_Index ].Location, 1,
						static_cast< const GLfloat * >( p_pValue ) );
					break;
				}
			case ZED_INT1:
				{
					zglUniform1i( m_pUniformMap[ p_Index ].Location,
						*( reinterpret_cast< const GLint * >( &p_pValue ) ) );
					break;
				}
			case ZED_MAT4X4:
				{
					zglUniformMatrix4fv( m_pUniformMap[ p_Index ].Location, 1,
						ZED_FALSE,
						static_cast< const GLfloat * >( p_pValue ) );
					break;
				}
			}
			return ZED_OK;
		}
	}
}

