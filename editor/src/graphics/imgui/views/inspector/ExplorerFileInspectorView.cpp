#include "ExplorerFileInspectorView.h"

#include "graphics/imgui/views/ExplorerFileUIView.h"
#include "editor/FileResource.h"
#include "utils/file_abstractions.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				ExplorerFileInspectorView::ExplorerFileInspectorView(ImGuiWindow& a_Window, ExplorerFileUIView& a_ExplorerFileUIView) : InspectorView(a_Window), m_ExplorerFileUIView(a_ExplorerFileUIView)
				{
					m_bShowRename = true;
					m_bShowDelete = true;
					m_bShowShowInExplorer = true;
				}

				void ExplorerFileInspectorView::OnRename(const std::string& a_sName)
				{
					m_ExplorerFileUIView.GetFileResource().Rename(a_sName);
				}

				void ExplorerFileInspectorView::OnDelete()
				{
					m_ExplorerFileUIView.GetFileResource().Delete();
				}

				void ExplorerFileInspectorView::OnShowInExplorer()
				{
					file::OpenInExplorer(m_ExplorerFileUIView.GetFileResource().GetPath().parent_path().lexically_normal());
				}

				std::string ExplorerFileInspectorView::GetName() const
				{
					return m_ExplorerFileUIView.GetFileResource().GetPath().filename().generic_string();
				}


				std::string ExplorerFileInspectorView::GetIcon() const
				{
					return m_ExplorerFileUIView.GetIcon();
				}

				void ExplorerFileInspectorView::Render()
				{
					ImGui::Text("Test");
				}
			}
		}
	}
}