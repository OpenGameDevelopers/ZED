#include <Game/GameState.hpp>
#include <Utility/EventRouter.hpp>

namespace ZED
{
	namespace Game
	{
		GameState::~GameState( )
		{
		}

		ZED::Utility::EventRouter * const GameState::GetEventRouter( )
		{
			return m_pEventRouter;
		}
	}
}

