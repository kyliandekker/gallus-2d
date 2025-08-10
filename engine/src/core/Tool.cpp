#include "Tool.h"

#include "logger/Logger.h"
#include <glm/vec2.hpp>

#include "graphics/win32/Window.h"
#include "graphics/dx12/DX12BaseSystem.h"

namespace tool
{
	namespace core
	{
#define CATEGORY_TOOL "TOOL"

	Tool::~Tool()
	{
		delete m_pWindow;
		delete m_pDX12;
	}

	//-----------------------------------------------------------------------------
		// Tool
		//-----------------------------------------------------------------------------
		bool Tool::Initialize(HINSTANCE a_hInstance, const std::string& a_sName)
		{
			TOOL = this;

#ifdef _TEST
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif // _TEST
			// Initialize logger.
			// Logger is a global var unlike all the other systems. Not the prettiest but not too bad either.
			logger::LOGGER.Initialize(true);

			LOG(LOGSEVERITY_INFO, CATEGORY_TOOL, "Initializing tool.");

			// Create window if user did not provide derived class.
			if (!m_pWindow)
			{
				m_pWindow = new graphics::win32::Window();
			}

			// We initialize the window first and set the size and title after it has been created.
			m_pWindow->OnQuit() += std::bind(&Tool::Shutdown, this);
			m_pWindow->Initialize(true, a_hInstance);
			m_pWindow->SetTitle(a_sName);

			// Create dx12 if user did not provide derived class.
			if (!m_pDX12)
			{
				m_pDX12 = new graphics::dx12::DX12BaseSystem();
			}
			m_pDX12->Initialize(true, m_pWindow->GetHWnd(), m_pWindow->GetRealSize(), m_pWindow);

			System::Initialize();

			LOG(LOGSEVERITY_INFO, CATEGORY_TOOL, "Initialized tool.");

#ifdef _TEST
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			std::string microseconds = "Initialization took " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) + " microseconds.";
			std::string nanoseconds = "Initialization took " + std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) + " nanoseconds.";
			std::string milliseconds = "Initialization took " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + " milliseconds.";
			std::string seconds = "Initialization took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()) + " seconds.";

			TEST(microseconds.c_str());
			TEST(nanoseconds.c_str());
			TEST(milliseconds.c_str());
			TEST(seconds.c_str());
#endif // _TEST

			return true;
		}

		//-----------------------------------------------------------------------------------------------------
		bool Tool::Destroy()
		{
			LOG(LOGSEVERITY_INFO, CATEGORY_TOOL, "Destroying engine.");

			m_pDX12->Destroy();

			m_pWindow->Destroy();

			// Destroy the logger last so we can see possible error messages from other systems.
			logger::LOGGER.Destroy();

			return System::Destroy();
		}

		//-----------------------------------------------------------------------------------------------------
		void Tool::Shutdown()
		{
			m_bRunning.store(false);
		}

		//-----------------------------------------------------------------------------------------------------
		graphics::win32::Window& Tool::GetWindow()
		{
			return *m_pWindow;
		}

		graphics::dx12::DX12BaseSystem& Tool::GetDX12()
		{
			return *m_pDX12;
		}

		//-----------------------------------------------------------------------------------------------------
		void Tool::Loop()
		{
			while (m_bRunning.load())
			{
			}
		}
	}
}