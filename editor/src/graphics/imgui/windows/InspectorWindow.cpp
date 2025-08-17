#include "InspectorWindow.h"

#include <imgui/imgui_helpers.h>

#include "graphics/imgui/ImGuiWindow.h"
#include "graphics/imgui/font_icon.h"
#include "graphics/imgui/views/inspector/InspectorView.h"
#include "core/EditorTool.h"
#include <imgui/imgui_internal.h>

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			namespace editor
			{
				InspectorWindow::InspectorWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(font::ICON_CIRCLE_INFO) + " Inspector", "Inspector"), m_NameInput(a_Window)
				{
					m_NameInput.Initialize("");
				}

				bool InspectorWindow::Destroy()
				{
					return true;
				}

				void InspectorWindow::Render()
				{
					InspectorView* inspectorView = core::EDITOR_TOOL->GetInspectorView();
					if (!inspectorView)
					{
						return;
					}

					ImVec2 initialPos = ImGui::GetCursorPos();
					ImGui::SetCursorPos(ImVec2(initialPos.x + m_Window.GetFramePadding().x, initialPos.y + m_Window.GetFramePadding().y));

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_Window.GetWindowPadding());
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));

					if (ImGui::BeginChild(
						ImGui::IMGUI_FORMAT_ID("", CHILD_ID, "INSPECTOR_TOOLBAR").c_str(),
						ImVec2(
						ImGui::GetContentRegionAvail().x - m_Window.GetFramePadding().x,
						m_Window.GetHeaderSize().y
						),
						ImGuiChildFlags_None
						))
					{
						std::string icon = inspectorView->GetIcon();

						if (!icon.empty())
						{
							ImVec2 pos = ImGui::GetCursorPos();
							if (ImGui::BeginChild(
								ImGui::IMGUI_FORMAT_ID("", CHILD_ID, "INSPECTOR_TOOLBAR_ICON").c_str(),
								m_Window.GetHeaderSize(),
								ImGuiChildFlags_None
								))
							{
								ImGui::PushFont(m_Window.GetBigIconFont());
								ImVec2 textSize = ImGui::CalcTextSize(icon.c_str());
								ImVec2 childSize = m_Window.GetHeaderSize();

								// Offset = half the difference between child height and text height
								float verticalOffset = (childSize.y - textSize.y) * 0.5f;

								ImVec2 pos = ImGui::GetCursorPos();
								pos.y += verticalOffset;

								ImGui::SetCursorPos(pos);
								ImGui::TextUnformatted(icon.c_str());

								ImGui::PopFont();
							}
							ImGui::EndChild();

							ImGui::SameLine();

							if (!inspectorView->GetShowRename())
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							}
							m_NameInput.SetString(inspectorView->GetName());
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							if (m_NameInput.Render(ImGui::IMGUI_FORMAT_ID("", INPUT_ID, "NAME_INSPECTOR").c_str(), ImGuiInputTextFlags_EnterReturnsTrue))
							{
								inspectorView->OnRename(m_NameInput.GetString());
							}
							if (!inspectorView->GetShowRename())
							{
								ImGui::PopItemFlag();
							}
						}
					}
					ImGui::EndChild();

					ImVec2 toolbarPos = ImGui::GetCursorPos();
					ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.GetHeaderSize().y);

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

					ImGui::BeginToolbar(toolbarSize);

					ImGui::PushFont(m_Window.GetIconFont());

					if (inspectorView->GetShowShowInExplorer())
					{
						if (ImGui::TextButton(
							ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_FOLDER_SHOW), BUTTON_ID, "SHOW_IN_EXPLORER_INSPECTOR").c_str(), m_Window.GetHeaderSize()))
						{
							inspectorView->OnShowInExplorer();
						}
						ImGui::SameLine();
					}
					if (inspectorView->GetShowDelete())
					{
						if (ImGui::TextButton(
							ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_DELETE), BUTTON_ID, "DELETE_INSPECTOR").c_str(), m_Window.GetHeaderSize()))
						{
							inspectorView->OnDelete();
						}
						ImGui::SameLine();
					}

					ImGui::PopStyleVar();

					ImGui::PopFont();

					ImGui::SetCursorPos(ImVec2(toolbarPos.x, toolbarPos.y + toolbarSize.y));

					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + m_Window.GetFramePadding().x, ImGui::GetCursorPos().y + m_Window.GetFramePadding().y));
					if (ImGui::BeginChild(
						ImGui::IMGUI_FORMAT_ID("", CHILD_ID, "INSPECTOR_VIEW").c_str(),
						ImVec2(
						ImGui::GetContentRegionAvail().x - m_Window.GetFramePadding().x,
						ImGui::GetContentRegionAvail().y - m_Window.GetFramePadding().y),
						ImGuiChildFlags_Borders))
					{
						inspectorView->Render();
					}
					ImGui::EndChild();

					ImGui::PopStyleVar();
					ImGui::PopStyleVar();
				}
			}
		}
	}
}
