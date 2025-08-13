#pragma once

#ifndef IMGUI_DISABLE
#ifdef _EDITOR

#include "graphics/imgui/views/ImGuiUIView.h"

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
				class ComponentBaseUIView;

				/// <summary>
				/// Represents a UI view for an entity in the editor, providing rendering and interaction functionality.
				/// </summary>
				class EntityUIView : public ImGuiUIView
				{
				public:
					/// <summary>
					/// Constructs an EntityUIView for a given entity ID.
					/// </summary>
					/// <param name="a_Window">The ImGui window for rendering the view.</param>
					/// <param name="a_EntityID">The entity ID associated with this view.</param>
					EntityUIView(ImGuiWindow& a_Window, const gameplay::EntityID& a_EntityID);

					/// <summary>
					/// Default constructor for EntityUIView.
					/// </summary>
					EntityUIView() = default;

					/// <summary>
					/// Retrieves the icon string representing this entity in the UI.
					/// </summary>
					/// <returns>A string representing the icon.</returns>
					std::string GetIcon() const;

					/// <summary>
					/// Empty render method (implemented because base class is pure).
					/// </summary>
					void Render() override
					{};

					/// <summary>
					/// Renders the entity UI with selection and click interaction.
					/// </summary>
					/// <param name="a_Clicked">Reference to a boolean indicating if the entity was clicked.</param>
					/// <param name="a_DoubleClicked">Reference to a boolean indicating if the entity was double clicked.</param>
					/// <param name="a_Selected">Boolean indicating if the entity is currently selected.</param>
					void Render(bool& a_Clicked, bool& a_DoubleClicked, bool a_Selected);

					///// <summary>
					///// Renders the selectable UI for the entity.
					///// </summary>
					//void RenderSelectable() override;

					///// <summary>
					///// Executes logic when the entity is selected.
					///// </summary>
					//void Select() override;

					///// <summary>
					///// Executes logic when the entity is deselected.
					///// </summary>
					//void Deselect() override;

					/// <summary>
					/// Retrieves the entity ID associated with this view.
					/// </summary>
					/// <returns>A reference to the EntityID.</returns>
					gameplay::EntityID GetEntityID() const;
				private:
					gameplay::EntityID m_EntityID; /// The entity ID associated with this view.
					StringTextInput m_NameInput; /// Inspector input for editing the entity's name.
				};
			}
		}
	}
}

#endif // _EDITOR
#endif // IMGUI_DISABLE