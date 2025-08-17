#pragma once

#include "core/Tool.h"
#include "editor/EditorSettings.h"
#include "editor/AssetDatabase.h"

#include <mutex>

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			namespace editor
			{
				class EditorSelectable;
				class InspectorView;
			}
		}
	}
	namespace core
	{
		//---------------------------------------------------------------------
		// EditorTool
		//---------------------------------------------------------------------
		class EditorTool : public Tool
		{
		public:
			EditorTool();

			~EditorTool() = default;

			/// <summary>
			/// Initializes the engine and all necessary subsystems with the specified parameters.
			/// </summary>
			/// <param name="a_hInstance">Handle to the program instance.</param>
			/// <param name="a_sName">Name of the program and window.</param>
			/// <returns>True if the engine initializes successfully, otherwise false.</returns>
			bool Initialize(HINSTANCE a_hInstance, const std::string& a_sName) override;

			/// <summary>
			/// Shuts down the engine and cleans up all subsystems.
			/// </summary>
			/// <returns>True if the destruction is successful, otherwise false.</returns>
			bool Destroy() override;

			/// <summary>
			/// Retrieves the editor settings.
			/// </summary>
			/// <returns>Reference to the editor settings.</returns>
			editor::EditorSettings& GetEditorSettings()
			{
				return m_EditorSettings;
			}

			/// <summary>
			/// Retrieves the asset database.
			/// </summary>
			/// <returns>Reference to the asset database.</returns>
			editor::AssetDatabase& GetAssetDatabase()
			{
				return m_AssetDatabase;
			}

			graphics::imgui::editor::EditorSelectable* GetSelectable()
			{
				return m_pSelectable;
			}

			void SetSelectable(graphics::imgui::editor::EditorSelectable* a_pSelectable, graphics::imgui::editor::InspectorView* a_pInspectorView)
			{
				std::lock_guard<std::mutex> lock(m_EditorMutex);

				if (m_pInspectorView)
				{
					delete m_pInspectorView;
					m_pInspectorView = nullptr;
				}
				m_pSelectable = a_pSelectable;
				m_pInspectorView = a_pInspectorView;
			}

			graphics::imgui::editor::InspectorView* GetInspectorView()
			{
				std::lock_guard<std::mutex> lock(m_EditorMutex);

				return m_pInspectorView;
			}
			std::mutex m_EditorMutex;
		private:
			editor::EditorSettings m_EditorSettings;
			editor::AssetDatabase m_AssetDatabase;
			graphics::imgui::editor::EditorSelectable* m_pSelectable = nullptr;
			graphics::imgui::editor::InspectorView* m_pInspectorView = nullptr;
		};
		extern inline EditorTool* EDITOR_TOOL = nullptr;
	}
}