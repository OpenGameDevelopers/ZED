#include <Utility/Events.hpp>
#include <System/Debugger.hpp>

namespace ZED
{
	namespace Utility
	{
		// Keyboard
		KeyboardInputEventData::KeyboardInputEventData( )
		{
		}

		KeyboardInputEventData::~KeyboardInputEventData( )
		{
		}

		ZED_UINT32 KeyboardInputEventData::GetState( ZED_KEY &p_Key,
			ZED_BOOL &p_State ) const
		{
			p_Key = m_Key;
			p_State = m_State;

			return ZED_OK;
		}

		void KeyboardInputEventData::SetState( const ZED_KEY p_Key,
			const ZED_BOOL p_State )
		{
			m_Key = p_Key;
			m_State = p_State;
		}

		KeyboardEvent::KeyboardEvent( KeyboardInputEventData *p_pKeyboardData,
			ZED_UINT64 p_DispatchTime ) :
			ZED::Utility::Event( KeyboardInputEventType.Name( ),
				p_pKeyboardData, p_DispatchTime )
		{
			m_pData = p_pKeyboardData;
		}

		KeyboardEvent::~KeyboardEvent( )
		{
		}

		void KeyboardEvent::SetState( const ZED_KEY p_Key,
			const ZED_BOOL p_State )
		{
			m_KeyboardData.SetState( p_Key, p_State );
		}

		// Mouse
		MousePositionInputEventData::MousePositionInputEventData( )
		{
		}

		MousePositionInputEventData::~MousePositionInputEventData( )
		{
		}

		ZED_UINT32 MousePositionInputEventData::GetPosition( ZED_SINT32 &p_X,
			ZED_SINT32 &p_Y ) const
		{
			p_X = m_X;
			p_Y = m_Y;

			return ZED_OK;
		}

		void MousePositionInputEventData::SetPosition( const ZED_SINT32 p_X,
			const ZED_SINT32 p_Y )
		{
			m_X = p_X;
			m_Y = p_Y;
		}

		MousePositionEvent::MousePositionEvent(
			MousePositionInputEventData *p_pMousePositionData,
			ZED_UINT64 p_DispatchTime ) :
				ZED::Utility::Event( MousePositionInputEventType.Name( ),
					p_pMousePositionData, p_DispatchTime )
		{
			m_pData = p_pMousePositionData;
		}

		MousePositionEvent::~MousePositionEvent( )
		{
		}

		void MousePositionEvent::SetPosition( const ZED_SINT32 p_X,
			const ZED_SINT32 p_Y )
		{
			m_MousePositionData.SetPosition( p_X, p_Y );
		}

		// Action
		ActionInputEventData::ActionInputEventData( )
		{
		}

		ActionInputEventData::~ActionInputEventData( )
		{
		}

		ZED_UINT32 ActionInputEventData::SetAction( const ZED_UINT32 p_ActionID,
			const ZED_FLOAT32 p_Value )
		{
			m_ActionID = p_ActionID;

			ZED_FLOAT32 Value = p_Value;

			if( p_Value < -1.0f )
			{
				Value = -1.0f;
			}
			if( p_Value > 1.0f )
			{
				Value = 1.0f;
			}

			m_Value = Value;

			return ZED_OK;
		}

		void ActionInputEventData::GetAction( ZED_UINT32 &p_ActionID,
			ZED_FLOAT32 &p_Value )
		{
			p_ActionID = m_ActionID;
			p_Value = m_Value;
		}

		ActionInputEvent::ActionInputEvent(
			ActionInputEventData *p_pActionData, ZED_UINT64 p_DispatchTime ) :
			ZED::Utility::Event( ActionInputEventType.Name( ), p_pActionData,
				p_DispatchTime )
		{
			m_pData = p_pActionData;
		}

		ActionInputEvent::~ActionInputEvent( )
		{
		}

		void ActionInputEvent::SetAction( const ZED_UINT32 p_Action,
			const ZED_FLOAT32 p_Value )
		{
			m_ActionData.SetAction( p_Action, p_Value );
		}

		// Resolution changed
		ResolutionChangeEventData::ResolutionChangeEventData( ) :
			m_Width( 0 ),
			m_Height( 0 )
		{
		}

		ResolutionChangeEventData::~ResolutionChangeEventData( )
		{
		}

		ZED_UINT32 ResolutionChangeEventData::SetResolution(
			const ZED_UINT32 p_Width, const ZED_UINT32 p_Height )
		{
			if( p_Width == 0 )
			{
				zedTrace( "[ZED::Utility::ResolutionChangeEventData::"
					"SetResolution] <ERROR> The width parameter is zero\n" );
				return ZED_FAIL;
			}

			if( p_Height == 0 )
			{
				zedTrace( "[ZED::Utility::ResolutionChangeEventData::"
					"SetResolution] <ERROR> The height parameter is zer\n" );
				return ZED_FAIL;
			}

			m_Width = p_Width;
			m_Height = p_Height;

			return ZED_OK;
		}

		ZED_UINT32 ResolutionChangeEventData::GetResolution(
			ZED_UINT32 &p_Width, ZED_UINT32 &p_Height ) const
		{
			p_Width = m_Width;
			p_Height = m_Height;

			return ZED_OK;
		}

		ResolutionChangeEvent::ResolutionChangeEvent(
			ResolutionChangeEventData *p_pResolutionData,
			ZED_UINT64 p_DispatchTime ) :
			ZED::Utility::Event( ResolutionChangeEventType.Name( ),
				p_pResolutionData, p_DispatchTime )
		{
			m_pData = p_pResolutionData;
		}

		ResolutionChangeEvent::~ResolutionChangeEvent( )
		{
		}

		ZED_UINT32 ResolutionChangeEvent::SetResolution( const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height )
		{
			return m_ResolutionData.SetResolution( p_Width, p_Height );
		}
	}
}

