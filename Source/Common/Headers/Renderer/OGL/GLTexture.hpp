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
		};
	}
}

#endif

