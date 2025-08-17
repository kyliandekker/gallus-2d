#ifndef IMGUI_DISABLE
#ifdef _EDITOR

#include "graphics/imgui/windows/HierarchyWindow.h"

#include <imgui/imgui_helpers.h>

#include "graphics/imgui/font_icon.h"
#include "graphics/imgui/ImGuiWindow.h"
#include "core/EditorTool.h"
#include "utils/string_extensions.h"
#include "graphics/imgui/views/inspector/EntityInspectorView.h"

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

					// This needs to be done at the start of the frame to avoid errors.
					// We refresh the assets that show up based on the search bar and the root directory.
					if (m_bNeedsRefresh)
					{
						SetSelectable(nullptr);

						m_aEntities.clear();
						m_aFilteredEntities.clear();

						for (auto& entity : core::TOOL->GetECS().GetEntities())
						{
							m_aEntities.emplace_back(m_Window, entity.GetEntityID());
						}

						for (HierarchyEntityUIView& view : m_aEntities)
						{
							gameplay::Entity* entity = core::TOOL->GetECS().GetEntity(view.GetEntityID());
							if (!entity)
							{
								continue;
							}
							if (m_SearchBar.GetString().empty() || string_extensions::StringToLower(entity->GetName()).find(m_SearchBar.GetString()) != std::string::npos)
							{
								m_aFilteredEntities.push_back(&view);
							}
						}

						m_bNeedsRefresh = false;
					}

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
						core::EDITOR_TOOL->SetSelectable(nullptr, nullptr);

						core::TOOL->GetECS().CreateEntity(core::TOOL->GetECS().GetUniqueName("New GameObject"));
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
						for (HierarchyEntityUIView* view : m_aFilteredEntities)
						{
							if (!view)
							{
								continue;
							}

							bool
								clicked = false,
								right_clicked = false;
							view->RenderEntity(clicked, right_clicked, core::EDITOR_TOOL->GetSelectable() == view);
							if (clicked)
							{
								SetSelectable(view);
							}
						}
						ImGui::PopStyleVar();
					}
					ImGui::EndChild();
				}

				void HierarchyWindow::UpdateEntities()
				{
					m_bNeedsRefresh = true;
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

				void HierarchyWindow::SetSelectable(HierarchyEntityUIView* a_EntityView)
				{
					core::EDITOR_TOOL->SetSelectable(a_EntityView, a_EntityView ? new EntityInspectorView(m_Window, *a_EntityView) : nullptr);
				}
			}
		}
	}
}

#endif // _EDITOR
#endif // IMGUI_DISABLE