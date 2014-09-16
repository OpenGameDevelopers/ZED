#include <Game/GameState.hpp>
#include <Utility/EventRouter.hpp>

namespace ZED
{
	namespace Game
	{
		GameState::GameState( ) :
			m_pInputBinder( ZED_NULL ),
			m_pEventRouter( ZED_NULL )
		{
		}

		GameState::~GameState( )
		{
		}

		ZED::Utility::EventRouter * const GameState::GetEventRouter( )
		{
			return m_pEventRouter;
		}
	}
}

