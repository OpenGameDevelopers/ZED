#ifndef __ZEDMODEL_HPP__
#define __ZEDMODEL_HPP__

#include <ModelStructs.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>

namespace ZED
{
	namespace Renderer
	{
		class Model
		{
		public:
		private:
			// Model data
			ZED::Arithmetic::Vector3 *m_pVertices;
			ZED::Arithmetic::Vector3 *m_pNormals;
			// Only one set of UVs is required
			ZED::Arithmetic::Vector2 m_UVs;
		};
	}
}

#endif
