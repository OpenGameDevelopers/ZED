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

			virtual ZED_CHAR8 *Name( ) const = 0;
		};
	}
}

#endif

