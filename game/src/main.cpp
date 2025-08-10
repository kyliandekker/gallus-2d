#include <windows.h>
#include <Shlwapi.h>
#include <filesystem>
#include <ShellScalingApi.h>

#include "utils/file_abstractions.h"
#include "core/Tool.h"
#include "graphics/dx12/DX12System3DGame.h"
#include "resource.h"
#include "graphics/win32/Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	fs::path path(buffer);
	path = path.parent_path();
	fs::current_path(path);
	SetCurrentDirectoryW(path.c_str());

	// Initialize systems.
	std::string name = "Gallus";
	std::string saveDirPath = tool::file::GetAppDataPath().generic_string() + "/tool";
	tool::core::TOOL = new tool::core::Tool();
	tool::core::TOOL->SetSaveDirectory(saveDirPath);
	tool::core::TOOL->SetDX12System<tool::graphics::dx12::DX12BaseSystem>();

	tool::core::TOOL->Initialize(hInstance, name);

	// Load icons.
	HICON hIconLarge = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	HICON hIconSmall = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0); // 16x16
	SendMessage(tool::core::TOOL->GetWindow().GetHWnd(), WM_SETICON, ICON_BIG, (LPARAM) hIconLarge);
	SendMessage(tool::core::TOOL->GetWindow().GetHWnd(), WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);

	// Loop.
	tool::core::TOOL->Loop();

	// Destroy the tool after loop ends.
	tool::core::TOOL->Destroy();

	return 0;
}