#pragma once

#ifndef IMGUI_DISABLE
#ifdef _EDITOR

#include "graphics/imgui/windows/BaseWindow.h"

#include <vector>

#include "gameplay/EntityID.h"
#include "graphics/imgui/views/DataTypes/StringTextInput.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				//---------------------------------------------------------------------
				// HierarchyWindow
				//---------------------------------------------------------------------
				/// <summary>
				/// A window that displays and manages the entities in a scene.
				/// Allows navigation and inspection of entities in a folder-like structure.
				/// </summary>
				class HierarchyWindow : public BaseWindow
				{
				public:
					/// <summary>
					/// Constructs a console window.
					/// </summary>
					/// <param name="a_Window">The ImGui window for rendering the view.</param>
					HierarchyWindow(ImGuiWindow& a_Window);

					/// <summary>
					/// Cleans up and destroys the console window.
					/// </summary>
					~HierarchyWindow();

					/// <summary>
					/// Initializes all values and behaviours associated with the hierarchy window.
					/// </summary>
					/// <returns>True if initialization is successful, otherwise false.</returns>
					bool Initialize() override;

					/// <summary>
					/// Destroys and disables the hierarchy window.
					/// </summary>
					/// <returns>True if destruction is successful, otherwise false.</returns>
					bool Destroy() override;

					/// <summary>
					/// Renders the console window.
					/// </summary>
					void Render() override;

					/// <summary>
					/// Updates all the entities shown in the hierarchy window.
					/// </summary>
					void UpdateEntities();

					/// <summary>
					/// Updates the components shown in the inspector window if applicable.
					/// </summary>
					void UpdateEntityComponents();

					std::string GetIcon(const gameplay::EntityID& a_EntityID) const;

					/// <summary>
					/// Renders the entity UI with selection and click interaction.
					/// </summary>
					/// <param name="a_EntityID">Entity to show.</param>
					/// <param name="a_sIcon">Icon to show in the inspector.</param>
					/// <param name="a_bClicked">Reference to a boolean indicating if the entity was clicked.</param>
					/// <param name="a_bDoubleClicked">Reference to a boolean indicating if the entity was double clicked.</param>
					/// <param name="a_bSelected">Boolean indicating if the entity is currently selected.</param>
					void RenderEntity(const gameplay::EntityID& a_EntityID, const std::string& a_sIcon, bool& a_bClicked, bool& a_bDoubleClicked, bool a_bSelected);
				private:
					gameplay::EntityID m_LastID; /// The last ID that was selected in the hierarchy view.
					bool m_bNeedsRefresh = true; /// Whether the hierarchy needs to refresh the results shown in the hierarchy window.
					
					std::vector<gameplay::EntityID> m_aFilteredEntities; /// List of entities shown in the hierarchy window.
					std::vector<std::string> m_aEntityIcons; /// List of entities shown in the hierarchy window.

					SearchBarInput m_SearchBar; /// Search bar to filter specific messages in the console window.
				};
			}
		}
	}
}

#endif // _EDITOR
#endif // IMGUI_DISABLE