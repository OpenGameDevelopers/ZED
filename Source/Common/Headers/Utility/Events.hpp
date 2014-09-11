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
		const ZED::Utility::EventType MousePositionInputEventType(
			"Mouse Position Input" );
		const ZED::Utility::EventType ActionInputEventType( "Action Input" );
		const ZED::Utility::EventType ResolutionChangeEventType(
			"Resolution Change" );

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

		// Mouse data
		class MousePositionInputEventData : public ZED::Utility::EventData
		{
		public:
			MousePositionInputEventData( );
			virtual ~MousePositionInputEventData( );

			ZED_UINT32 GetPosition( ZED_SINT32 &p_X, ZED_SINT32 &p_Y ) const;

			void SetPosition( const ZED_SINT32 p_X, const ZED_SINT32 p_Y );
		private:
			ZED_SINT32 m_X;
			ZED_SINT32 m_Y;
		};

		// Mouse event
		class MousePositionEvent : public ZED::Utility::Event
		{
		public:
			ZED_EXPLICIT MousePositionEvent(
				MousePositionInputEventData *p_pMousePositionData = ZED_NULL,
				ZED_UINT64 p_DispatchTime = ZED::System::GetTimeMiS( ) );
			virtual ~MousePositionEvent( );

			void SetPosition( const ZED_SINT32 p_X, const ZED_SINT32 p_Y );

		private:
			MousePositionInputEventData m_MousePositionData;
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

		// Resolution data
		class ResolutionChangeEventData : public ZED::Utility::EventData
		{
		public:
			ResolutionChangeEventData( );
			virtual ~ResolutionChangeEventData( );

			ZED_UINT32 SetResolution( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

			ZED_UINT32 GetResolution( ZED_UINT32 &p_Width,
				ZED_UINT32 &p_Height ) const;

		private:
			ZED_UINT32 m_Width;
			ZED_UINT32 m_Height;
		};

		// Resolution event
		class ResolutionChangeEvent : public ZED::Utility::Event
		{
		public:
			ResolutionChangeEvent(
				ResolutionChangeEventData *p_pResolutionData = ZED_NULL,
				ZED_UINT64 p_DispatchTime = ZED::System::GetTimeMiS( ) );
			virtual ~ResolutionChangeEvent( );

			ZED_UINT32 SetResolution( const ZED_UINT32 p_Width,
				const ZED_UINT32 p_Height );

		private:
			ResolutionChangeEventData	m_ResolutionData;
		};
	}
}

#endif // __ZED_UTILITY_EVENTS_HPP__

