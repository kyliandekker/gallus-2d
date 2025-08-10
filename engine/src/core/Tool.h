#include "System.h"

#include <wtypes.h>

#include "utils/file_abstractions.h"

namespace tool
{
	namespace graphics
	{
		namespace win32
		{
			class Window;
		}
		namespace dx12
		{
			class DX12BaseSystem;
		}
	}
	namespace core
	{
		//-----------------------------------------------------------------------------
		// Tool
		//-----------------------------------------------------------------------------
		/// <summary>
		/// Main tool that manages all systems in the program, like initialization, startup, update and shutdown.
		/// </summary>
		class Tool : public System
		{
		public:
			Tool() = default;

			virtual ~Tool();

			/// <summary>
			/// Initializes the engine and all necessary subsystems with the specified parameters.
			/// </summary>
			/// <param name="a_hInstance">Handle to the program instance.</param>
			/// <param name="a_sName">Name of the program and window.</param>
			/// <returns>True if the engine initializes successfully, otherwise false.</returns>
			virtual bool Initialize(HINSTANCE a_hInstance, const std::string& a_sName);

			/// <summary>
			/// Shuts down the engine and cleans up all subsystems.
			/// </summary>
			/// <returns>True if the destruction is successful, otherwise false.</returns>
			virtual bool Destroy() override;

			/// <summary>
			/// Shuts down the engine.
			/// </summary>
			void Shutdown();

			/// <summary>
			/// Retrieves the window.
			/// </summary>
			/// <returns>Reference to the window.</returns>
			graphics::win32::Window& GetWindow();

			/// <summary>
			/// Retrieves the dx12 system.
			/// </summary>
			/// <returns>Reference to the dx12 system.</returns>
			graphics::dx12::DX12BaseSystem& GetDX12();

			/// <summary>
			/// Sets the dx12 system.
			/// </summary>
			template<typename T>
			void SetDX12System()
			{
				m_pDX12 = new T();
			}

			/// <summary>
			/// Retrieves the save directory of the program.
			/// </summary>
			/// <returns>Path to the save directory.</returns>
			const std::filesystem::path& GetSaveDirectory()
			{
				return m_sSaveDirectory;
			}

			/// <summary>
			/// Sets the save directory of the program.
			/// </summary>
			/// <param name="a_sSaveDirectory">Save directory.</param>
			void SetSaveDirectory(const std::filesystem::path& a_sSaveDirectory)
			{
				m_sSaveDirectory = a_sSaveDirectory;
				file::CreateDirectory(a_sSaveDirectory);
			}

			/// <summary>
			/// Main loop of the program.
			/// </summary>
			void Loop();
		private:
			graphics::win32::Window* m_pWindow = nullptr;
			graphics::dx12::DX12BaseSystem* m_pDX12 = nullptr;

			std::filesystem::path m_sSaveDirectory;
		};
		extern inline Tool* TOOL = nullptr;
	}
}