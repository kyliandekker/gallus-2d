#include "ExplorerFileUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/FileResource.h"
#include "graphics/imgui/ImGuiWindow.h"
#include "graphics/imgui/font_icon.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			namespace editor
			{
				const std::string& imgui::editor::ExplorerFileUIView::GetIcon() const
				{
					return m_sIcon;
				}

				void ExplorerFileUIView::SetIcon(const std::string& a_sIcon)
				{
					m_sIcon = a_sIcon;
				}

				bool ExplorerFileUIView::IsFoldedOut() const
				{
					return m_bIsFoldedOut;
				}

				void ExplorerFileUIView::SetFoldedOut(bool a_bIsFoldedOut)
				{
					m_bIsFoldedOut = a_bIsFoldedOut;
				}

				void ExplorerFileUIView::RenderTree(bool& a_bClicked, bool& a_bRightClicked, bool a_bSelected, int a_iIndent, const ImVec2& a_vInitialPos, bool a_bInContextMenu)
				{
					ImGui::SetCursorPos(ImVec2(a_vInitialPos.x, ImGui::GetCursorPos().y));

					// Set the size of each child
					ImVec2 childSize = ImVec2(ImGui::GetContentRegionAvail().x, 32);
					ImVec2 endCursorPos = ImGui::GetCursorPos();
					ImVec2 screenCursorPos = ImGui::GetCursorScreenPos();
					ImVec2 cursorPos = screenCursorPos;
					ImVec2 childEnd = ImVec2(cursorPos.x + childSize.x, cursorPos.y + childSize.y);

					if (a_bSelected)
					{
						ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, childEnd, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive)));
					}
					if (a_bInContextMenu)
					{
						ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, childEnd, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered)));
					}

					std::string name = m_FileResource.GetPath().filename().generic_string();

					ImVec2 buttonCursorPos = ImVec2(ImGui::GetCursorPosX() + (m_Window.GetFramePadding().x * 3), ImGui::GetCursorPosY());
					if (ImGui::InvisibleButton(ImGui::IMGUI_FORMAT_ID("", BUTTON_ID, "FILES_FOLDERS_INVIS_BUTTON_" + name).c_str(), childSize))
					{
						a_bClicked = true;
					}
					a_bRightClicked = ImGui::IsItemHovered() && ImGui::IsMouseClicked(1);
					if (ImGui::IsItemHovered())
					{
						ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, childEnd, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered)));
					}

					std::string foldIcon = m_bIsFoldedOut ? font::ICON_FOLDED_OUT : font::ICON_FOLDED_IN;

					float offsetPos = a_vInitialPos.x + (ImGui::GetStyle().IndentSpacing * a_iIndent);
					ImGui::PushFont(m_Window.GetIconFont());

					// Dynamically calculate the size of the icon
					ImVec2 iconSize = ImGui::CalcTextSize(foldIcon.c_str()); // Replace this with your icon size calculation.

					float iconOffset = m_Window.GetIconFont()->FontSize * 2.0f;

					// Calculate offsets for centering
					float verticalOffset = (childSize.y - iconSize.y) / 2.0f;   // Center vertically

					// Final position of the icon
					ImVec2 centerPos = ImVec2(offsetPos, buttonCursorPos.y);
					centerPos.y += verticalOffset;

					if (HasFolders())
					{
						// Set cursor to the calculated position and render the icon
						ImGui::SetCursorPos(centerPos);
						ImGui::Text(foldIcon.c_str());

						ImGui::SetCursorPos(centerPos);
						ImVec2 foldButtonScreenPos = ImGui::GetCursorPos();

						ImVec2 foldButtonMin = foldButtonScreenPos;
						ImVec2 foldButtonMax = ImVec2(foldButtonMin.x + iconSize.x, foldButtonMin.y + iconSize.y);

						ImVec2 mousePos = ImGui::GetMousePos();
						bool isFoldButtonHovered = (mousePos.x >= foldButtonMin.x && mousePos.x <= foldButtonMax.x &&
							mousePos.y >= foldButtonMin.y && mousePos.y <= foldButtonMax.y);

						bool isFoldButtonClicked = ImGui::IsMouseClicked(0) && isFoldButtonHovered;
						if (isFoldButtonClicked)
						{
							m_bIsFoldedOut = !m_bIsFoldedOut;
						}
					}

					std::string	folderIcon = m_bIsFoldedOut ? font::ICON_FOLDER_OPEN : font::ICON_FOLDER;

					// Dynamically calculate the size of the icon
					iconSize = ImGui::CalcTextSize(folderIcon.c_str()); // Replace this with your icon size calculation.

					iconOffset = m_Window.GetIconFont()->FontSize * 2.0f;

					// Calculate offsets for centering
					verticalOffset = (childSize.y - iconSize.y) / 2.0f;   // Center vertically

					// Final position of the icon
					centerPos = ImVec2(centerPos.x + iconOffset, buttonCursorPos.y);
					centerPos.y += verticalOffset;

					// Set cursor to the calculated position and render the icon
					ImGui::SetCursorPos(centerPos);
					ImGui::Text(folderIcon.c_str());

					ImGui::PopFont();

					ImVec2 textSize = ImGui::CalcTextSize(name.c_str());

					// Calculate position to center the icon
					centerPos = ImVec2(
						centerPos.x + iconOffset,
						buttonCursorPos.y + (childSize.y - textSize.y) * 0.5f
					);
					ImGui::SetCursorPos(centerPos);
					ImGui::Text(name.c_str());

					ImGui::SetCursorPos(ImVec2(endCursorPos.x, endCursorPos.y + childSize.y));
				}

				bool ExplorerFileUIView::HasFolders() const
				{
					for (const gallus::editor::FileResource& resource : m_FileResource.GetResources())
					{
						if (resource.GetResourceType() == gallus::editor::FileResourceType::Folder)
						{
							return true;
						}
					}
					return false;
				}
			}
		}
	}
}