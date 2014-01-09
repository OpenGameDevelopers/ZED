#ifndef __ZED_RENDERER_MATERIAL_HPP__
#define __ZED_RENDERER_MATERIAL_HPP__

namespace ZED
{
	namespace Renderer
	{
		class Texture;
		class Material
		{
		public:
			virtual ~Material( ) { }

			virtual ZED_UINT32 AttachTexture(
				const ZED::Renderer::Texture *p_pTexture,
				const ZED_UINT32 p_Unit ) = 0;

			virtual ZED_UINT32 GetID( ) const;

		protected:
		};
	}
}

#endif // __ZED_RENDERER_MATERIAL_HPP__

