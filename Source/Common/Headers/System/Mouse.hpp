#ifndef __ZED_SYSTEM_INPUT_MOUSE_HPP__
#define __ZED_SYSTEM_INPUT_MOUSE_HPP__

#include <System/InputDevice.hpp>
#include <System/Debugger.hpp>
namespace ZED
{
	namespace System
	{
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

			ZED_INLINE ZED_BOOL IsButtonUp( const ZED_UINT32 p_Button )
				{ return !( m_Buttons & p_Button ); }
			ZED_INLINE ZED_BOOL IsButtonDown( const ZED_UINT32 p_Button )
				{ return ( m_Buttons & p_Button ); }

			ZED_INLINE void ButtonUp( const ZED_UINT32 p_Button )
				{ m_Buttons ^= ( 0xFFFFFFFF & ( 1 << ( p_Button-1 ) ) );
#if defined ZED_DEBUG_INPUT
					zedTrace( "Released: %d\n",
						( 0xFFFFFFFF & ( 1 << ( p_Button-1 ) ) ) );
					zedTrace( "Released: m_Buttons: 0x" );
					for( int i = 0; i < 32; ++i )
					{
						zedTrace( "%d", ( m_Buttons >> i ) & 0x00000001 );
					}
					zedTrace( "\n" );
#endif
				}
			ZED_INLINE void ButtonDown( const ZED_UINT32 p_Button )
				{ m_Buttons |= ( 1 << ( p_Button-1 ) );
#if defined ZED_DEBUG_INPUT
					zedTrace( "Pressed: %d\n", ( 1 << ( p_Button-1 ) ) );
					zedTrace( "Pressed: m_Buttons: 0x" );
					for( int i = 31; i > -1; --i )
					{
						zedTrace( "%d", ( m_Buttons >> i ) & 0x00000001 );
					}
					zedTrace( "\n" );
#endif
				}

			ZED_INLINE void Position( const ZED_UINT32 p_X,
				const ZED_UINT32 p_Y )
				{ m_X = p_X; m_Y = p_Y; }

			ZED_INLINE void Position( ZED_UINT32 *p_pX, ZED_UINT32 *p_pY )
				{ ( *p_pX ) = m_X; ( *p_pY ) = m_Y; }
			
		private:
			ZED_UINT32	m_X, m_Y;
			ZED_UINT32	m_Buttons;
		};
	}
}

#endif // __ZED_SYSTEM_MOUSE_HPP__

