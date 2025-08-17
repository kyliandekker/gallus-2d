#pragma once

#include "graphics/imgui/views/inspector/InspectorView.h"

#include <string>

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				class ExplorerFileUIView;

				/// <summary>
				/// Class that displays entities in the inspector.
				/// </summary>
				class ExplorerFileInspectorView : public InspectorView
				{
				public:
					/// <summary>
					/// Constructs an inspector view.
					/// </summary>
					/// <param name="a_Window">The ImGui window for rendering the view.</param>
					/// <param name="a_EntityID">The entity that will be shown in the view.</param>
					ExplorerFileInspectorView(ImGuiWindow& a_Window, ExplorerFileUIView& a_ExplorerFileUIView);

					void OnRename(const std::string& a_sName) override;

					void OnDelete() override;

					void OnShowInExplorer() override;

					std::string GetName() const override;

					std::string GetIcon() const override;

					void Render();
				protected:
					ExplorerFileUIView& m_ExplorerFileUIView;
				};
			}
		}
	}
}