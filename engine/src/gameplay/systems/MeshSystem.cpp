#include "gameplay/systems/MeshSystem.h"

#include "graphics/imgui/font_icon.h"
#include "logger/Logger.h"

namespace gallus
{
	namespace gameplay
	{
		//---------------------------------------------------------------------
		// MeshSystem
		//---------------------------------------------------------------------
		bool MeshSystem::Initialize()
		{
			LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_ECS, "Mesh system initialized.");
			return true;
		}

		//---------------------------------------------------------------------
		std::string MeshSystem::GetPropertyName() const
		{
			return "meshInfo";
		}

		//---------------------------------------------------------------------
		std::string MeshSystem::GetSystemName() const
		{
			std::string name = "Mesh";
#ifdef _EDITOR
			name = std::string(font::ICON_CUBE) + " " + name;
#endif // _EDITOR
			return name;
		}

		void MeshSystem::Update(float a_fDeltaTime)
		{}
	}
}