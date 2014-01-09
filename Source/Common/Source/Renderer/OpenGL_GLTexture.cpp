#if defined ZED_PLATFORM_SUPPORTS_OPENGL

#include <Renderer/OGL/GLTexture.hpp>
#include <System/Memory.hpp>
#include <Renderer/OGL/GLExtender.hpp>
#include <cstdio>

namespace ZED
{
	namespace Renderer
	{
		ZED_MEMSIZE FormatToBytes( const ZED_FORMAT p_Format )
		{
			ZED_MEMSIZE Size = 0;
			
			switch( p_Format )
			{
				case ZED_FORMAT_XRGB8:
				case ZED_FORMAT_ARGB8:
				{
					Size = 4;
					break;
				}
				case ZED_FORMAT_RGB565:
				{
					Size = 2;
					break;
				}
			}

			return Size;
		}

		GLTexture::GLTexture( )
		{
			m_pData = ZED_NULL;

			m_Width = 0;
			m_Height = 0;

			m_Format = ZED_FORMAT_UNDEFINED;
		}

		GLTexture::~GLTexture( )
		{
			//zedSafeDeleteArray( m_pData );
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

			return ZED_OK;
		}

		ZED_UINT32 GLTexture::Activate( ) const
		{
			if( m_TextureID == 0 )
			{
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 GLTexture::SetTextureUnit(
			const ZED_UINT32 p_TextureUnit )
		{
			if( m_TextureID == 0 )
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

