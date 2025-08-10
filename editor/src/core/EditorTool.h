#pragma once

#include "core/Tool.h"
#include "editor/EditorSettings.h"

namespace tool
{
	namespace core
	{
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
		private:
			editor::EditorSettings m_EditorSettings;
		};
		extern inline EditorTool* EDITOR_TOOL = nullptr;
	}
}