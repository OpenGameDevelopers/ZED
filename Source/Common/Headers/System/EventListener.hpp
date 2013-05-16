#ifndef __ZED_SYSTEM_EVENTLISTENER_HPP__
#define __ZED_SYSTEM_EVENTLISTENER_HPP__

namespace ZED
{
	namespace System
	{
		class EventListener
		{
		public:
			virtual ~EventListener( ) { }

			virtual ZED_BOOL HandleEvent( ) = 0;

#ifdef ZED_BUILD_DEBUG
			virtual ZED_CHAR8 *Name( ) = 0;
#endif
		};
	}
}

#endif

