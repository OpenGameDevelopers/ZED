#include <GLShader.hpp>
#include <GLWExtender.hpp>

namespace ZED
{
	namespace Renderer
	{
		GLShader::GLShader( )
		{
			m_pLocationsID = ZED_NULL;
			m_pInputMap = ZED_NULL;

			m_VertexID = m_FragmentID = m_GeometryID = m_ProgramID = 0;

#ifdef ZED_BUILD_DEBUG
			m_ppVertexSrc = ZED_NULL;
			m_ppFragmentSrc = ZED_NULL;
			m_ppGeometrySrc = ZED_NULL;
#endif
		}

		GLShader::GLShader( const ZED_BOOL p_Vertex, const ZED_BOOL p_Fragment,
			const ZED_BOOL p_Geometry )
		{
			m_pLocationsID = ZED_NULL;
			m_pInputMap = ZED_NULL;

			m_VertexID = m_FragmentID = m_GeometryID = m_ProgramID = 0;

#ifdef ZED_BUILD_DEBUG
			m_ppVertexSrc = ZED_NULL;
			m_ppFragmentSrc = ZED_NULL;
			m_ppGeometrySrc = ZED_NULL;
#endif
			for( ZED_MEMSIZE i = 0; i < 3; i++ )
			{
				m_pInitShader[ i ] = ZED_FALSE;
			}

			if( p_Vertex == ZED_TRUE )
			{
				m_VertexID = zglCreateShader( GL_VERTEX_SHADER );
				m_pInitShader[ 0 ] = ZED_TRUE;
			}
			if( p_Fragment == ZED_TRUE )
			{
				m_FragmentID = zglCreateShader( GL_FRAGMENT_SHADER );
				m_pInitShader[ 1 ] = ZED_TRUE;
			}
		}

		ZED_UINT32 GLShader::Compile( const ZED_CHAR8 **p_ppData,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_FromFile )
		{
			// Common locals
			ZED_INT32 CompileStatus = 0;

			// If p_FromFile is true, p_Data is the filepath
			if( p_FromFile == ZED_FALSE )
			{
				// Get the Data as a GLchar pointer pointer
				const GLchar **ppData =
					reinterpret_cast< const GLchar** >( p_ppData );

				if( p_Type == ZED_VERTEX_SHADER )
				{
					if( m_VertexID == 0 )
					{
						zedAssert( ZED_FALSE );
						zedTrace( "[GLShader | Compile | FAILED] Vertex Shader"
							" invalid\n" );
						return ZED_GRAPHICS_ERROR;
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
						ZED_INT32 LogLength;
						GLchar *pLog;
						zglGetShaderiv( m_VertexID, GL_INFO_LOG_LENGTH,
							&LogLength );
						pLog = new GLchar[ LogLength ];
						zglGetShaderInfoLog( m_VertexID, LogLength, &LogLength,
							pLog );
						zedTrace( "[GLShader | Compile | FAILED] Vertex Shader"
							" Log:\n\t%s\n", pLog );
						delete pLog;
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
					if( m_FragmentID == 0 )
					{
						zedAssert( ZED_FALSE );
						zedTrace( "[GLShader | Compile | FAILED] Fragment "
							"Shader invalid\n" );
						return ZED_GRAPHICS_ERROR;
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
						ZED_INT32 LogLength;
						GLchar *pLog;

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
			// NO CHECKING OF VALID PROGRAM!
			zglUseProgram( m_ProgramID );

			return ZED_OK;
		}

		void GLShader::Deactivate( )
		{
			// Don't do anything, as when zglUseProgram is called, the
			// previous program isn't in use while the current one is
		}

		void GLShader::AddType( const ZED_SHADER_TYPE p_Type )
		{
		}

		void GLShader::Delete( )
		{
			// Unload everything associated with this shader
			if( m_pLocationsID != ZED_NULL )
			{
				delete m_pLocationsID;
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

		ZED_UINT32 GLShader::SetVertexAttributeTypes( const ZED_SHADER_VA_MAP *p_pVAMap,
			const ZED_MEMSIZE p_Count )
		{
			m_pVAMap = new ZED_SHADER_VA_MAP[ p_Count ];
			for( ZED_MEMSIZE i = 0; i < p_Count; i++ )
			{
				m_pVAMap[ i ] = p_pVAMap[ i ];
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVariableTypes( const ZED_SHADER_INPUT_MAP *p_pTypes,
			const ZED_UINT32 p_Count )
		{
			// NO ERROR CHECKING!
			m_pInputMap = new ZED_SHADER_INPUT_MAP[ p_Count ];

			// Find each location and set it up
			for( ZED_MEMSIZE i = 0; i < p_Count; i++ )
			{
				m_pInputMap[ i ].Location = zglGetUniformLocation( m_ProgramID,
					p_pTypes[ i ].pName );
				m_pInputMap[ i ].Index = p_pTypes[ i ].Index;
				m_pInputMap[ i ].Type = p_pTypes[ i ].Type;
				// For debugging purposes, keep the name
#ifdef ZED_BUILD_DEBUG
				m_pInputMap[ i ].pName = p_pTypes[ i ].pName;
#endif
			}

			return ZED_OK;
		}

		ZED_UINT32 GLShader::SetVariable( const ZED_UINT32 p_Index,
			const void *p_pValue )
		{
			switch( m_pInputMap[ p_Index ].Type )
			{
			case ZED_FLOAT3:
				{
					zglUniform3fv( m_pInputMap[ p_Index ].Location, 1,
						( const GLfloat * )p_pValue );
					break;
				}
			case ZED_INT1:
				{
					zglUniform1i( m_pInputMap[ p_Index ].Location,
						( GLint )p_pValue );
					break;
				}
			case ZED_MAT4X4:
				{
					zglUniformMatrix4fv( m_pInputMap[ p_Index ].Location, 1,
						ZED_FALSE, ( const GLfloat * )p_pValue );
					break;
				}
			}
			return ZED_OK;
		}



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
				zedTrace( "[ ZED | GLSahder | Activate | ERROR] "
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
