#ifndef __ZED_SYSTEM_WINDOWDATA_HPP__
#define __ZED_SYSTEM_WINDOWDATA_HPP__

#include <System/DataTypes.hpp>

namespace ZED
{
	namespace System
	{
		class WindowData
		{
		public:
			virtual ~WindowData( );

			ZED_SINT32 GetDisplayNumber( ) const;
			ZED_SINT32 GetScreenNumber( ) const;

		protected:
			ZED_SINT32 m_DisplayNumber;
			ZED_SINT32 m_ScreenNumber;
		};
	}
}

#endif // __ZED_SYSTEM_WINDOWDATA_HPP__

