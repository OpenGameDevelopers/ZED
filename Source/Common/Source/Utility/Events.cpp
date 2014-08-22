#include <Utility/Events.hpp>

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
	}
}

