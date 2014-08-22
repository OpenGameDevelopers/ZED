#include <Game/GameStateInputListener.hpp>
#include <Utility/InputBinder.hpp>
#include <Utility/EventRouter.hpp>
#include <Utility/Events.hpp>

namespace ZED
{
	namespace Game
	{
		GameStateInputListener::GameStateInputListener( ) :
			m_pInputBinder( ZED_NULL )
		{
		}

		GameStateInputListener::~GameStateInputListener( )
		{
		}

		ZED_BOOL GameStateInputListener::HandleEvent(
			const ZED::Utility::Event &p_Event )
		{
			if( p_Event.Type( ).ID( ) ==
				ZED::Utility::KeyboardInputEventType.ID( ) )
			{
				if( m_pInputBinder )
				{
					ZED::Utility::KeyboardInputEventData *pKeyboardData =
						p_Event.Data<
							ZED::Utility::KeyboardInputEventData >( );

					ZED_KEY Key;
					ZED_BOOL KeyState = ZED_FALSE;

					pKeyboardData->GetState( Key, KeyState );

					ZED_UINT32 ActionCount =
						m_pInputBinder->GetActionCountForKey( Key );

					if( ActionCount == 0 )
					{
						return ZED_FALSE;
					}
					else if( ActionCount == 1 )
					{
						ZED_UINT32 ActionID;
						ActionID = m_pInputBinder->GetActionFromKey( Key );

						if( ActionID != 0 )
						{
							ZED_FLOAT32 ActionValue = KeyState ? 1.0f : 0.0f;
							ZED::Utility::ActionInputEventData ActionData;
							ActionData.SetAction( ActionID, ActionValue );
							ZED::Utility::ActionInputEvent Action(
								&ActionData );

							ZED::Utility::SendEvent( Action );
							return ZED_TRUE;
						}
					}
					else
					{
						ZED_UINT32 ActionID[ ActionCount ];
						m_pInputBinder->GetActionsFromKey( Key, ActionID );

						for( ZED_UINT32 i = 0; i < ActionCount; ++i )
						{
							if( ActionID[ i ] != 0 )
							{
								ZED_FLOAT32 ActionValue =
									KeyState ? 1.0f : 0.0f;
								ZED::Utility::ActionInputEventData ActionData;
								ActionData.SetAction( ActionID[ i ],
									ActionValue );
								ZED::Utility::ActionInputEvent Action(
									&ActionData );

								ZED::Utility::SendEvent( Action );

								return ZED_TRUE;
							}
						}
					}
				}
			}

			return ZED_FALSE;
		}

		ZED_CHAR8 *GameStateInputListener::GetName( ) const
		{
			return "Game state input listener";
		}

		ZED_UINT32 GameStateInputListener::SetInputBinder(
			ZED::Utility::InputBinder * const &p_pInputBinder )
		{
			if( p_pInputBinder )
			{
				m_pInputBinder = p_pInputBinder;
				return ZED_OK;
			}

			return ZED_FAIL;
		}
	}
}

