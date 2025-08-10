#pragma once

#include <imgui/imgui.h>

namespace tool
{
	namespace graphics
	{
		namespace imgui
		{
			void SetBessDark(ImGuiStyle& style, ImVec4* colors)
			{
				colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);

				colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);  // #131318
				colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f); // #131318

				colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);

				colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
				colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.40f, 1.00f);
				colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);

				colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.32f, 0.40f, 1.00f);
				colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.38f, 0.50f, 1.00f);

				colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
				colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.27f, 1.00f);
				colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);

				colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
				colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.50f, 1.00f);
				colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.38f, 1.00f);
				colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.13f, 0.17f, 1.00f);
				colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);

				colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
				colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
				colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);

				colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.50f);
				colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

				colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.70f, 1.00f, 1.00f);
				colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.70f, 1.00f, 1.00f);
				colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.80f, 1.00f, 1.00f);
				colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.70f, 1.00f, 0.50f);
				colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.80f, 1.00f, 0.75f);
				colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.90f, 1.00f, 1.00f);

				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.50f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.55f, 1.00f);
			}

			void SetOwnDark(ImGuiStyle& style, ImVec4* colors)
			{
				ImVec4 accentColor = ImVec4(0.42f, 0.34f, 0.83f, 1.00f);
				ImVec4 accentColorText = ImVec4(0.5f, 0.44f, 0.93f, 1.00f);

				colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
				colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
				colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_HeaderHovered] = ImVec4(accentColor.x, accentColor.y, accentColor.z, 0.5f);
				//colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_HeaderActive] = accentColor;
				colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_TabActive] = accentColor;
				colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_DragDropTarget] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TextColorAccent] = accentColorText;
#ifndef _RENDER_TEX
				float transparency = 0.4;
				ImGui::GetStyle().Colors[ImGuiCol_FrameBg].w = transparency;
				ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = transparency;
				ImGui::GetStyle().Colors[ImGuiCol_ChildBg].w = transparency;
