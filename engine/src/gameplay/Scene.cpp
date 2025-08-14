#include "Scene.h"

#include <rapidjson/document.h>

#include "core/Tool.h"
#include "logger/Logger.h"

#include "gameplay/ECSBaseSystem.h"

#define JSON_SCENE_ENTITIES_VAR "entities"
#define JSON_SCENE_ENTITIES_VAR_NAME "name"
#define JSON_SCENE_ENTITIES_VAR_ACTIVE "isActive"

namespace gallus
{
	namespace gameplay
	{
		//---------------------------------------------------------------------
		// Scene
		//---------------------------------------------------------------------
		Scene::~Scene()
		{
			// Clear all entities.
			core::TOOL->GetECS().Clear();
		}

		//---------------------------------------------------------------------
		bool Scene::LoadData()
		{
			rapidjson::Document document;
			document.Parse(m_Data.dataAs<const char>(), m_Data.size());

			if (document.HasParseError())
			{
				LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_GAME, "Something went wrong when trying to load scene data.");
				return false;
			}

			// Clear all entities.
			core::TOOL->GetECS().Clear();

			// Load entities and systems.
			if (document.HasMember(JSON_SCENE_ENTITIES_VAR) && document[JSON_SCENE_ENTITIES_VAR].IsArray())
			{
				for (auto& element : document[JSON_SCENE_ENTITIES_VAR].GetArray())
				{
					if (!element.IsObject())
					{
						continue;
					}

					std::string name = core::TOOL->GetECS().GetUniqueName("New GameObject");
					if (element.HasMember(JSON_SCENE_ENTITIES_VAR_NAME) && element[JSON_SCENE_ENTITIES_VAR_NAME].IsString())
					{
						name = element[JSON_SCENE_ENTITIES_VAR_NAME].GetString();
					}

					gameplay::EntityID id = core::TOOL->GetECS().CreateEntity(name);
					gameplay::Entity* entity = core::TOOL->GetECS().GetEntity(id);
					if (!entity)
					{
						continue;
					}

					bool isActive = true;
					if (element.HasMember(JSON_SCENE_ENTITIES_VAR_ACTIVE) && element[JSON_SCENE_ENTITIES_VAR_ACTIVE].IsBool())
					{
						isActive = element[JSON_SCENE_ENTITIES_VAR_ACTIVE].GetBool();
					}

					for (gameplay::AbstractECSSystem* system : core::TOOL->GetECS().GetSystems())
					{
						if (element.HasMember(system->GetPropertyName().c_str()))
						{
							gameplay::Component* component = system->GetBaseComponent(id);
							if (!component)
							{
								continue;
							}

							const rapidjson::Value& testMember = element[system->GetPropertyName().c_str()];
							component->Deserialize(testMember, document.GetAllocator());
						}
					}
				}
			}

			return true;
		}

		//---------------------------------------------------------------------
		void Scene::SetData(const core::Data& a_Data)
		{
			m_Data = a_Data;
		}
	}
}