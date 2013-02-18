#include <OGL/GLShader.hpp>
#include <OGL/GLExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShader::GLShader( )
		{
			m_pConstantMap = ZED_NULL;
			m_VertexID = m_FragmentID = m_GeometryID = m_ProgramID = 0;
			m_Flags = 0;
			
#ifdef ZED_BUILD_DEBUG
			m_ppVertexSrc = ZED_NULL;
			m_ppFragmentSrc = ZED_NULL;
			m_ppGeometrySrc = ZED_NULL;
#endif
		}

		GLShader::GLShader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
			const ZED_BOOL p_Geometry )
		{
			m_pConstantMap = ZED_NULL;
			m_VertexID = m_FragmentID = m_GeometryID = m_ProgramID = 0;
			m_Flags = 0;

#ifdef ZED_BUILD_DEBUG
			m_ppVertexSrc = ZED_NULL;
			m_ppFragmentSrc = ZED_NULL;
			m_ppGeometrySrc = ZED_NULL;
#endif

			if( p_Vertex == ZED_TRUE )
			{
				m_VertexID = zglCreateShader( GL_VERTEX_SHADER );
			}
			if( p_Fragment == ZED_TRUE )
			{
				m_FragmentID = zglCreateShader( GL_FRAGMENT_SHADER );
			}
		}


		ZED_UINT32 GLShader::Compile( const ZED_CHAR8 **p_ppData,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_FromFile )
		{
			// Common locals
			ZED_SINT32 CompileStatus = 0;

			// If p_FromFile is true, p_Data is the filepath
			if( p_FromFile == ZED_FALSE )
			{
				// Get the Data as a GLchar pointer pointer
				const GLchar **ppData =
					reinterpret_cast< const GLchar** >( p_ppData );

				if( p_Type == ZED_VERTEX_SHADER )
				{
					// Check for a valid shader
					if( !zglIsShader( m_VertexID ) )
					{
						// Add the shader type
						this->AddType( p_Type );
					}
					zglShaderSource( m_VertexID, 1, ppData, ZED_NULL );
					zglCompileShader( m_VertexID );

					// Did the shader compilation go well?
					zglGetShaderiv( m_VertexID, GL_COMPILE_STATUS,
						&CompileStatus );

					if( !CompileStatus )
					{
						// Something went horribly wrong...
						zedAssert( ZED_FALSE );
						ZED_SINT32 LogLength;
						GLchar *pLog = ZED_NULL;
						zglGetShaderiv( m_VertexID, GL_INFO_LOG_LENGTH,
							&LogLength );
						pLog = new GLchar[ LogLength ];
						zglGetShaderInfoLog( m_VertexID, LogLength, &LogLength,
							pLog );
						zedTrace( "[GLShader | Compile | FAILED] Vertex Shader"
							" Log:\n\t%s\n", pLog );
						delete [ ] pLog;
					}
					/*
#ifdef ZED_BUILD_DEBUG
					else
					{
						// Copy the source for debugging
						// Initialise the inner and outer chars to something
						// other than '\0'
						ZED_CHAR8 OuterChar = **p_ppData, InnerChar = *p_ppData[ 0 ];

						while( OuterChar != '\0' )
						{
							// Copy the lines over to m_ppVertexSrc
							while( InnerChar != '\0' )
							{
							}
						}
					}
#endif
					*/
				}
				if( p_Type == ZED_FRAGMENT_SHADER )
				{
					if( !zglIsShader( m_FragmentID ) )
					{
						this->AddType( p_Type );
					}
					zglShaderSource( m_FragmentID, 1, ppData, ZED_NULL );
					zglCompileShader( m_FragmentID );

					// Did the shader compile go well?
					zglGetShaderiv( m_FragmentID, GL_COMPILE_STATUS,
						&CompileStatus );

					if( !CompileStatus )
					{
						// Something went horribly wrong
						zedAssert( ZED_FALSE );
						ZED_SINT32 LogLength;
						GLchar *pLog = ZED_NULL;

						zglGetShaderiv( m_FragmentID, GL_INFO_LOG_LENGTH,	
							&LogLength );

						pLog = new GLchar[ LogLength ];

						zglGetShaderInfoLog( m_FragmentID, LogLength,
							&LogLength, pLog );

						zedTrace( "[ZED | GLShader | Compile( ) | FAILED] "
							"Fragment Shader Log:\n\t%s\n", pLog );

						delete pLog;
					}
				}
			}
			else
			{
				// Extract the source from the file passed in
				FILE *pFile = fopen( *p_ppData, "rb" );

				if( pFile == ZED_NULL )
				{
					zedTrace( "[ZED::Renderer::GLShader::Compile] <ERROR> "
						"Failed to open file: %s\n", *p_ppData );
					return ZED_FAIL;
				}

				GLint CompileStatus = 0;

				// Read the source from the file
				ZED_MEMSIZE FileSize = 0;

				// Get the file size for zglShaderSource
				fseek( pFile, 0, SEEK_END );
				FileSize = ftell( pFile );
				rewind( pFile );

				GLchar **ppSource = new GLchar*[ 1 ];
				ppSource[ 0 ] = new GLchar[ FileSize ];
				
				fread( ppSource[ 0 ], sizeof( ZED_BYTE ), FileSize, pFile );

				// Print the source
				char *pPrintSource = ZED_NULL;
				pPrintSource = new char[ FileSize+1 ];
				strncpy( pPrintSource, ppSource[ 0 ], FileSize );
				// Add a zero to ensure when tracing, it doesn't go into unkown
				// memory space
				pPrintSource[ FileSize ] = '\0';
				zedTrace( "[ZED::Renderer::GLShader::Compile] <INFO> "
					"Size: %d\nSource\n******\n%s\n", FileSize, pPrintSource );

				delete [ ] pPrintSource;
				pPrintSource = ZED_NULL;

				// Compile as a vertex, fragment, or geometry shader
				if( p_Type == ZED_VERTEX_SHADER )
				{
					// Check for a valid vertex shader ID
					if( !zglIsShader( m_VertexID ) )
					{
						// Create a shader
						this->AddType( p_Type );
					}

					zglShaderSource( m_VertexID, 1,
						const_cast< const GLchar ** >( ppSource ),
						reinterpret_cast< const GLint * >( &FileSize ) );
					zglCompileShader( m_VertexID );

					// Was the compile successful?
					zglGetShaderiv( m_VertexID, GL_COMPILE_STATUS,
						&CompileStatus );

					if( !CompileStatus )
					{
						GLsizei LogLength = 0;
						GLchar *pLog = ZED_NULL;
						zglGetShaderiv( m_VertexID, GL_INFO_LOG_LENGTH,
							&LogLength );
						pLog = new GLchar[ LogLength ];

						zglGetShaderInfoLog( m_VertexID, LogLength, &LogLength,
							pLog );

						zedTrace( "[ZED::Renderer::GLShader::Compile] <ERROR> "
							"Vertex Shader Log:\n%s\n", pLog );

						delete [ ] pLog;

						if( pFile != ZED_NULL )
						{
							fclose( pFile );
							pFile = ZED_NULL;
						}

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
					// Check for a valid vertex shader ID
					if( !zglIsShader( m_FragmentID ) )
					{
						// Create a shader
						this->AddType( p_Type );
					}

					zglShaderSource( m_FragmentID, 1,
						const_cast< const GLchar ** >( ppSource ),
						reinterpret_cast< const GLint * >( &FileSize ) );
					zglCompileShader( m_FragmentID );

					// Was the compile successful?
					zglGetShaderiv( m_FragmentID, GL_COMPILE_STATUS,
						&CompileStatus );

					if( !CompileStatus )
					{
						GLsizei LogLength = 0;
						GLchar *pLog = ZED_NULL;
						zglGetShaderiv( m_FragmentID, GL_INFO_LOG_LENGTH,
							&LogLength );
						pLog = new GLchar[ LogLength ];

						zglGetShaderInfoLog( m_FragmentID, LogLength,
							&LogLength, pLog );

						zedTrace( "[ZED::Renderer::GLShader::Compile] <ERROR> "
							"Fragment Shader Log:\n%s\n", pLog );

						delete [ ] pLog;

						if( pFile != ZED_NULL )
						{
							fclose( pFile );
							pFile = ZED_NULL;
						}

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
					ppSource[ 0 ] = ZED_NULL;

					delete [ ] ppSource;
					ppSource = ZED_NULL;
				}

				if( pFile != ZED_NULL )
				{
					fclose( pFile );
					pFile = ZED_NULL;
				}
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
			// Check if the shader has been linked already
			if( !( m_Flags && 0x3 ) )
			{
				m_ProgramID = zglCreateProgram( );

				// Something went wrong
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

				// Need to set up the attributes (if any)
				for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
				{
					/*switch( m_pAttributes[ i ].Type )
					{
						case ZED_VERTEX_SHADER:
						{*/
							zglBindAttribLocation( m_ProgramID,
								m_pAttributes[ i ].Index,
								m_pAttributes[ i ].pName );
						/*	break;
						}
						case ZED_FRAGMENT_SHADER:
						{
							zglBindFragDataLocation( m_ProgramID, 
								m_pAttributes[ i ].Index,
								m_pAttributes[ i ].pName );
							break;
						}
						default:
						{
							zedTrace( "[ZED::Renderer::GLShader::"
								"SetUniformTypes] <WARN> Unknown shader "
								"attribute type.\n" );
						}
					}*/
				}
				
				zglLinkProgram( m_ProgramID );

				// Was this linked properly?
				GLint Error;
				zglGetProgramiv( m_ProgramID, GL_LINK_STATUS, &Error );

				if( Error == GL_FALSE )
				{
					zedTrace( "[ZED::Renderer::GLShader::Activate] <ERROR> "
						"Failed to link program.\n" );
					GLsizei Length = 0;
					GLchar *pLog = ZED_NULL;

					zglGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH,
						&Length );
					pLog = new GLchar[ Length ];

					zglGetProgramInfoLog( m_ProgramID, Length, &Length, pLog );
					zedTrace( "%s\n", pLog );
					
					delete [ ] pLog;

					return ZED_FAIL;
				}

				// Set the linked flag
				m_Flags |= 0x3;
			}

			// NO CHECKING OF VALID PROGRAM!
			if( zglIsProgram( m_ProgramID ) )
			{
				zglUseProgram( m_ProgramID );
			}

			return ZED_OK;
		}

		void GLShader::Deactivate( )
		{
			// Don't do anything, as when zglUseProgram is called, the
			// previous program isn't in use while the current one is
		}

		void GLShader::AddType( const ZED_SHADER_TYPE p_Type )
		{
			if( p_Type == ZED_VERTEX_SHADER )
			{
				m_VertexID = zglCreateShader( GL_VERTEX_SHADER );
			}
			if( p_Type == ZED_FRAGMENT_SHADER )
			{
				m_FragmentID = zglCreateShader( GL_FRAGMENT_SHADER );
			}
		}

		void GLShader::Delete( )
		{
			// Unload everything associated with this shader
			if( m_pConstantMap != ZED_NULL )
			{
				delete [ ] m_pConstantMap;
				m_pConstantMap = ZED_NULL;
			}

			if( m_pAttributes != ZED_NULL )
			{
				delete [ ] m_pAttributes;
				m_pAttributes = ZED_NULL;
			}

			if( zglIsShader( m_VertexID ) )
			{
				zglDeleteShader( m_VertexID );
			}
			if( zglIsShader( m_FragmentID ) )
			{
				zglDeleteShader( m_FragmentID );
			}
			if( zglIsShader( m_GeometryID ) )
			{
				zglDeleteShader( m_GeometryID );
			}
			if( zglIsProgram( m_ProgramID ) )
			{
				zglDeleteProgram( m_ProgramID );
			}
		}

#ifdef ZED_BUILD_DEBUG
		ZED_UINT32 GLShader::Save( const ZED_UCHAR8 *p_pFile,
			const ZED_BOOL p_HLSL )
		{
			return ZED_OK;
		}
#endif

		ZED_UINT32 GLShader::SetVertexAttributeTypes(
			const ZED_SHADER_VERTEXATTRIBUTE *p_pAttributes,
			const ZED_MEMSIZE p_Count )
		{
			m_pAttributes = new ZED_SHADER_VERTEXATTRIBUTE_GL[ p_Count ];
			m_AttributeCount = p_Count;

			// If already linked, set the linked flag to zero
			if( ( m_Flags && 0x3 ) )
			{
				m_Flags != 0x3;
			}

			// Copy over the attributes (need to copy the string in this
			// manner)
			ZED_UINT32 VertexIndex = 0;
			for( ZED_MEMSIZE i = 0; i < p_Count; i++ )
			{
				m_pAttributes[ i ].Index = VertexIndex;
				VertexIndex++;

				m_pAttributes[ i ].Type = p_pAttributes[ i ].Type;

				const ZED_SHADER_VERTEXATTRIBUTE_GL *pGLAttr =
					reinterpret_cast< const ZED_SHADER_VERTEXATTRIBUTE_GL * >(
						&(p_pAttributes[ i ]) );
				ZED_MEMSIZE NameSize = strlen( pGLAttr->pName );
				m_pAttributes[ i ].pName = new char[ NameSize ];
				strcpy( m_pAttributes[ i ].pName, pGLAttr->pName );
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetConstantTypes(
			const ZED_SHADER_CONSTANT_MAP *p_pTypes,
			const ZED_MEMSIZE p_Count )
		{
			// Link before getting the location
			// Check if the shader has been linked already
			if( !( m_Flags && 0x3 ) )
			{
				// Link the program
				m_ProgramID = zglCreateProgram( );

				if( m_ProgramID == 0 )
				{
					zedTrace( "[ZED::Renderer::GLShader::Activate] <ERROR> "
						"Failed to create program for shader.\n" );
					return ZED_GRAPHICS_ERROR;
				}

				// Attach shader
				if( zglIsShader( m_VertexID ) )
				{
					zglAttachShader( m_ProgramID, m_VertexID );
				}
				if( zglIsShader( m_FragmentID ) )
				{
					zglAttachShader( m_ProgramID, m_FragmentID );
				}
				
				// Need to set up the attributes (if any)
				for( ZED_MEMSIZE i = 0; i < m_AttributeCount; i++ )
				{
					switch( m_pAttributes[ i ].Type )
					{
						case ZED_VERTEX_SHADER:
						{
							zglBindAttribLocation( m_ProgramID,
								m_pAttributes[ i ].Index,
								m_pAttributes[ i ].pName );
							break;
						}
						case ZED_FRAGMENT_SHADER:
						{
							zglBindFragDataLocation( m_ProgramID, 
								m_pAttributes[ i ].Index,
								m_pAttributes[ i ].pName );
							break;
						}
						default:
						{
							zedTrace( "[ZED::Renderer::GLShader::"
								"SetUniformTypes] <WARN> Unknown shader "
								"attribute type.\n" );
						}
					}
				}

				zglLinkProgram( m_ProgramID );

				// Was this linked properly?
				GLint Error;
				zglGetProgramiv( m_ProgramID, GL_LINK_STATUS, &Error );

				if( Error == GL_FALSE )
				{
					zedTrace( "[ZED::Renderer::GLShader::SetUnifromTypes] "
						"<ERROR> Failed to link program.\n" );
					GLsizei Length = 0;
					GLchar *pLog = ZED_NULL;

					zglGetProgramiv( m_ProgramID, GL_INFO_LOG_LENGTH,
						&Length );
					pLog = new GLchar[ Length ];

					zglGetProgramInfoLog( m_ProgramID, Length, &Length, pLog );
					zedTrace( "%s\n", pLog );
					
					delete [ ] pLog;

					return ZED_FAIL;
				}

				// Set the linked status
				m_Flags |= 0x3;
			}

			// NO ERROR CHECKING!
			m_pConstantMap = new ZED_SHADER_CONSTANT_MAP[ p_Count ];

			// Find each location and set it up
			for( ZED_MEMSIZE i = 0; i < p_Count; i++ )
			{
				m_pConstantMap[ i ].Location = zglGetUniformLocation( m_ProgramID,
					p_pTypes[ i ].pName );
				m_pConstantMap[ i ].Index = p_pTypes[ i ].Index;
				m_pConstantMap[ i ].Type = p_pTypes[ i ].Type;
				// For debugging purposes, keep the name
#ifdef ZED_BUILD_DEBUG
				m_pConstantMap[ i ].pName = p_pTypes[ i ].pName;
#endif
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetConstantData( const ZED_UINT32 p_Index,
			const void *p_pValue )
		{
			switch( m_pConstantMap[ p_Index ].Type )
			{
				case ZED_FLOAT1:
				{
					zglUniform1f( m_pConstantMap[ p_Index ].Location,
						*( reinterpret_cast< GLfloat * >( &p_pValue ) )	);
					break;
				}
				case ZED_FLOAT3:
				{
					zglUniform3fv( m_pConstantMap[ p_Index ].Location, 1,
						static_cast< const GLfloat * >( p_pValue ) );
					break;
				}
				case ZED_INT1:
				{
					zglUniform1i( m_pConstantMap[ p_Index ].Location,
						*( reinterpret_cast< const GLint * >( &p_pValue ) ) );
					break;
				}
				case ZED_MAT4X4:
				{
					zglUniformMatrix4fv( m_pConstantMap[ p_Index ].Location, 1,
						ZED_FALSE,
						static_cast< const GLfloat * >( p_pValue ) );
					break;
				}
				default:
				{
					zedTrace( "[ZED::Renderer::GLShader::SetVariable] <WARN> "
						"Unknown uniform type.\n" );
					break;
				}
			}

			return ZED_OK;
		}
/*
		ZED_SINT32 GLShader::GetLocation( )
		{
			return ZED_OK;
		}
*/

		ZED_UINT32 GLShader::AttachShaders( )
		{
			// Get rid of the program as it's now being re-compiled
			if( m_ProgramID != 0 )
			{
				// Also, detach the shaders if they weren't already
				if( zglIsShader( m_VertexID ) )
				{
					zglDetachShader( m_ProgramID, m_VertexID );
				}
				if( zglIsShader( m_FragmentID ) )
				{
					zglDetachShader( m_ProgramID, m_FragmentID );
				}

				zglDeleteProgram( m_ProgramID );
			}

			// Now that (hopefully) the shaders are good to go, attach
			// them, linking is done upon calling Link (naturally)
			m_ProgramID = zglCreateProgram( );

			// Was the program created?
			if( m_ProgramID == 0 )
			{
				zedAssert( ZED_FALSE );
				zedTrace( "[ ZED | GLShader | Activate | ERROR] "
					"Program was not created" );

				return ZED_GRAPHICS_ERROR;
			}

			if( zglIsShader( m_VertexID ) )
			{
				zglAttachShader( m_ProgramID, m_VertexID );
			}

			if( zglIsShader( m_FragmentID ) )
			{
				zglAttachShader( m_ProgramID, m_FragmentID );
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShader::Link( )
		{
			zglLinkProgram( m_ProgramID );
			return ZED_OK;
		}
	}
}
