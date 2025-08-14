#include "gameplay/systems/TransformSystem.h"

#include "graphics/imgui/font_icon.h"
#include "logger/Logger.h"

namespace gallus
{
	namespace gameplay
	{
		//---------------------------------------------------------------------
		// TransformSystem
		//---------------------------------------------------------------------
		bool TransformSystem::Initialize()
		{
			LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_ECS, "Transform system initialized.");
			return true;
		}

		//---------------------------------------------------------------------
		std::string TransformSystem::GetPropertyName() const
		{
			return "transform";
		}

		//---------------------------------------------------------------------
		std::string TransformSystem::GetSystemName() const
		{
			std::string name = "Transform";
#ifdef _EDITOR
			name = std::string(font::ICON_AXIS) + " " + name;
#endif // _EDITOR
			return name;
		}

		void TransformSystem::Update(float a_fDeltaTime)
		{}
	}
}