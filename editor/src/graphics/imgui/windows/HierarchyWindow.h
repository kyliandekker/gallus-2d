#pragma once

#ifndef IMGUI_DISABLE
#ifdef _EDITOR

#include "graphics/imgui/windows/BaseWindow.h"

#include <vector>

#include "gameplay/EntityID.h"
#include "graphics/imgui/views/DataTypes/StringTextInput.h"
#include "graphics/imgui/views/EntityUIView.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				/// <summary>
				/// A window that displays and manages the entities in a scene.
				/// Allows navigation and inspection of entities in a folder-like structure.
				/// </summary>
				//-----------------------------------------------------------------------------
				// HierarchyWindow
				//-----------------------------------------------------------------------------
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
				private:
					gameplay::EntityID m_LastID; /// The last ID that was selected in the hierarchy view.
					bool m_NeedsRefresh = true; /// Whether the hierarchy needs to refresh the results shown in the hierarchy window.
					std::vector<EntityUIView> m_FilteredEntities; /// List of entities shown in the hierarchy window.

					SearchBarInput m_SearchBar; /// Search bar to filter specific messages in the console window.
				};
			}
		}
	}
}

#endif // _EDITOR
#endif // IMGUI_DISABLE