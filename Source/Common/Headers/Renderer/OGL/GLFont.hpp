#ifndef __ZED_RENDERER_GLFONT_HPP__
#define __ZED_RENDERER_GLFONT_HPP__

#include <Renderer/Font.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLFont : public Font
		{
		public:
			GLFont( );
			virtual ~GLFont( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilePath );

		private:
		};
	}
}

#endif // __ZED_RENDERER_GLFONT_HPP__

