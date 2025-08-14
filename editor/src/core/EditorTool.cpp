#include "EditorTool.h"

namespace gallus
{
	namespace core
	{
		//---------------------------------------------------------------------
		// EditorTool
		//---------------------------------------------------------------------
		EditorTool::EditorTool() : Tool::Tool(), m_EditorSettings("editorsettings.settings")
		{}

		//---------------------------------------------------------------------
		bool EditorTool::Initialize(HINSTANCE a_hInstance, const std::string& a_sName)
		{
			Tool::Initialize(a_hInstance, a_sName);

			m_AssetDatabase.Initialize();
			m_AssetDatabase.Scan();

			EDITOR_TOOL = this;

			return true;
		}

		//---------------------------------------------------------------------
		bool EditorTool::Destroy()
		{
			m_AssetDatabase.Destroy();

			return Tool::Destroy();
		}
	}
}