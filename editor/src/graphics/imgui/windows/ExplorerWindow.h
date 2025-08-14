#pragma once

#ifdef _EDITOR

#include <vector>
#include <string>
#include <memory>

#include "graphics/imgui/windows/BaseWindow.h"
#include "graphics/imgui/views/DataTypes/StringTextInput.h"
#include "graphics/imgui/views/ExplorerFileUIView.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				enum class ExplorerViewMode
				{
					ExplorerViewMode_List,
					ExplorerViewMode_Grid
				};

				/// <summary>
				/// A window that displays and manages the asset database of the project.
				/// Allows navigation, inspection, and organization of assets in a folder-like structure.
				/// </summary>
				class ExplorerWindow : public BaseWindow
				{
				public:
					/// <summary>
					/// Constructs an explorer window.
					/// </summary>
					/// <param name="a_Window">The ImGui window for rendering the view.</param>
					ExplorerWindow(ImGuiWindow& a_Window);

					/// <summary>
					/// Initializes all values and behaviours associated with the explorer window.
					/// </summary>
					/// <returns>True if initialization is successful, otherwise false.</returns>
					bool Initialize() override;

					/// <summary>
					/// Destroys and disables the explorer window.
					/// </summary>
					/// <returns>True if destruction is successful, otherwise false.</returns>
					bool Destroy() override;

					/// <summary>
					/// Sets the root folder of the explorer that shows the files.
					/// </summary>
					/// <param name="a_Resource">Pointer to the file resource.</param>
					//void SetExplorerRoot(FileResourceUIView* a_Resource);

					/// <summary>
					/// Renders a folder in the explorer.
					/// </summary>
					/// <param name="a_Resource">Pointer to the file resource.</param>
					/// <param name="a_Indent">Amount of indents.</param>
					/// <param name="a_InitialPos">The starting pos for indent 0.</param>
					void RenderFolder(ExplorerFileUIView& a_Resource, int a_Indent, const ImVec2& a_InitialPos);

					/// <summary>
					/// Loads a texture and sets it as preview texture.
					/// </summary>
					/// <param name="a_ResourceView">The resource containing the image.</param>
					//void LoadTexture(FileResourceUIView* a_ResourceView);

					/// <summary>
					/// Renders the explorer window.
					/// </summary>
					void Render() override;

					/// <summary>
					/// Callback function for when the scanning of the explorer has been completed.
					/// </summary>
					//void OnScanCompleted();

					/// <summary>
					/// Callback function for when the scanning of the explorer has been called.
					/// </summary>
					//void OnBeforeScan();

					ExplorerViewMode m_ExplorerViewMode = ExplorerViewMode::ExplorerViewMode_List; // How are explorer resources shown?
					//std::vector<FileResourceUIView*> m_FilteredResources; /// List of explorer items shown in the explorer window.
					//FileResourceUIView* m_AssetRoot = nullptr; /// Root of the asset database.
					//FileResourceUIView* m_FolderRoot = nullptr; /// Root of the folder that is shown in the explorer window.
					//FileResourceUIView* m_NewFolderRoot = nullptr; /// Temporary variable that detects changes in root folder structures.
					//FileResourceUIView m_PreviousFolder; /// Temporary variable that refers to the parent folder.
				private:
					bool m_bNeedsRefresh = true; /// Whether the explorer needs to refresh the results shown in the explorer window.

					std::vector<ExplorerFileUIView> m_aExplorerItems;

					//FileResourceUIView* m_SelectedResource = nullptr; /// Selected resource used for context menu.
					//fs::path m_PreviousFolderRoot;

					SearchBarInput m_SearchBar; /// Search bar to filter specific explorer items in the explorer window.

					//std::shared_ptr<graphics::dx12::Texture> m_PreviewTexture = nullptr; /// The preview texture shown in the inspector window.
				};
			}
		}
	}
}

#endif // _EDITOR