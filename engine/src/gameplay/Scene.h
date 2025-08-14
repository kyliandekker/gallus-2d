#pragma once

#include "core/System.h"

#include "core/Data.h"

namespace gallus
{
	namespace gameplay
	{
		//---------------------------------------------------------------------
		// Scene
		//---------------------------------------------------------------------
		class Scene
		{
		public:
			~Scene();

			bool LoadData();

			void SetData(const core::Data& a_Data);

			const core::Data& GetData() const
			{
				return m_Data;
			}

		private:
			core::Data m_Data;
		};
	}
}