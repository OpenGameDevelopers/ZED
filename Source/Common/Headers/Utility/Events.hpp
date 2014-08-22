#ifndef __ZED_UTILITY_EVENTS_HPP__
#define __ZED_UTILITY_EVENTS_HPP__

#include <System/DataTypes.hpp>
#include <System/InputTypes.hpp>
#include <Utility/Event.hpp>

namespace ZED
{
	namespace Utility
	{
		const ZED::Utility::EventType KeyboardInputEventType(
			"Keyboard Input" );
		const ZED::Utility::EventType ActionInputEventType( "Action Input" );

		// Keyboard data
		class KeyboardInputEventData : public ZED::Utility::EventData
		{
		public:
			KeyboardInputEventData( );
			virtual ~KeyboardInputEventData( );

			ZED_UINT32 GetState( ZED_KEY &p_Key, ZED_BOOL &p_State ) const;

			void SetState( const ZED_KEY p_Key, const ZED_BOOL p_State );

		private:
			ZED_KEY		m_Key;
			ZED_BOOL	m_State;
		};

		// Keyboard event
		class KeyboardEvent : public ZED::Utility::Event
		{
		public:
			KeyboardEvent( KeyboardInputEventData *p_pKeyboardData = ZED_NULL,
				ZED_UINT64 p_DispatchTime = ZED::System::GetTimeMiS( ) );
			virtual ~KeyboardEvent( );

			void SetState( const ZED_KEY p_Key, const ZED_BOOL p_State );

		private:
			KeyboardInputEventData	m_KeyboardData;
		};

		// Action data
		class ActionInputEventData: public ZED::Utility::EventData
		{
		public:
			ActionInputEventData( );
			virtual ~ActionInputEventData( );

			ZED_UINT32 SetAction( const ZED_UINT32 p_Action,
				const ZED_FLOAT32 p_Value );

			void GetAction( ZED_UINT32 &p_Action, ZED_FLOAT32 &p_Value );

		private:
			ZED_UINT32	m_ActionID;
			ZED_FLOAT32	m_Value;
		};

		// Action event
		class ActionInputEvent : public ZED::Utility::Event
		{
		public:
			ActionInputEvent( ActionInputEventData *p_pActionData = ZED_NULL,
				ZED_UINT64 p_DispatchTime = ZED::System::GetTimeMiS( ) );
			virtual ~ActionInputEvent( );

			void SetAction( const ZED_UINT32 p_Action,
				const ZED_FLOAT32 p_Value );

		private:
			ActionInputEventData	m_ActionData;
		};
	}
}

#endif // __ZED_UTILITY_EVENTS_HPP__

