#ifndef IMGUI_DISABLE

#include "ImGuiWindow.h"

#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>
#include <imgui/imgui_internal.h>

#include "core/Tool.h"
#include "logger/Logger.h"
#include "font_arial.h"
#include "font_icon.h"

#include "graphics/win32/Window.h"
#include "graphics/dx12/DX12BaseSystem.h"
#include "graphics/dx12/CommandQueue.h"
#include "graphics/dx12/CommandList.h"
#include "windows/BaseWindow.h"
#include "themes.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace tool
{
	namespace graphics
	{
		namespace imgui
		{
			//-----------------------------------------------------------------------------
			// ImGuiWindow
			//-----------------------------------------------------------------------------
			ImGuiWindow::ImGuiWindow() : core::System()
			{}

			std::string m_IniPath;
			//-----------------------------------------------------------------------------------------------------
			bool ImGuiWindow::Initialize()
			{
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();

				m_IniPath = fs::path(core::TOOL->GetSaveDirectory().generic_string() + "/imgui.ini").generic_string();
				ImGuiIO& io = ImGui::GetIO();
				io.IniFilename = m_IniPath.c_str();

				if (!CreateContextWin32() || !CreateContextDX12())
				{
					return false;
				}

				CreateImGui();

				LOG(LOGSEVERITY_SUCCESS, LOG_CATEGORY_EDITOR, "Initialized ImGui.");

				return System::Initialize();
			}

			//-----------------------------------------------------------------------------------------------------
			bool ImGuiWindow::Destroy()
			{
				ImGui_ImplDX12_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();

				LOG(LOGSEVERITY_SUCCESS, LOG_CATEGORY_EDITOR, "Destroyed ImGui.");

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool ImGuiWindow::CreateContextWin32()
			{
				if (!ImGui_ImplWin32_Init(core::TOOL->GetWindow().GetHWnd()))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_EDITOR, "Failed creating WIN32 context for ImGui.");
					return false;
				}

				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_EDITOR, "Created WIN32 context for ImGui.");
				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool ImGuiWindow::CreateContextDX12()
			{
				dx12::DX12BaseSystem& dx12window = core::TOOL->GetDX12();
				m_SrvIndex = dx12window.GetSRV().Allocate();

				if (!ImGui_ImplDX12_Init(dx12window.GetDevice().Get(), dx12::g_iBufferCount,
					DXGI_FORMAT_R8G8B8A8_UNORM, dx12window.GetSRV().GetHeap().Get(),
					dx12window.GetSRV().GetCPUHandle(m_SrvIndex),
					dx12window.GetSRV().GetGPUHandle(m_SrvIndex)))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_EDITOR, "Failed creating DX12 context for ImGui.");
					return false;
				}

				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_EDITOR, "Created DX12 context for ImGui.");
				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			void ImGuiWindow::CreateImGui()
			{
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
				io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;

				(void) io;

				// On Windows 8.1+:
				UINT dpi = GetDpiForWindow(tool::core::TOOL->GetWindow().GetHWnd()); // returns DPI, e.g., 96, 120, 144
				float dp = dpi / 96.0f; // 96 is the default DPI (100%)

				m_FontSize *= dp;
				m_IconFontSize *= dp;

				m_HeaderSize = ImVec2(m_FontSize * 3.0f, m_FontSize * 3.0f);

				// setup Dear ImGui style
				ImGui::StyleColorsDark();

				constexpr ImWchar icons_ranges_b[] = { font::FONT_START, font::FONT_END, 0 };

				ImFontConfig font_config_capital;
				font_config_capital.FontDataOwnedByAtlas = false;
				m_Capital = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize * 2, &font_config_capital);

				ImFontConfig font_config_icon_capital;
				font_config_icon_capital.FontDataOwnedByAtlas = false;
				m_CapitalIconFont = io.Fonts->AddFontFromMemoryTTF(&font::ICON, sizeof(font::ICON), m_HeaderSize.x / 2, &font_config_icon_capital, icons_ranges_b);

				ImFontConfig icons_config_b;
				icons_config_b.FontDataOwnedByAtlas = false;
				m_IconFont = io.Fonts->AddFontFromMemoryTTF(&font::ICON, sizeof(font::ICON), m_IconFontSize, &icons_config_b, icons_ranges_b);

				ImFontConfig small_icons_config_b;
				small_icons_config_b.FontDataOwnedByAtlas = false;
				m_SmallIconFont = io.Fonts->AddFontFromMemoryTTF(&font::ICON, sizeof(font::ICON), m_FontSize, &small_icons_config_b, icons_ranges_b);

				ImFontConfig font_config_bold;
				font_config_bold.FontDataOwnedByAtlas = false;
				m_BoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), m_FontSize, &font_config_bold);

				ImFontConfig font_config_default;
				font_config_default.FontDataOwnedByAtlas = false;
				m_DefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize, &font_config_default);

				ImFontConfig icons_config_m;
				icons_config_m.MergeMode = true;
				icons_config_m.PixelSnapH = true;
				icons_config_m.FontDataOwnedByAtlas = false;
				m_IconFontM = io.Fonts->AddFontFromMemoryTTF(&font::ICON, sizeof(font::ICON), m_FontSize, &icons_config_m, icons_ranges_b);

				io.Fonts->Build();

				ImGui::StyleColorsDark();

				ImGuiStyle& style = ImGui::GetStyle();
				ImVec4* colors = style.Colors;

				SetOwnDark(style, colors);
			}

			//-----------------------------------------------------------------------------------------------------
			void ImGuiWindow::OnRenderTargetCreated(std::shared_ptr<dx12::CommandList> a_pCommandList)
			{
				if (!m_bRunning.load())
				{
					return;
				}

				ImGui_ImplDX12_CreateDeviceObjects();
			}

			//-----------------------------------------------------------------------------------------------------
			void ImGuiWindow::Resize(const glm::ivec2& a_vPos, const glm::ivec2& a_vSize)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize.x = static_cast<float>(a_vSize.x);
				io.DisplaySize.y = static_cast<float>(a_vSize.y);
			}

			//-----------------------------------------------------------------------------------------------------
			void ImGuiWindow::OnRenderTargetCleaned()
			{
				if (!m_bRunning.load())
				{
					return;
				}

				ImGui_ImplDX12_InvalidateDeviceObjects();
			}

			//-----------------------------------------------------------------------------------------------------
			void ImGuiWindow::Render(std::shared_ptr<dx12::CommandList> a_pCommandList)
			{
				if (!m_bRunning.load())
				{
					return;
				}

				ImGui_ImplDX12_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				ImGui::PushFont(m_IconFontM);

				ImGuiIO& io = ImGui::GetIO();

				if (!m_aWindows.empty())
				{
					m_aWindows[0]->SetSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
					for (BaseWindow* window : m_aWindows)
					{
						window->Update();
					}
				}

				ImGui::PopFont();

				ImGui::EndFrame();
				ImGui::Render();

				ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), a_pCommandList->GetCommandList().Get());
			}

			//-----------------------------------------------------------------------------------------------------
			LRESULT ImGuiWindow::WndProcHandler(HWND a_hWnd, UINT a_iMsg, WPARAM a_wParam, LPARAM a_lParam)
			{
				if (!m_bRunning.load())
				{
					return a_iMsg;
				}

				return ImGui_ImplWin32_WndProcHandler(a_hWnd, a_iMsg, a_wParam, a_lParam);
			}

			//-----------------------------------------------------------------------------------------------------
			ImFont* ImGuiWindow::GetCapitalFont()
			{
				return m_Capital;
			}

			//-----------------------------------------------------------------------------------------------------
			ImFont* ImGuiWindow::GetBigIconFont()
			{
				return m_CapitalIconFont;
			}

			//-----------------------------------------------------------------------------------------------------
			ImFont* ImGuiWindow::GetIconFont()
			{
				return m_IconFont;
			}

			//-----------------------------------------------------------------------------------------------------
			ImFont* ImGuiWindow::GetSmallIconFont()
			{
				return m_SmallIconFont;
			}

			//-----------------------------------------------------------------------------------------------------
			ImFont* ImGuiWindow::GetBoldFont()
			{
				return m_BoldFont;
			}

			//-----------------------------------------------------------------------------------------------------
			float ImGuiWindow::GetFontSize() const
			{
				return m_FontSize;
			}

			//-----------------------------------------------------------------------------------------------------
			const ImVec2& ImGuiWindow::GetFramePadding() const
			{
				return m_FramePadding;
			}

			//-----------------------------------------------------------------------------------------------------
			const ImVec2& ImGuiWindow::GetWindowPadding() const
			{
				return m_WindowPadding;
			}

			//-----------------------------------------------------------------------------------------------------
			const ImVec2& ImGuiWindow::GetHeaderSize() const
			{
				return m_HeaderSize;
			}
		}
	}
}

#endif // IMGUI_DISABLE