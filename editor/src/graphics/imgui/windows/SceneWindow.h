#pragma once

#ifndef IMGUI_DISABLE
#ifdef _EDITOR

#include "graphics/imgui/windows/BaseWindow.h"

#include <vector>

#include "gameplay/EntityID.h"
#include "graphics/imgui/views/DataTypes/StringTextInput.h"
#include "graphics/imgui/views/EntityUIView.h"

namespace gallus
{
	namespace graphics
	{
		namespace imgui
		{
			class ImGuiWindow;

			namespace editor
			{
				/// <summary>
				/// A window that displays the scene.
				/// </summary>
				//-----------------------------------------------------------------------------
				// SceneWindow
				//-----------------------------------------------------------------------------
				class SceneWindow : public BaseWindow
				{
				public:
					/// <summary>
					/// Constructs a console window.
					/// </summary>
					/// <param name="a_Window">The ImGui window for rendering the view.</param>
					SceneWindow(ImGuiWindow& a_Window);

					/// <summary>
					/// Cleans up and destroys the console window.
					/// </summary>
					~SceneWindow();

					/// <summary>
					/// Renders the console window.
					/// </summary>
					void Render() override;
				};
			}
		}
	}
}

#endif // _EDITOR
#endif // IMGUI_DISABLE