#ifndef __ZEDSTRING_HPP__
#define __ZEDSTRING_HPP__

#include <DataTypes.hpp>

#ifdef ZED_USESTLSTRING
#include <iostream>
typedef	std::string ZED_STRING;

ZED_INT32 zedStringCmp( const ZED_CHAR8 *String1, const ZED_CHAR8 *String2,
	size_t MaxSize );
#else
class ZED_STRING
{
public:
	ZED_STRING( );
	~ZED_STRING( );

	ZED_STRING( const ZED_UINT16 *p_pInString );
private:
	// Use unicode for strings
	ZED_UINT16	*m_pString;
};
#endif

#endif
