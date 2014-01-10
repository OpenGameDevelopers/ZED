#ifndef __ZED_RENDERER_MATERIAL_HPP__
#define __ZED_RENDERER_MATERIAL_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Texture;
		class Material
		{
		public:
			Material( );
			virtual ~Material( );

			virtual ZED_UINT32 AttachTexture(
				const ZED::Renderer::Texture *p_pTexture,
				const ZED_UINT32 p_Unit ) = 0;

			ZED_UINT32 GetID( ) const;
			ZED_CHAR8 *GetName( ) const;

			void SetID( const ZED_UINT32 p_ID );
			void SetName( const ZED_CHAR8 *p_pName );

		protected:
			ZED_UINT32	m_ID;
			ZED_CHAR8	*m_pName;
		};
	}
}

#endif // __ZED_RENDERER_MATERIAL_HPP__