#endif // _RENDER_TEX

				style.WindowBorderSize = 1;
				style.WindowRounding = 0;
				style.WindowPadding = ImVec2(0, 0);
				style.ItemInnerSpacing = ImVec2(0, 0);
				style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
				style.IndentSpacing = 25;
				style.ScrollbarSize = 15;
				style.GrabMinSize = 10;
				style.TabRounding = 8;
				style.TabBorderSize = 1;
				style.PopupBorderSize = 1;
				style.FrameBorderSize = 1;
				style.ChildBorderSize = 1;
				style.ChildRounding = 0;
				style.FrameRounding = 0;
				style.PopupRounding = 0;
				style.ScrollbarRounding = 8;
				style.GrabRounding = 8;
				style.LogSliderDeadzone = 4;
				style.FramePadding = ImVec2(0, 0);
			}

			void SetSimplifiedDark(ImGuiStyle& style, ImVec4* colors)
			{
				// Base Colors
				ImVec4 bgColor = ImVec4(0.10f, 0.105f, 0.11f, 1.00f);
				ImVec4 lightBgColor = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
				ImVec4 panelColor = ImVec4(0.17f, 0.18f, 0.19f, 1.00f);
				ImVec4 panelHoverColor = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
				ImVec4 panelActiveColor = ImVec4(0.23f, 0.26f, 0.29f, 1.00f);
				ImVec4 textColor = ImVec4(0.86f, 0.87f, 0.88f, 1.00f);
				ImVec4 textDisabledColor = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
				ImVec4 borderColor = ImVec4(0.14f, 0.16f, 0.18f, 1.00f);

				// Text
				colors[ImGuiCol_Text] = textColor;
				colors[ImGuiCol_TextDisabled] = textDisabledColor;

				// Windows
				colors[ImGuiCol_WindowBg] = bgColor;
				colors[ImGuiCol_ChildBg] = bgColor;
				colors[ImGuiCol_PopupBg] = bgColor;
				colors[ImGuiCol_Border] = borderColor;
				colors[ImGuiCol_BorderShadow] = borderColor;

				// Headers
				colors[ImGuiCol_Header] = panelColor;
				colors[ImGuiCol_HeaderHovered] = panelHoverColor;
				colors[ImGuiCol_HeaderActive] = panelActiveColor;

				// Buttons
				colors[ImGuiCol_Button] = panelColor;
				colors[ImGuiCol_ButtonHovered] = panelHoverColor;
				colors[ImGuiCol_ButtonActive] = panelActiveColor;

				// Frame BG
				colors[ImGuiCol_FrameBg] = lightBgColor;
				colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
				colors[ImGuiCol_FrameBgActive] = panelActiveColor;

				// Tabs
				colors[ImGuiCol_Tab] = panelColor;
				colors[ImGuiCol_TabHovered] = panelHoverColor;
				colors[ImGuiCol_TabActive] = panelActiveColor;
				colors[ImGuiCol_TabUnfocused] = panelColor;
				colors[ImGuiCol_TabUnfocusedActive] = panelHoverColor;

				// Title
				colors[ImGuiCol_TitleBg] = bgColor;
				colors[ImGuiCol_TitleBgActive] = bgColor;
				colors[ImGuiCol_TitleBgCollapsed] = bgColor;

				// Scrollbar
				colors[ImGuiCol_ScrollbarBg] = bgColor;
				colors[ImGuiCol_ScrollbarGrab] = panelColor;
				colors[ImGuiCol_ScrollbarGrabHovered] = panelHoverColor;
				colors[ImGuiCol_ScrollbarGrabActive] = panelActiveColor;

				// Checkmark
				colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

				// Slider
				colors[ImGuiCol_SliderGrab] = panelHoverColor;
				colors[ImGuiCol_SliderGrabActive] = panelActiveColor;

				// Resize Grip
				colors[ImGuiCol_ResizeGrip] = panelColor;
				colors[ImGuiCol_ResizeGripHovered] = panelHoverColor;
				colors[ImGuiCol_ResizeGripActive] = panelActiveColor;

				// Separator
				colors[ImGuiCol_Separator] = borderColor;
				colors[ImGuiCol_SeparatorHovered] = panelHoverColor;
				colors[ImGuiCol_SeparatorActive] = panelActiveColor;

				// Plot
				colors[ImGuiCol_PlotLines] = textColor;
				colors[ImGuiCol_PlotLinesHovered] = panelActiveColor;
				colors[ImGuiCol_PlotHistogram] = textColor;
				colors[ImGuiCol_PlotHistogramHovered] = panelActiveColor;

				// Text Selected BG
				colors[ImGuiCol_TextSelectedBg] = panelActiveColor;

				// Modal Window Dim Bg
				colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.105f, 0.11f, 0.5f);

				// Tables
				colors[ImGuiCol_TableHeaderBg] = panelColor;
				colors[ImGuiCol_TableBorderStrong] = borderColor;
				colors[ImGuiCol_TableBorderLight] = borderColor;
				colors[ImGuiCol_TableRowBg] = bgColor;
				colors[ImGuiCol_TableRowBgAlt] = lightBgColor;

				// Styles
				style.FrameBorderSize = 1.0f;
				style.FrameRounding = 2.0f;
				style.WindowBorderSize = 1.0f;
				style.PopupBorderSize = 1.0f;
				style.ScrollbarSize = 12.0f;
				style.ScrollbarRounding = 2.0f;
				style.GrabMinSize = 7.0f;
				style.GrabRounding = 2.0f;
				style.TabBorderSize = 1.0f;
				style.TabRounding = 2.0f;

				// Reduced Padding and Spacing
				style.WindowPadding = ImVec2(5.0f, 5.0f);
				style.FramePadding = ImVec2(4.0f, 3.0f);
				style.ItemSpacing = ImVec2(6.0f, 4.0f);
				style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
			}
		}
	}
}