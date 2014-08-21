#include <Game/GameStateManager.hpp>
#include <Game/GameState.hpp>
#include <Renderer/Renderer.hpp>
#include <System/Debugger.hpp>
#include <System/Time.hpp>
#include <cstring>

namespace ZED
{
	namespace Game
	{
		GameStateManager::GameStateManager( ) :
			m_pRenderer( ZED_NULL ),
			m_TotalElapsedTime( 0ULL ),
			m_Running( ZED_FALSE )
		{
			ZED::System::StartTime( );
			m_StartTime = ZED::System::GetTimeMiS( );
		}

		GameStateManager::~GameStateManager( )
		{
		}

		ZED_UINT32 GameStateManager::Initialise( )
		{
			m_Running = ZED_TRUE;
			
			return ZED_OK;
		}

		ZED_UINT32 GameStateManager::Execute( )
		{
			if( m_GameStateStack.empty( ) )
			{
				this->Quit( );

				zedTrace( "[ZED::Game::GameStateManager::Execute] <ERROR> "
					"No more game states left, exiting\n" );

				return ZED_FAIL;
			}

			static ZED_UINT64 BeginTime = ZED::System::GetTimeMiS( );
			ZED_UINT64 ElapsedTime = ZED::System::GetTimeMiS( );
			ZED_UINT64 TimeDifference = ElapsedTime - m_StartTime;
			m_TotalElapsedTime = ElapsedTime - BeginTime;
			m_StartTime = ElapsedTime;

			m_GameStateStack.top( )->Update( TimeDifference );

			m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );

			m_GameStateStack.top( )->Render( );

			m_pRenderer->EndScene( );

			return ZED_OK;
		}

		ZED_UINT32 GameStateManager::RegisterState(
			GameState * const &p_pGameState )
		{
			m_GameStateRegistry.insert( p_pGameState );

			return ZED_OK;
		}

		ZED_UINT32 GameStateManager::GetNumberOfStates( ) const
		{
			return m_GameStateRegistry.size( );
		}

		ZED_CHAR8 *GameStateManager::GetStateName(
			const ZED_UINT32 p_Index ) const
		{
			if( p_Index > m_GameStateRegistry.size( ) )
			{
				zedTrace( "[ZED::Game::GameStateManager::GetStateName] <WARN> "
					"Index out of range.  Maximum range: 0-%d\n",
					m_GameStateRegistry.size( ) );

				return "\0";
			}

			GameStateRegistry::const_iterator RegistryItr =
				m_GameStateRegistry.begin( );

			for( ZED_UINT32 i = 0; i < p_Index; ++i )
			{
				++RegistryItr;
			}

			return ( *RegistryItr )->GetName( );
		}

		ZED_UINT32 GameStateManager::ChangeState(
			const ZED_CHAR8 *p_pGameStateName, ZED_UINT32 p_Level )
		{
			if( this->IsGameStateNameValid( p_pGameStateName )== ZED_FALSE )
			{
				zedTrace( "[ZED::Game::GameStateManager::ChangeState] <ERROR> "
					"Invalid game state name: \"%s\" not found in registry\n",
					p_pGameStateName );

				return ZED_FAIL;
			}

			// By default, the level should be zero, this indicates all states
			// should be popped off and replaced with this one
			if( p_Level == 0 )
			{
				while( !m_GameStateStack.empty( ) )
				{
					this->PopState( );
				}
			}
			else
			{
				ZED_UINT32 Level = p_Level;
				if( p_Level > m_GameStateStack.size( ) )
				{
					zedTrace( "[ZED::Game::GameStateManager::ChangeState] "
						"<WARN> Specified level to roll back too large, "
						"going back %d levels\n", m_GameStateStack.size( ) );
					Level = m_GameStateStack.size( );
				}

				for( ZED_UINT32 i = 0; i < Level; ++i )
				{
					this->PopState( );
				}
			}

			this->PushState( p_pGameStateName );

			return ZED_OK;
		}

		ZED_UINT32 GameStateManager::PushState(
			const ZED_CHAR8 *p_pGameStateName )
		{
			if( this->IsGameStateNameValid( p_pGameStateName ) == ZED_FALSE )
			{
				zedTrace( "[ZED::Game::GameStateManager::PushState] <ERROR> "
					"Failed to push non-existant state \"%s\" onto the "
					"stack\n", p_pGameStateName );

					
				return ZED_FAIL;
			}

			GameStateRegistry::const_iterator RegistryItr =
				m_GameStateRegistry.begin( );

			for( ; RegistryItr != m_GameStateRegistry.end( ); ++RegistryItr )
			{
				if( strncmp( ( *RegistryItr )->GetName( ), p_pGameStateName,
					strlen( p_pGameStateName ) ) == 0 )
				{
					break;
				}
			}

			m_GameStateStack.push( ( *RegistryItr ) );

			if( m_GameStateStack.top( )->Enter( ) != ZED_OK )
			{
				zedTrace( "[ZED::Game::GameStatemanager::PushState] <ERROR> "
					"Failed to enter the state \"%s\"\n", p_pGameStateName );

				this->PopState( );
				return ZED_FAIL;
			}

			return ZED_OK;
		}

		void GameStateManager::PopState( )
		{
			if( !m_GameStateStack.empty( ) )
			{
				m_GameStateStack.pop( );
			}
		}

		void GameStateManager::Quit( )
		{
			while( !m_GameStateStack.empty( ) )
			{
				m_GameStateStack.pop( );
			}

			m_Running = ZED_FALSE;
		}

		ZED_BOOL GameStateManager::IsRunning( ) const
		{
			return m_Running;
		}

		ZED::Renderer::Renderer * const GameStateManager::GetRenderer( )
		{
			return m_pRenderer;
		}

		ZED_UINT32 GameStateManager::SetRenderer(
			ZED::Renderer::Renderer * const &p_pRenderer )
		{
			if( p_pRenderer )
			{
				m_pRenderer = p_pRenderer;
				return ZED_OK;
			}

			zedTrace( "[ZED::Game::GameStateManager::SetRenderer] <ERROR> "
				"Unable to set renderer, possibly unset\n" );

			return ZED_FAIL;
		}

		ZED_FLOAT64 GameStateManager::GetTotalElapsedTimeAsFloat64( ) const
		{
			return static_cast< ZED_FLOAT64 >( m_TotalElapsedTime ) *
				OneOverMillion;
		}

		ZED_UINT64 GameStateManager::GetTotalElapsedTimeAsUInt64( ) const
		{
			return m_TotalElapsedTime;
		}

		GameStateManager &GameStateManager::GetInstance( )
		{
			static GameStateManager Instance;

			return Instance;
		}

		ZED_BOOL GameStateManager::IsGameStateNameValid(
			const ZED_CHAR8 *p_pGameStateName ) const
		{
			GameStateRegistry::const_iterator RegistryItr =
				m_GameStateRegistry.begin( );

			for( ; RegistryItr != m_GameStateRegistry.end( ); ++RegistryItr )
			{
				if( strncmp( ( *RegistryItr )->GetName( ), p_pGameStateName,
					strlen( p_pGameStateName ) ) == 0 )
				{
					return ZED_TRUE;
				}
			}

			zedTrace( "[ZED::Game::GameStateManager::IsGameStateNameValid] "
				"<ERROR> Could not find \"%s\" in registry\n",
				p_pGameStateName );

			return ZED_FALSE;
		}

	}
}

