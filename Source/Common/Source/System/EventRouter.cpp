#include <System/Event.hpp>
#include <System/EventRouter.hpp>

namespace ZED
{
	namespace System
	{
		EventRouter::EventRouter( const ZED_CHAR8 *p_pName,
			const ZED_BOOL p_Global, const ZED_UINT32 p_BufferCount )
		{
			if( p_Global )
			{
				g_pEventRouter = this;
			}

			if( p_BufferCount > 2 )
			{
				m_BufferCount = p_BufferCount;
			}
			else
			{
				m_BufferCount = 2;
			}

			m_pEvents = new std::priority_queue< Event >[ m_BufferCount ];
		}

		EventRouter::~EventRouter( )
		{
			if( g_pEventRouter == this )
			{
				g_pEventRouter = ZED_NULL;
			}

			if( m_pEvents )
			{
				delete [ ] m_pEvents;
				m_pEvents = ZED_NULL;
			}
		}
	}
}

