#pragma once

#include "graphics/imgui/views/inspector/InspectorView.h"

#include <string>

namespace gallus
{
	namespace gameplay
	{
		class Entity;
	}
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				class HierarchyEntityUIView;

				/// <summary>
				/// Class that displays entities in the inspector.
				/// </summary>
				class EntityInspectorView : public InspectorView
				{
				public:
					/// <summary>
					/// Constructs an inspector view.
					/// </summary>
					/// <param name="a_Window">The ImGui window for rendering the view.</param>
					/// <param name="a_HierarchyEntityUIView">The entity that will be shown in the view.</param>
					EntityInspectorView(ImGuiWindow& a_Window, HierarchyEntityUIView& a_HierarchyEntityUIView);

					void OnRename(const std::string& a_sName) override;

					void OnDelete() override;

					std::string GetName() const override;

					std::string GetIcon() const override;

					void Render();
				protected:
					HierarchyEntityUIView& m_HierarchyEntityUIView;
					gameplay::Entity* m_Entity = nullptr;
				};
			}
		}
	}
}