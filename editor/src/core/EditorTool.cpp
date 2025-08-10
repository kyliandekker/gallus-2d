#include "EditorTool.h"

namespace tool
{
    namespace core
    {
        EditorTool::EditorTool() : Tool::Tool(), m_EditorSettings("editorsettings.settings")
        { }
    //-----------------------------------------------------------------------------
        // EditorTool
        //-----------------------------------------------------------------------------
        bool EditorTool::Initialize(HINSTANCE a_hInstance, const std::string& a_sName)
        {
            Tool::Initialize(a_hInstance, a_sName);

            EDITOR_TOOL = this;

            return true;
        }

        //-----------------------------------------------------------------------------------------------------
        bool EditorTool::Destroy()
        {
            return Tool::Destroy();
        }
    }
}