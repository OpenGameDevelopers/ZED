#ifndef __ZED_RENDERER__LINUXRENDEREROGL3_HPP__
#define __ZED_RENDERER__LINUXRENDEREROGL3_HPP__

#include <DataTypes.hpp>
#include <GLExtender.hpp>
#include <CanvasDescription.hpp>
#include <Renderer.hpp>
#include <Vector3.hpp>

namespace ZED
{
	namespace Renderer
	{
		class LinuxRendererOGL3 : public Renderer
		{
		public:
			LinuxRendererOGL3( );
			virtual ~LinuxRendererOGL3( );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas );

			virtual void ForceClear( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );

			virtual void ClearColour( const ZED_FLOAT32 p_Red,
				const ZED_FLOAT32 p_Green, const ZED_FLOAT32 p_Blue );

			virtual ZED_UINT32 BeginScene( const ZED_BOOL p_Colour,
				const ZED_BOOL p_Depth, const ZED_BOOL p_Stencil );
			virtual void EndScene( );

			virtual ZED_BOOL ToggleFullscreen( );

			virtual ZED_UINT32 ResizeCanvas( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			virtual void Release( );

			virtual void SetView3D(
				const Arithmetic::Vector3 &p_Right,
				const Arithmetic::Vector3 &p_Up,
				const Arithmetic::Vector3 &p_Direction,
				const Arithmetic::Vector3 &p_Position );

			virtual void SetViewLookAt(
				const Arithmetic::Vector3 &p_Position,
				const Arithmetic::Vector3 &p_Point,
				const Arithmetic::Vector3 &p_WorldUp );

			virtual void CalcViewProjMatrix( );
			virtual void CalcWorldViewProjMatrix( );

			virtual void SetClippingPlanes( const ZED_FLOAT32 p_Near,
				const ZED_FLOAT32 p_Far );

			virtual void Prepare2D( );

			virtual ZED_UINT32 CalcPerspProjMatrix( const ZED_FLOAT32 p_FOV,
				const ZED_FLOAT32 p_AspectRatio,
				Arithmetic::Matrix4x4 *p_pMatrix );

			virtual ZED_UINT32 SetMode( const ZED_UINT32 p_Stage,
				const ZED_VIEWMODE p_Mode );

			virtual ZED_UINT32 SetDisplay( Display *p_pDisplay );

			virtual ZED_UINT32 Create( GraphicsAdapter *p_pAdapter,
				const CanvasDescription &p_Canvas,
				Display *p_pDisplay );

			// TEMP!
			// For after Create and after window creation
			void CreateGLContext( );
			// !TEMP

//		private:
			GLExtender	m_GLExt;
			Display		*m_pDisplay;
			Screen		*m_pScreen;
			Window		m_Window;
			GLXContext	m_GLContext;
			Colormap	m_ColMap;

			CanvasDescription m_Canvas;
		};
	}
}

#endif

