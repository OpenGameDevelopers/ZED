#ifndef __ZED_GAME_GAMESTATEINPUTLISTENER_HPP__
#define __ZED_GAME_GAMESTATEINPUTLISTENER_HPP__

#include <System/DataTypes.hpp>
#include <Utility/Event.hpp>

namespace ZED
{
	namespace Utility
	{
		class InputBinder;
	}
	namespace Game
	{
		class GameStateInputListener : public ZED::Utility::EventListener
		{
		public:
			GameStateInputListener( );
			virtual ~GameStateInputListener( );

			virtual ZED_BOOL HandleEvent( const ZED::Utility::Event &p_Event );
			virtual ZED_CHAR8 *GetName( ) const;

			ZED_UINT32 SetInputBinder(
				ZED::Utility::InputBinder * const &p_pInputBinder );

		private:
			ZED::Utility::InputBinder	*m_pInputBinder;
		};
	}
}

#endif // __ZED_GAME_GAMESTATEINPUTLISTENER_HPP__

