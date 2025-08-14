#ifdef _EDITOR

#include "graphics/imgui/windows/ExplorerWindow.h"

#include <imgui/imgui_helpers.h>

#include "graphics/imgui/ImGuiWindow.h"
#include "graphics/imgui/font_icon.h"
#include "utils/string_extensions.h"
#include "utils/file_abstractions.h"
#include "core/EditorTool.h"
#include "editor/FileResource.h"

#include "graphics/dx12/CommandQueue.h"
#include "graphics/dx12/CommandList.h"
#include "graphics/dx12/Texture.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			namespace editor
			{
				ExplorerWindow::ExplorerWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(font::ICON_FOLDER) + " Explorer", "Explorer"), m_SearchBar(a_Window)
				{
					m_SearchBar.Initialize("");

					//m_PreviousFolder.SetIcon(font::ICON_FILE_FOLDER);
					//m_PreviousFolder.SetName("../");
					//m_PreviousFolder.SetFileResourceUIType(FileResourceUIType::FileResourceType_PreviousFolder);
					//m_PreviousFolder.SetFileResourceType(FileResourceType::Folder);
				}

				bool ExplorerWindow::Initialize()
				{
					//OnScanCompleted();

					//core::TOOL->GetEditor().GetAssetDatabase().m_OnBeforeScan += std::bind(&ExplorerWindow::OnBeforeScan, this);
					//core::TOOL->GetEditor().GetAssetDatabase().m_OnScanCompleted += std::bind(&ExplorerWindow::OnScanCompleted, this);

					return BaseWindow::Initialize();
				}

				bool ExplorerWindow::Destroy()
				{
					//core::TOOL->GetEditor().GetAssetDatabase().m_OnBeforeScan -= std::bind(&ExplorerWindow::OnBeforeScan, this);
					//core::TOOL->GetEditor().GetAssetDatabase().m_OnScanCompleted -= std::bind(&ExplorerWindow::OnScanCompleted, this);

					return BaseWindow::Destroy();
				}

				//void ExplorerWindow::SetExplorerRoot(FileResourceUIView* a_Resource)
				//{
				//	if (!a_Resource)
				//	{
				//		return;
				//	}

				//	m_FolderRoot = a_Resource;
				//	m_PreviousFolderRoot = m_FolderRoot->GetResource()->GetPath();
				//}

				/*
				void ExplorerWindow::LoadTexture(FileResourceUIView* a_ResourceView)
				{
					if (!m_PreviewTexture)
					{
						m_PreviewTexture = core::TOOL->GetDX12().GetResourceAtlas().LoadTextureEmpty(L"Explorer Preview Texture");
					}

					m_PreviewTexture->SetResourceCategory(graphics::dx12::DX12ResourceCategory::Editor);

					if (m_PreviewTexture->IsValid())
					{
						m_PreviewTexture->Destroy();
					}

					auto cCommandQueue = core::TOOL->GetDX12().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
					auto cCommandList = cCommandQueue->GetCommandList();
					m_PreviewTexture->LoadByPath(a_ResourceView->GetResource()->GetPath(), cCommandList);
					cCommandQueue->ExecuteCommandList(cCommandList);
					cCommandQueue->Flush();

					core::TOOL->GetEditor().m_PreviewTexture = m_PreviewTexture;
				}*/

				void ExplorerWindow::RenderFolder(ExplorerFileUIView& a_Resource, int a_Indent, const ImVec2& a_InitialPos)
				{
					bool
						clicked = false,
						right_clicked = false;

					a_Resource.RenderTree(clicked, right_clicked, false /*This should be highlighted if it is the root.*/, a_Indent, a_InitialPos, false /*highlighted if selected*/);

					//if (clicked)
					//{
					//	m_NewFolderRoot = a_Resource;
					//}

					//if (right_clicked)
					//{
					//	m_SelectedResource = a_Resource;
					//}

					if (a_Resource.IsFoldedOut())
					{

					}
				}

				void ExplorerWindow::Render()
				{
					// This needs to be done at the start of the frame to avoid errors.
					// We set the root directory of the second window that shows the assets.
					//if (m_NewFolderRoot)
					//{
					//	if (m_NewFolderRoot != m_FolderRoot)
					//	{
					//		if (FileResourceUIView* derivedPtr = dynamic_cast<FileResourceUIView*>(core::TOOL->GetEditor().GetSelectable()))
					//		{
					//			core::TOOL->GetEditor().SetSelectable(nullptr);
					//		}

					//		SetExplorerRoot(m_NewFolderRoot);
					//		m_NeedsRefresh = true;
					//	}
					//	m_NewFolderRoot = nullptr;
					//	m_SelectedResource = nullptr;
					//}

					// This needs to be done at the start of the frame to avoid errors.
					// We refresh the assets that show up based on the search bar and the root directory.
					if (m_bNeedsRefresh)
					{
						m_aExplorerItems.clear();

						for (gallus::editor::FileResource& fileResource : gallus::core::EDITOR_TOOL->GetAssetDatabase().GetRoot().GetResources())
						{
							m_aExplorerItems.emplace_back(m_Window, fileResource);
						}
						m_bNeedsRefresh = false;
					}

					//if (!m_FolderRoot)
					//{
					//	return;
					//}

					ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.GetHeaderSize().y);
					ImGui::BeginToolbar(toolbarSize);

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

					float topPosY = ImGui::GetCursorPosY();

					if (ImGui::IconButton(
						ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_REFRESH), BUTTON_ID, "RESCAN_EXPLORER").c_str(), m_Window.GetHeaderSize(), m_Window.GetIconFont(), ImGui::GetStyleColorVec4(ImGuiCol_TextColorAccent)))
					{
						//core::TOOL->GetEditor().GetAssetDatabase().Rescan();
						m_bNeedsRefresh = true;
					}

					ImGui::SameLine();

					bool list = m_ExplorerViewMode == ExplorerViewMode::ExplorerViewMode_List;
					bool grid = m_ExplorerViewMode == ExplorerViewMode::ExplorerViewMode_Grid;
					if (ImGui::IconCheckboxButton(
						ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_LIST), BUTTON_ID, "LIST_EXPLORER").c_str(), &list, m_Window.GetHeaderSize(), m_Window.GetIconFont(), list ? ImGui::GetStyleColorVec4(ImGuiCol_TextColorAccent) : ImGui::GetStyleColorVec4(ImGuiCol_Text)))
					{
						if (list)
						{
							m_ExplorerViewMode = ExplorerViewMode::ExplorerViewMode_List;
						}
					}

					ImGui::SameLine();

					if (ImGui::IconCheckboxButton(
						ImGui::IMGUI_FORMAT_ID(std::string(font::ICON_GRID), BUTTON_ID, "GRID_EXPLORER").c_str(), &grid, m_Window.GetHeaderSize(), m_Window.GetIconFont(), grid ? ImGui::GetStyleColorVec4(ImGuiCol_TextColorAccent) : ImGui::GetStyleColorVec4(ImGuiCol_Text)))
					{
						if (grid)
						{
							m_ExplorerViewMode = ExplorerViewMode::ExplorerViewMode_Grid;
						}
					}

					ImVec2 endPos = ImGui::GetCursorPos();

					float searchbarWidth = 300;
					float inputPadding = m_Window.GetFramePadding().x / 2;
					ImVec2 searchBarPos = ImVec2(
						ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (searchbarWidth + m_Window.GetWindowPadding().x),
						(topPosY + (toolbarSize.y / 2)) - (((inputPadding * 2) + m_Window.GetFontSize()) / 2)
					);
					ImGui::SetCursorPos(searchBarPos);
					if (m_SearchBar.Render(ImGui::IMGUI_FORMAT_ID("", INPUT_ID, "SEARCHBAR_EXPLORER").c_str(), ImVec2(searchbarWidth, toolbarSize.y), inputPadding))
					{
						//m_NeedsRefresh = true;
					}

					ImGui::SetCursorPos(endPos);

					ImGui::PopStyleVar();
					ImGui::PopStyleVar();

					ImGui::EndToolbar(ImVec2(m_Window.GetWindowPadding().x, 0));

					//m_ShowPopUp = false;

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_Window.GetWindowPadding());
					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + m_Window.GetFramePadding().y);
					if (ImGui::BeginChild(
						ImGui::IMGUI_FORMAT_ID("", CHILD_ID, "DIRECTORIES_EXPLORER").c_str(),
						ImVec2(
						0,
						ImGui::GetContentRegionAvail().y - m_Window.GetFramePadding().y
						),
						ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX
						))
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
						for (ExplorerFileUIView view : m_aExplorerItems)
						{
							RenderFolder(view, 0, ImGui::GetCursorPos());
						}
						ImGui::PopStyleVar();
					}
					ImGui::EndChild();

					std::string popUpID = ImGui::IMGUI_FORMAT_ID("", POPUP_WINDOW_ID, "RESOURCE_OPTIONS_EXPLORER");

					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_ExplorerViewMode == ExplorerViewMode::ExplorerViewMode_List ? ImVec2() : m_Window.GetWindowPadding());
					if (ImGui::BeginChild(
						ImGui::IMGUI_FORMAT_ID("", CHILD_ID, "FILES_EXPLORER").c_str(),
						ImVec2(
						ImGui::GetContentRegionAvail().x - m_Window.GetFramePadding().x,
						ImGui::GetContentRegionAvail().y - m_Window.GetFramePadding().y
						),
						ImGuiChildFlags_Borders
						))
					{

					}
					ImGui::EndChild();
					ImGui::PopStyleVar();
					ImGui::PopStyleVar();
				}
			}
		}
	}
}

#endif // _EDITOR