#ifndef __ZED_RENDERER_GLTEXTURE_HPP__
#define __ZED_RENDERER_GLTEXTURE_HPP__

#include <Renderer/Texture.hpp>
#include <Renderer/Targa.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLTexture : public Texture
		{
		public:
			GLTexture( );
			~GLTexture( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilename );
			virtual ZED_UINT32 Load( ZED::System::File *p_pFile );

			virtual ZED_UINT32 Activate( ) const;

			virtual ZED_UINT32 SetTextureType(
				const ZED_TEXTURE_TYPE p_TextureType );
			virtual ZED_UINT32 SetTextureUnit(
				const ZED_UINT32 p_TextureUnit );

			virtual ZED_BYTE	*GetData( ) const;
			virtual ZED_FORMAT	GetFormat( ) const;
			virtual ZED_UINT32 	GetWidth( ) const;
			virtual ZED_UINT32	GetHeight( ) const;

		private:
			ZED_BYTE	*m_pData;

			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;

			ZED_FORMAT	m_Format;

			Targa		m_TargaTexture;

			GLuint		m_TextureID;
			GLenum		m_TextureUnit;
			GLenum		m_TextureType;
		};
	}
}

#endif

