#include <windows.h>
#include <Shlwapi.h>
#include <filesystem>
#include <ShellScalingApi.h>

#include "utils/file_abstractions.h"
#include "core/EditorTool.h"
#include "resource.h"
#include "graphics/imgui/windows/MainWindowDock.h"
#include "graphics/imgui/windows/ConsoleWindow.h"
#include "graphics/imgui/windows/HierarchyWindow.h"
#include "graphics/imgui/windows/SceneWindow.h"
#include "gameplay/systems/MeshSystem.h"

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
	std::string name = "Gallus 2D Engine";
	std::string saveDirPath = gallus::file::GetAppDataPath().generic_string() + "/tool";
	gallus::core::TOOL = new gallus::core::EditorTool();
	gallus::core::TOOL->SetSaveDirectory(saveDirPath);
	gallus::core::TOOL->GetDX12().GetImGuiWindow().AddWindow(new gallus::graphics::imgui::MainWindowDock(gallus::core::TOOL->GetDX12().GetImGuiWindow()));
	gallus::core::TOOL->GetDX12().GetImGuiWindow().AddWindow(new gallus::graphics::imgui::editor::ConsoleWindow(gallus::core::TOOL->GetDX12().GetImGuiWindow()));
	gallus::core::TOOL->GetDX12().GetImGuiWindow().AddWindow(new gallus::graphics::imgui::editor::HierarchyWindow(gallus::core::TOOL->GetDX12().GetImGuiWindow()));
	gallus::core::TOOL->GetDX12().GetImGuiWindow().AddWindow(new gallus::graphics::imgui::editor::SceneWindow(gallus::core::TOOL->GetDX12().GetImGuiWindow()));

	gallus::core::TOOL->Initialize(hInstance, name);

	auto entityId = gallus::core::TOOL->GetECS().CreateEntity("New Sprite");
	gallus::gameplay::MeshComponent* meshComp = reinterpret_cast<gallus::gameplay::MeshComponent*>(gallus::core::TOOL->GetECS().GetSystem<gallus::gameplay::MeshSystem>().CreateBaseComponent(entityId));
	meshComp->SetMesh(gallus::core::TOOL->GetResourceAtlas().GetDefaultMesh());
	meshComp->SetShader(gallus::core::TOOL->GetResourceAtlas().GetDefaultShader());
	meshComp->SetTexture(gallus::core::TOOL->GetResourceAtlas().GetDefaultTexture());

	// Load icons.
	HICON hIconLarge = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	HICON hIconSmall = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0); // 16x16
	SendMessage(gallus::core::TOOL->GetWindow().GetHWnd(), WM_SETICON, ICON_BIG, (LPARAM) hIconLarge);
	SendMessage(gallus::core::TOOL->GetWindow().GetHWnd(), WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);

	// Loop.
	gallus::core::TOOL->Loop();

	// Destroy the tool after loop ends.
	gallus::core::TOOL->Destroy();

	return 0;
}