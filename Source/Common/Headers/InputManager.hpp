#ifndef __ZED_SYSTEM_INPUTMANAGER_HPP__
#define __ZED_SYSTEM_INPUTMANAGER_HPP__

#include <DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		class InputManager
		{
		public:
			virtual ZED_UINT32 Initialise( )=0;
			virtual ZED_UINT32 AddDevice( )=0;
			virtual void Update( )=0;
		};
	}
}

#endif
