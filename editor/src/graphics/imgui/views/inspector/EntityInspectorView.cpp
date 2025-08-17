#include "EntityInspectorView.h"

#include "core/Tool.h"
#include "graphics/imgui/font_icon.h"
#include "graphics/imgui/views/HierarchyEntityUIView.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			namespace editor
			{
				EntityInspectorView::EntityInspectorView(ImGuiWindow& a_Window, HierarchyEntityUIView& a_HierarchyEntityUIView) : InspectorView(a_Window), m_HierarchyEntityUIView(a_HierarchyEntityUIView)
				{
					m_bShowRename = true;
					m_bShowDelete = true;

					m_Entity = core::TOOL->GetECS().GetEntity(m_HierarchyEntityUIView.GetEntityID());
				}

				void EntityInspectorView::OnRename(const std::string& a_sName)
				{
					if (m_Entity)
					{
						m_Entity->SetName(a_sName);
					}
				}

				void EntityInspectorView::OnDelete()
				{
					core::TOOL->GetECS().DeleteEntity(m_HierarchyEntityUIView.GetEntityID());
				}

				std::string EntityInspectorView::GetName() const
				{
					return m_Entity ? m_Entity->GetName() : "";
				}


				std::string EntityInspectorView::GetIcon() const
				{
					return m_HierarchyEntityUIView.GetIcon();
				}

				void EntityInspectorView::Render()
				{
					ImGui::Text("Test");
				}
			}
		}
	}
}