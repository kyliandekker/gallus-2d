#ifndef IMGUI_DISABLE
#ifdef _EDITOR

#include "graphics/imgui/windows/HierarchyWindow.h"

#include <imgui/imgui_helpers.h>

#include "graphics/imgui/font_icon.h"
#include "graphics/imgui/ImGuiWindow.h"
#include "core/EditorTool.h"
#include "utils/string_extensions.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			namespace editor
			{
				//---------------------------------------------------------------------
				// HierarchyWindow
				//---------------------------------------------------------------------
				HierarchyWindow::HierarchyWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(font::ICON_LIST) + " Hierarchy", "Hierarchy"), m_SearchBar(a_Window)
				{
					m_SearchBar.Initialize("");
				}

				//---------------------------------------------------------------------
				HierarchyWindow::~HierarchyWindow()
				{}

				//---------------------------------------------------------------------
				bool HierarchyWindow::Initialize()
				{
					core::TOOL->GetECS().OnEntitiesUpdated() += std::bind(&HierarchyWindow::UpdateEntities, this);
					core::TOOL->GetECS().OnEntityComponentsUpdated() += std::bind(&HierarchyWindow::UpdateEntityComponents, this);
					return BaseWindow::Initialize();
				}

				//---------------------------------------------------------------------
				bool HierarchyWindow::Destroy()
				{
					core::TOOL->GetECS().OnEntitiesUpdated() -= std::bind(&HierarchyWindow::UpdateEntities, this);
					core::TOOL->GetECS().OnEntityComponentsUpdated() -= std::bind(&HierarchyWindow::UpdateEntityComponents, this);
					return BaseWindow::Destroy();
				}

				//---------------------------------------------------------------------
				void HierarchyWindow::Render()
				{
					std::lock_guard<std::recursive_mutex> lock(core::TOOL->GetECS().m_EntityMutex);

					//if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S) && core::TOOL->GetEditor().GetCurrentScene() && core::TOOL->GetEditor().GetCurrentScene()->IsDirty())
					//{
					//}

					ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.GetHeaderSize().y);
					ImGui::BeginToolbar(toolbarSize);

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

					float topPosY = ImGui::GetCursorPosY();

					if (ImGui::IconButton(
						ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_CUBE), BUTTON_ID, "SPAWN_ENTITY_HIERARCHY").c_str(), m_Window.GetHeaderSize(), m_Window.GetIconFont()))
					{
						core::TOOL->GetECS().CreateEntity(core::TOOL->GetECS().GetUniqueName("New GameObject"));
						m_bNeedsRefresh = true;
						//if (core::TOOL->GetEditor().GetCurrentScene() && !core::TOOL->GetECS().HasStarted())
						//{
						//	core::TOOL->GetEditor().SetDirty();
						//}
					}
					ImGui::SameLine();

					//bool wasDirty = !core::TOOL->GetEditor().GetCurrentScene() || !core::TOOL->GetEditor().GetCurrentScene()->IsDirty();
					//if (wasDirty)
					//{
					//	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					//}

					if (ImGui::IconButton(
						ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_SAVE), BUTTON_ID, "SAVE_HIERARCHY").c_str(), m_Window.GetHeaderSize(), m_Window.GetIconFont(), ImGui::GetStyleColorVec4(ImGuiCol_TextColorAccent)))
					{
						//core::TOOL->GetEditor().SaveScene();
					}

					ImVec2 endPos = ImGui::GetCursorPos();

					//if (wasDirty)
					//{
					//	ImGui::PopItemFlag();
					//	ImGui::PopStyleVar();
					//}

					ImGui::PopStyleVar();
					ImGui::PopStyleVar();

					float searchbarWidth = 200;
					float inputPadding = m_Window.GetFramePadding().x / 2;
					ImVec2 searchBarPos = ImVec2(
						ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (searchbarWidth + m_Window.GetWindowPadding().x),
						(topPosY + (toolbarSize.y / 2)) - (((inputPadding * 2) + m_Window.GetFontSize()) / 2)
					);
					ImGui::SetCursorPos(searchBarPos);
					if (m_SearchBar.Render(ImGui::IMGUI_FORMAT_ID("", INPUT_ID, "HIERARCHY_CONSOLE").c_str(), ImVec2(searchbarWidth, toolbarSize.y), inputPadding))
					{
						m_bNeedsRefresh = true;
					}

					ImGui::SetCursorPos(endPos);

					ImGui::EndToolbar(ImVec2(0, 0));

					if (m_bNeedsRefresh)
					{
						m_aFilteredEntities.clear();

						m_bNeedsRefresh = false;

						bool isEmptyString = m_SearchBar.GetString().empty();

						for (auto& entity : core::TOOL->GetECS().GetEntities())
						{
							if (isEmptyString || string_extensions::StringToLower(entity.GetName()).find(m_SearchBar.GetString()) != std::string::npos)
							{
								m_aFilteredEntities.push_back(entity.GetEntityID());
								m_aEntityIcons.push_back(GetIcon(entity.GetEntityID()));
							}
						}
					}

					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + m_Window.GetFramePadding().x, ImGui::GetCursorPos().y + m_Window.GetFramePadding().y));
					if (ImGui::BeginChild(
						ImGui::IMGUI_FORMAT_ID("", CHILD_ID, "ENTITIES_HIERARCHY").c_str(),
						ImVec2(
						ImGui::GetContentRegionAvail().x - m_Window.GetFramePadding().x,
						ImGui::GetContentRegionAvail().y - m_Window.GetFramePadding().y
						),
						ImGuiChildFlags_Borders
						))
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
						for (size_t i = 0; i < m_aFilteredEntities.size(); i++)
						{
							gameplay::EntityID& entity = m_aFilteredEntities[i];
							std::string& icon = m_aEntityIcons[i];

							bool clicked = false, doubleClicked = false;
							RenderEntity(entity, icon, clicked, doubleClicked, /*&entity == core::TOOL->GetEditor().GetSelectable()*/ false);
							if (clicked)
							{
								//m_LastID = entity.GetEntityID();
								//core::TOOL->GetEditor().SetSelectable(&entity);
							}
							if (doubleClicked)
							{
								//if (core::TOOL->GetECS().GetSystem<gameplay::TransformSystem>().HasComponent(entity.GetEntityID()))
								//{
								//	auto& transformComponent = core::TOOL->GetECS().GetSystem<gameplay::TransformSystem>().GetComponent(entity.GetEntityID());
								//	core::TOOL->GetDX12().GetEditorCamera().GetTransform().SetPosition(transformComponent.Transform().GetPosition());
								//}
								//else if (core::TOOL->GetECS().GetSystem<gameplay::CameraSystem>().HasComponent(entity.GetEntityID()))
								//{
								//	auto& cameraComponent = core::TOOL->GetECS().GetSystem<gameplay::CameraSystem>().GetComponent(entity.GetEntityID());
								//	core::TOOL->GetDX12().GetEditorCamera().GetTransform().SetPosition(cameraComponent.GetCamera().GetTransform().GetPosition());
								//}
							}
						}
						ImGui::PopStyleVar();
					}
					ImGui::EndChild();
				}

				void HierarchyWindow::UpdateEntities()
				{
					m_bNeedsRefresh = true;

					//core::ENGINE.GetEditor().SetDirty();
				}

				void HierarchyWindow::UpdateEntityComponents()
				{
					//if (EntityUIView* derivedPtr = dynamic_cast<EntityUIView*>(core::ENGINE.GetEditor().GetSelectable()))
					//{
					//	core::ENGINE.GetEditor().GetSelectable()->Deselect();
					//	core::ENGINE.GetEditor().GetSelectable()->Select();
					//}

					//core::ENGINE.GetEditor().SetDirty();
				}

				std::string HierarchyWindow::GetIcon(const gameplay::EntityID& a_EntityID) const
				{
					return font::ICON_IMAGE;
				}

				void HierarchyWindow::RenderEntity(const gameplay::EntityID& a_EntityID, const std::string& a_sIcon, bool& a_bClicked, bool& a_bDoubleClicked, bool a_bSelected)
				{
					gameplay::Entity* entity = core::TOOL->GetECS().GetEntity(a_EntityID);
					if (!entity)
					{
						return;
					}

					bool wasInactive = !entity->IsActive();
					if (wasInactive)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					}

					// Set the size of each child
					ImVec2 childSize = ImVec2(ImGui::GetContentRegionAvail().x, 32);
					ImVec2 screenCursorPos = ImGui::GetCursorScreenPos();
					ImVec2 cursorPos = screenCursorPos;
					ImVec2 childEnd = ImVec2(cursorPos.x + childSize.x, cursorPos.y + childSize.y);

					if (a_bSelected)
					{
						ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, childEnd, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive)));
					}

					ImVec2 buttonCursorPos = ImVec2(ImGui::GetCursorPosX() + (m_Window.GetFramePadding().x * 3), ImGui::GetCursorPosY());

					ImVec2 min = ImGui::GetCursorScreenPos(); // Top-left corner
					ImVec2 max = ImVec2(min.x + childSize.x, min.y + childSize.y); // Bottom-right corner
					if (ImGui::IsMouseHoveringRect(min, max))
					{
						ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, childEnd, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered)));
						if (ImGui::IsMouseClicked(0))
						{
							a_bClicked = true;
						}
						if (ImGui::IsMouseDoubleClicked(0))
						{
							a_bDoubleClicked = true;
						}
					}

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					float checkboxHeight = ImGui::GetFrameHeight();
					ImVec2 centerPos = ImVec2(m_Window.GetWindowPadding().x, buttonCursorPos.y + (childSize.y - checkboxHeight) * 0.5f);
					ImGui::SetCursorPos(centerPos);
					bool temp = entity->IsActive();
					if (ImGui::Checkbox(ImGui::IMGUI_FORMAT_ID("", CHECKBOX_ID, string_extensions::StringToUpper(entity->GetName()) + "_HIERARCHY").c_str(), &temp))
					{
						entity->SetIsActive(temp);
						//core::TOOL->GetEditor().SetDirty();
					}
					ImGui::PopStyleVar();

					ImGui::PushFont(m_Window.GetIconFont());

					// Dynamically calculate the size of the icon
					ImVec2 iconSize = ImVec2(m_Window.GetFontSize(), m_Window.GetFontSize()); // Replace this with your icon size calculation.

					float iconOffset = m_Window.GetIconFont()->FontSize * 2.0f;

					// Calculate offsets for centering
					float verticalOffset = (childSize.y - iconSize.y) / 2.0f;   // Center vertically

					// Final position of the icon
					centerPos.y = buttonCursorPos.y + verticalOffset;

					// Dynamically calculate the size of the icon
					iconSize = ImGui::CalcTextSize(a_sIcon.c_str()); // Replace this with your icon size calculation.

					iconOffset = m_Window.GetIconFont()->FontSize * 2.0f;

					// Calculate offsets for centering
					verticalOffset = (childSize.y - iconSize.y) / 2.0f;   // Center vertically

					// Final position of the icon
					centerPos = ImVec2(centerPos.x + iconOffset, buttonCursorPos.y);
					centerPos.y += verticalOffset;

					// Set cursor to the calculated position and render the icon
					ImGui::SetCursorPos(centerPos);
					ImGui::Text(a_sIcon.c_str());

					ImGui::PopFont();

					ImVec2 textSize = ImGui::CalcTextSize(entity->GetName().c_str());

					// Calculate position to center the icon
					centerPos = ImVec2(
						centerPos.x + iconOffset,
						buttonCursorPos.y + (childSize.y - textSize.y) * 0.5f
					);
					ImGui::SetCursorPos(centerPos);
					ImGui::Text(entity->GetName().c_str());

					ImGui::SetCursorScreenPos(ImVec2(screenCursorPos.x, screenCursorPos.y + childSize.y));

					if (wasInactive)
					{
						ImGui::PopStyleVar();
					}
				}
			}
		}
	}
}

#endif // _EDITOR
#endif // IMGUI_DISABLE