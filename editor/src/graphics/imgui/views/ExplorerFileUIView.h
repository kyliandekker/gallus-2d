#pragma once

#include "graphics/imgui/views/ImGuiUIView.h"

#include <imgui/imgui.h>

#include <string>

namespace gallus
{
	namespace editor
	{
		class FileResource;
	}
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				class ExplorerFileUIView : public ImGuiUIView
				{
				public:
					ExplorerFileUIView(ImGuiWindow& a_Window, gallus::editor::FileResource& a_FileResource) : ImGuiUIView(a_Window), m_FileResource(a_FileResource)
					{}

					/// <summary>
					/// Retrieves the icon string for the resource.
					/// </summary>
					/// <returns>A reference to the icon string.</returns>
					const std::string& GetIcon() const;

					/// <summary>
					/// Sets the icon string for the resource.
					/// </summary>
					void SetIcon(const std::string& a_sIcon);

					/// <summary>
					/// Checks if the resource node is folded out in the UI.
					/// </summary>
					/// <returns>True if the node is folded out, otherwise false.</returns>
					bool IsFoldedOut() const;

					/// <summary>
					/// Sets the folded-out state of the resource node.
					/// </summary>
					/// <param name="a_FoldedOut">True to fold out, false to collapse.</param>
					void SetFoldedOut(bool a_bIsFoldedOut);

					/// <summary>
					/// Renders the file resource in the UI in the tree.
					/// </summary>
					/// <param name="a_bClicked">Indicates if the resource was clicked.</param>
					/// <param name="a_bRightClicked">Indicates if the resource was right-clicked.</param>
					/// <param name="a_bSelected">Indicates if the resource is selected.</param>
					/// <param name="a_iIndent">The amount of indents.</param>
					/// <param name="a_vInitialPos">The starting position for 0 indent.</param>
					/// <param name="a_bInContextMenu">Indicates if the resource is currently open in the context menu.</param>
					void RenderTree(bool& a_bClicked, bool& a_bRightClicked, bool a_bSelected, int a_iIndent, const ImVec2& a_vInitialPos, bool a_bInContextMenu);

					/// <summary>
					/// Checks if the resource has sub folders.
					/// Override to specify folder support for the resource.
					/// </summary>
					/// <returns>True if the resource has sub folders, otherwise false.</returns>
					bool HasFolders() const;

					void Render() override
					{}
				private:
					gallus::editor::FileResource& m_FileResource;
					std::string m_sIcon; /// The icon of the resource.
					bool m_bIsFoldedOut = false; /// Whether the node is folded out.
				};
			}
		}
	}
}