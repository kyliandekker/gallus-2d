#pragma once

#ifndef IMGUI_DISABLE

#include "core/System.h"

#include <imgui/imgui.h>

#include <wtypes.h>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <string>

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			class DX12BaseSystem;
			class CommandList;
			class Texture;
		}

		class FileResource;

		namespace imgui
		{
			class BaseWindow;

			//-----------------------------------------------------------------------------
			// ImGuiWindow
			//-----------------------------------------------------------------------------
			class ImGuiWindow : public core::System
			{
			public:
				ImGuiWindow();

				/// <summary>
				/// Initializes the imgui system.
				/// </summary>
				bool Initialize();

				/// <summary>
				/// Destroys all imgui resources.
				/// </summary>
				bool Destroy() override;
			private:
				/// <summary>
				/// Handles Windows messages for the editor's window.
				/// </summary>
				/// <param name="a_hWnd">Handle to the window.</param>
				/// <param name="a_iMsg">Message identifier.</param>
				/// <param name="a_wParam">Additional message information (WPARAM).</param>
				/// <param name="a_lParam">Additional message information (LPARAM).</param>
				/// <returns>The result of the message processing.</returns>
				LRESULT CALLBACK WndProcHandler(HWND a_hWnd, UINT a_iMsg, WPARAM a_wParam, LPARAM a_lParam);

				/// <summary>
				/// Creates the ImGui context for Win32.
				/// </summary>
				/// <returns>True if the context creation succeeds, otherwise false.</returns>
				bool CreateContextWin32();

				/// <summary>
				/// Creates the ImGui context for DirectX 12.
				/// </summary>
				/// <returns>True if the context creation succeeds, otherwise false.</returns>
				bool CreateContextDX12();

				/// <summary>
				/// Initializes the ImGui UI components.
				/// </summary>
				void CreateImGui();

				/// <summary>
				/// Called when the render target is created.
				/// </summary>
				void OnRenderTargetCreated(std::shared_ptr<dx12::CommandList> a_pCommandList);

				/// <summary>
				/// Called when the render target is created.
				/// </summary>
				/// <param name="a_vPos">Position of the window.</param>
				/// <param name="a_vSize">Size of the window.</param>
				void Resize(const glm::ivec2& a_vPos, const glm::ivec2& a_vSize);

				/// <summary>
				/// Called when the render target is cleaned.
				/// </summary>
				void OnRenderTargetCleaned();
			public:
				void Render(std::shared_ptr<dx12::CommandList> a_pCommandList);

				/// <summary>
				/// Retrieves the capital font.
				/// </summary>
				/// <returns>A pointer to the ImFont.</returns>
				ImFont* GetCapitalFont();

				/// <summary>
				/// Retrieves the big icon font.
				/// </summary>
				/// <returns>A pointer to the ImFont.</returns>
				ImFont* GetBigIconFont();

				/// <summary>
				/// Retrieves the icon font.
				/// </summary>
				/// <returns>A pointer to the ImFont.</returns>
				ImFont* GetIconFont();

				/// <summary>
				/// Retrieves the small font.
				/// </summary>
				/// <returns>A pointer to the ImFont.</returns>
				ImFont* GetSmallIconFont();

				/// <summary>
				/// Retrieves the bold font.
				/// </summary>
				/// <returns>A pointer to the ImFont.</returns>
				ImFont* GetBoldFont();

				/// <summary>
				/// Retrieves the font size.
				/// </summary>
				/// <returns>A vector containing the size of the font.</returns>
				float GetFontSize() const;

				/// <summary>
				/// Retrieves the capital font.
				/// </summary>
				/// <returns>A vector containing the frame padding settings.</returns>
				const ImVec2& GetFramePadding() const;

				/// <summary>
				/// Retrieves the capital font.
				/// </summary>
				/// <returns>A vector containing the window padding settings.</returns>
				const ImVec2& GetWindowPadding() const;

				/// <summary>
				/// Retrieves the capital font.
				/// </summary>
				/// <returns>A vector containing the header font size.</returns>
				const ImVec2& GetHeaderSize() const;

				void AddWindow(BaseWindow* window)
				{
					m_aWindows.push_back(window);
				}
			private:
				size_t m_SrvIndex = 0;

				ImFont* m_DefaultFont = nullptr;
				ImFont* m_Capital = nullptr;
				ImFont* m_CapitalIconFont = nullptr;
				ImFont* m_BoldFont = nullptr;
				ImFont* m_IconFont = nullptr;
				ImFont* m_IconFontM = nullptr;
				ImFont* m_SmallIconFont = nullptr;

				float m_FontSize = 15.0f; /// Default font size for ImGui.
				float m_IconFontSize = 15.0f; /// Default font size for ImGui.

				ImVec2 m_FramePadding = ImVec2(8, 8); /// Frame padding for ImGui elements.
				ImVec2 m_WindowPadding = ImVec2(8, 8); /// Window padding for ImGui elements.
				ImVec2 m_HeaderSize;

				std::vector<BaseWindow*> m_aWindows;

				friend dx12::DX12BaseSystem;
			};
		}
	}
}

#endif // IMGUI_DISABLE