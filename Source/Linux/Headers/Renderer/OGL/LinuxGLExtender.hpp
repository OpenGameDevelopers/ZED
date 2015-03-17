#ifndef __ZED_RENDERER_LINUXGLEXTENDER_HPP__
#define __ZED_RENDERER_LINUXGLEXTENDER_HPP__

#include <Renderer/OGL/GLExtender.hpp>
#include <System/LinuxWindowData.hpp>

namespace ZED
{
	namespace Renderer
	{
		class LinuxGLExtender : public GLExtender 
		{
		public:
			ZED_EXPLICIT LinuxGLExtender(
				const ZED::System::LinuxWindowData &p_WindowData );
			virtual ~LinuxGLExtender( );

		protected:
			virtual ZED_UINT32 InitialiseWindowExtensions( );

			ZED::System::LinuxWindowData	m_WindowData;
		private:
			LinuxGLExtender( );
		};
	}
}

#endif // __ZED_RENDERER_LINUXGLEXTENDER_HPP__

