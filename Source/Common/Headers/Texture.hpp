#ifndef __ZED_RENDERER_TEXTURE_HPP__
#define __ZED_RENDERER_TEXTURE_HPP__

#include <DataTypes.hpp>
#include <RenderTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Texture
		{
		public:
			Texture( );
			~Texture( );

			void SetData( const void *p_pData );
			void SetFormat( const ZED_FORMAT *p_Format );
			void SetWidth( const ZED_UINT32 p_Width );
			void SetHeight( const ZED_UINT32 p_Heght );

			void		*GetData( ) const;
			ZED_FORMAT	GetFormat( ) const;
			ZED_UINT32	GetWidth( ) const;
			ZED_UINT32	GetHeight( ) const;

		private:
			void		*m_pData;
			ZED_UCHAR8	*m_pName;
			ZED_UINT32	m_Width;
			ZED_UINT32	m_Height;
			ZED_UINT32	m_FileType;
			ZED_FORMAT	m_Format;
		};
	}
}

#endif
