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
					/// Renders a folder in the explorer.
					/// </summary>
					/// <param name="a_Resource">Pointer to the file resource.</param>
					/// <param name="a_Indent">Amount of indents.</param>
					/// <param name="a_InitialPos">The starting pos for indent 0.</param>
					void RenderFolder(ExplorerFileUIView& a_Resource, int a_Indent, const ImVec2& a_InitialPos);

					/// <summary>
					/// Renders the explorer window.
					/// </summary>
					void Render() override;

					/// <summary>
					/// Callback function for when the scanning of the explorer has been completed.
					/// </summary>
					void OnScanCompleted();
				private:
					ExplorerViewMode m_ExplorerViewMode = ExplorerViewMode::ExplorerViewMode_List; // How are explorer resources shown?

					bool m_bNeedsRescan = true; /// Whether the explorer needs to refresh the results shown in the explorer window.
					bool m_bNeedsRefresh = true; /// Whether the explorer needs to refresh the results shown in the explorer window.

					std::vector<ExplorerFileUIView> m_aExplorerItems;
					std::vector<ExplorerFileUIView*> m_aFilteredExplorerItems;

					ExplorerFileUIView* m_pViewedFolder = nullptr; /// Selected resource used for context menu.

					SearchBarInput m_SearchBar; /// Search bar to filter specific explorer items in the explorer window.
				};
			}
		}
	}
}

#endif // _EDITOR