#include <Renderer/Targa.hpp>

namespace ZED
{
	namespace Renderer
	{
		Targa::Targa( ) :
			m_pData( ZED_NULL ),
			m_Width( 0U ),
			m_Height( 0U )
		{
		}

		Targa::~Targa( )
		{
		}

		ZED_UINT32 Targa::Load( const ZED_CHAR8 *p_pFileName )
		{
			return ZED_FAIL;
		}
	}
}

