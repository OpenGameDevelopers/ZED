#include <Renderer/Material.hpp>
#include <System/Memory.hpp>
#include <cstring>

namespace ZED
{
	namespace Renderer
	{
		Material::Material( ) :
			m_ID( 0 ),
			m_pName( ZED_NULL )
		{
		}

		Material::~Material( )
		{
			zedSafeDeleteArray( m_pName );
		}

		ZED_UINT32 Material::GetID( ) const
		{
			return m_ID;
		}

		ZED_CHAR8 *Material::GetName( ) const
		{
			return m_pName;
		}

		void Material::SetID( const ZED_UINT32 p_ID )
		{
			m_ID = p_ID;
		}

		void Material::SetName( const ZED_CHAR8 *p_pName )
		{
			ZED_MEMSIZE NameLength = strlen( p_pName );
			m_pName = new ZED_CHAR8[ NameLength + 1 ];
			strncpy( m_pName, p_pName, NameLength );
			m_pName[ NameLength ] = '\0';
		}
	}
}

