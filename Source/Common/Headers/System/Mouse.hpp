#ifndef __ZED_SYSTEM_INPUT_MOUSE_HPP__
#define __ZED_SYSTEM_INPUT_MOUSE_HPP__

#include <System/InputDevice.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_BYTE MOUSE_BUTTON_1 = 1 << 0;
		const ZED_BYTE MOUSE_BUTTON_2 = 1 << 1;
		const ZED_BYTE MOUSE_BUTTON_3 = 1 << 2;
		const ZED_BYTE MOUSE_BUTTON_4 = 1 << 3;
		const ZED_BYTE MOUSE_BUTTON_5 = 1 << 4;

		class Mouse : public InputDevice
		{
		public:
			Mouse( );
			virtual ~Mouse( );

			virtual ZED_UINT32 State( void *p_pState ) const;
			virtual ZED_INLINE ZED_UINT32 Type( ) const
			{
				return ZED_INPUT_DEVICE_MOUSE;
			}

			ZED_INLINE ZED_BOOL IsButtonUp( const ZED_BYTE p_Button )
				{ return !( m_Buttons &p_Button ); }
			ZED_INLINE ZED_BOOL IsButtonDown( const ZED_BYTE p_Button )
				{ return ( m_Buttons & p_Button ); }

			ZED_INLINE void ButtonUp( const ZED_BYTE p_Button )
				{ m_Buttons &= ~p_Button; }
			ZED_INLINE void ButtonDown( const ZED_BYTE p_Button )
				{ m_Buttons &= p_Button; }

			ZED_INLINE void Position( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y )
				{ m_X = p_X; m_Y = p_Y; }

			ZED_INLINE void Position( ZED_UINT32 *p_pX, ZED_UINT32 *p_pY )
				{ ( *p_pX ) = m_X; ( *p_pY ) = m_Y; }
			
		private:
			ZED_UINT32	m_X, m_Y;
			ZED_BYTE	m_Buttons;
		};
	}
}

#endif // __ZED_SYSTEM_INPUT_MOUSE_HPP__

