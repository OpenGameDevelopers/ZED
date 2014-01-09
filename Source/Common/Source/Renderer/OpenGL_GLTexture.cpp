#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLTexture.hpp>
#include <System/Memory.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <cstdio>

namespace ZED
{
	namespace Renderer
	{
		GLTexture::GLTexture( ) :
			m_pData( ZED_NULL ),
			m_Width( 0 ),
			m_Height( 0 ),
			m_Format( ZED_FORMAT_UNDEFINED ),
			m_TextureID( 0 ),
			m_TextureUnit( 0 ),
			m_TextureType( ZED_TEXTURE_TYPE_INVALID )
		{
		}

		GLTexture::~GLTexture( )
		{
			//zedSafeDeleteArray( m_pData );
			zglDeleteTextures( 1, &m_TextureID );
		}

		ZED_UINT32 GLTexture::Load( const ZED_CHAR8 *p_pFilename )
		{
			if( m_TargaTexture.Load( p_pFilename ) != ZED_OK )
			{
				return ZED_FAIL;
			}

			m_Width = m_TargaTexture.GetWidth( );
			m_Height = m_TargaTexture.GetHeight( );
			m_pData = m_TargaTexture.GetImageData( );
			m_Format = m_TargaTexture.GetFormat( );

			zglGenTextures( 1, &m_TextureID );

			if( m_TextureID == 0 )
			{
				return ZED_FAIL;
			}

			zglBindTexture( m_TextureType, m_TextureID );

			GLenum InternalFormat = GL_INVALID_ENUM;
			GLenum Format = GL_INVALID_ENUM;

			switch( m_Format )
			{
				case ZED_FORMAT_ARGB8:
				{
					InternalFormat = GL_RGBA8;
					Format = GL_RGBA;
					break;
				}
				default:
				{
					return ZED_FAIL;
				}
			}
			
			zglTexStorage2D( m_TextureType, 1, InternalFormat, m_Width,
				m_Height );
			zglTexSubImage2D( m_TextureType, 0, 0, 0, m_Width, m_Height,
				Format, GL_UNSIGNED_BYTE, m_pData );				

			return ZED_OK;
		}

		ZED_UINT32 GLTexture::Activate( ) const
		{
			if( m_TextureID == 0 )
			{
				return ZED_FAIL;
			}

			if( m_TextureType == ZED_TEXTURE_TYPE_INVALID )
			{
				return ZED_FAIL;
			}

			zglBindTexture( m_TextureType, m_TextureID );

			return ZED_OK;
		}

		ZED_UINT32 GLTexture::SetTextureType(
			const ZED_TEXTURE_TYPE p_TextureType )
		{
			m_TextureType = p_TextureType;

			return ZED_OK;
		}

		ZED_UINT32 GLTexture::SetTextureUnit(
			const ZED_UINT32 p_TextureUnit )
		{
			if( m_TextureID == 0 )
			{
				return ZED_FAIL;
			}

			if( m_TextureType == ZED_TEXTURE_TYPE_INVALID )
			{
				return ZED_FAIL;
			}

			GLint TextureUnitsMax = 0;

			zglGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &TextureUnitsMax );

			if( p_TextureUnit > TextureUnitsMax )
			{
				return ZED_FAIL;
			}

			zglBindTexture( m_TextureType, m_TextureID );
			zglActiveTexture( GL_TEXTURE0 + p_TextureUnit );

			return ZED_OK;
		}

		ZED_BYTE *GLTexture::GetData( ) const
		{
			return m_pData;
		}

		ZED_FORMAT GLTexture::GetFormat( ) const
		{
			return m_Format;
		}

		ZED_UINT32 GLTexture::GetWidth( ) const
		{
			return m_Width;
		}

		ZED_UINT32 GLTexture::GetHeight( ) const
		{
			return m_Height;
		}
	}
}

#endif // ZED_PLATFORM_SUPPORTS_OPENGL

