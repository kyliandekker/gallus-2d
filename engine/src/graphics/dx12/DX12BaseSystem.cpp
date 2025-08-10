#include "DX12BaseSystem.h"

#include "logger/Logger.h"
#include "graphics/win32/Window.h"
#include "CommandQueue.h"
#include "CommandList.h"

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			//-----------------------------------------------------------------------------
			// DX12BaseSystem
			//-----------------------------------------------------------------------------
			bool DX12BaseSystem::Initialize(bool a_bWait, HWND a_hWnd, const glm::ivec2& a_vSize, win32::Window* a_pWindow)
			{
				m_vSize = a_vSize;
				m_hWnd = a_hWnd;
				m_pWindow = a_pWindow;

				LOG(LOGSEVERITY_INFO, LOG_CATEGORY_DX12, "Initializing dx12 system.");
				return ThreadedSystem::Initialize(a_bWait);
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::Destroy()
			{
				LOG(LOGSEVERITY_INFO, LOG_CATEGORY_DX12, "Destroying dx12 system.");
				return ThreadedSystem::Destroy();
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::InitThreadWorker()
			{
#if _DEBUGs
				// Always enable the debug layer before doing anything DX12 related
				// so all possible errors generated while creating DX12 objects
				// are caught by the debug layer.
				Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
				if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed retrieving debug interface.");
					return false;
				}
				debugInterface->EnableDebugLayer();

				Microsoft::WRL::ComPtr<ID3D12Debug1> debugController1 = nullptr;
				if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController1))))
				{
					debugController1->SetEnableGPUBasedValidation(TRUE);
				}
#endif // _DEBUGs
				// Get the adapter.
				if (!GetAdapter(false))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed retrieving adapter.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Retrieved adapter.");
#endif // LOG_DX!2

				// Create the device.
				if (!CreateDevice())
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating device.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Created device.");
#endif // LOG_DX!2

				ID3D12InfoQueue* infoQueue = nullptr;
				if (SUCCEEDED(m_pDevice->QueryInterface(IID_PPV_ARGS(&infoQueue))))
				{
					infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
					infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
					infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE);

					// Optionally filter out less important messages
					D3D12_MESSAGE_ID denyIds[] = {
						D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE,
						D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE
					};

					D3D12_INFO_QUEUE_FILTER filter = {};
					filter.DenyList.NumIDs = _countof(denyIds);
					filter.DenyList.pIDList = denyIds;
					infoQueue->PushStorageFilter(&filter);

					infoQueue->Release();
				}

				// Create the command queues.
				if (!CreateCommandQueues())
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating command queues.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Created command queues.");
#endif // LOG_DX!2

				m_bIsTearingSupported = CheckTearingSupport();

				if (!DirectX::XMVerifyCPUSupport())
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed verifying DirectX Math support.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Verified DirectX Math support.");
#endif // LOG_DX!2

				// Create the swap chain.
				if (!CreateSwapChain())
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating swap chain.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Created swap chain.");
#endif // LOG_DX!2

				// Create views.
				if (!CreateViews())
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating views.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Created views.");
#endif // LOG_DX!2

				std::shared_ptr<CommandQueue> dCommandQueue = GetCommandQueue();
				std::shared_ptr<CommandList> dCommandList = dCommandQueue->GetCommandList();

				// Used for creating stuff like root signature, etc.
				if (!BeforeInitialize(dCommandQueue, dCommandList))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed before initialization could succeed.");
					return false;
				}

				m_eOnInitialize(*this);
#ifndef IMGUI_DISABLE
				m_ImGuiWindow.Initialize();
#endif // IMGUI_DISABLE
				if (!AfterInitialize(dCommandQueue, dCommandList))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed after initialization.");
					return false;
				}

				UpdateRenderTargetViews();
#ifndef IMGUI_DISABLE
				m_ImGuiWindow.OnRenderTargetCreated(dCommandList);
#endif // IMGUI_DISABLE
				auto fenceValue = dCommandQueue->ExecuteCommandList(dCommandList);
				dCommandQueue->WaitForFenceValue(fenceValue);

				LOG(LOGSEVERITY_SUCCESS, LOG_CATEGORY_DX12, "Initialized dx12 system.");

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::GetAdapter(bool a_bUseWarp)
			{
				Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
				UINT createFactoryFlags = 0;
#if defined(_DEBUGs)
				createFactoryFlags = DXGI_CREATE_FACTORY_DEBUGs;
#endif // _DEBUGs

				if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating DXGI Factory.");
					return false;
				}

				Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter1;

				if (a_bUseWarp)
				{
					if (FAILED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1))))
					{
						LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating warp adapter.");
						return false;
					}
					if (FAILED(dxgiAdapter1.As(&m_pAdapter)))
					{
						LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed casting adapter.");
						return false;
					}
				}
				else
				{
					SIZE_T maxDedicatedVideoMemory = 0;
					for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
					{
						DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
						dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

						// Check to see if the adapter can create a D3D12 m_pDevice without actually 
						// creating it. The adapter with the largest dedicated video memory
						// is favored.
						if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
							SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
							D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
							dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
						{
							maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
							if (FAILED(dxgiAdapter1.As(&m_pAdapter)))
							{
								LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed casting adapter.");
								return false;
							}
						}
					}
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Created adapter.");
#endif // LOG_DX!2

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::CreateDevice()
			{
				if (FAILED(D3D12CreateDevice(m_pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating m_pDevice.");
					return false;
				}
				//    NAME_D3D12_OBJECT(d3d12Device2);

				// Enable debug messages in debug mode.
#ifdef _DEBUGs
				Microsoft::WRL::ComPtr<ID3D12InfoQueue> pInfoQueue;
				if (SUCCEEDED(m_pDevice.As(&pInfoQueue)))
				{
					pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
					pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
					pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

					// Suppress whole categories of messages
					//D3D12_MESSAGE_CATEGORY Categories[] = {};

					// Suppress messages based on their severity level
					D3D12_MESSAGE_SEVERITY Severities[] =
					{
						D3D12_MESSAGE_SEVERITY_INFO
					};

					// Suppress individual messages by their ID
					D3D12_MESSAGE_ID DenyIds[] = {
						D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
						D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
						D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
					};

					D3D12_INFO_QUEUE_FILTER NewFilter = {};
					//NewFilter.DenyList.NumCategories = _countof(Categories);
					//NewFilter.DenyList.pCategoryList = Categories;
					NewFilter.DenyList.NumSeverities = _countof(Severities);
					NewFilter.DenyList.pSeverityList = Severities;
					NewFilter.DenyList.NumIDs = _countof(DenyIds);
					NewFilter.DenyList.pIDList = DenyIds;

					if (FAILED(pInfoQueue->PushStorageFilter(&NewFilter)))
					{
						LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed pushing storage filter.");
						return false;
					}
				}
#endif // _DEBUGs

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::CheckTearingSupport()
			{
				BOOL allowTearing = FALSE;

				// Rather than create the DXGI 1.5 factory interface directly, we create the
				// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
				// graphics debugging tools which will not support the 1.5 factory interface 
				// until a future update.
				Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
				if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
				{
					Microsoft::WRL::ComPtr<IDXGIFactory5> factory5;
					if (SUCCEEDED(factory4.As(&factory5)))
					{
						factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
							&allowTearing, sizeof(allowTearing));
					}
				}

				return allowTearing == TRUE;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::CreateSwapChain()
			{
				Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory4;
				UINT createFactoryFlags = 0;
#ifdef _DEBUGs
				createFactoryFlags = DXGI_CREATE_FACTORY_DEBUGs;
#endif // _DEBUGs

				if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating DXGI factory.");
					return false;
				}
#if LOG_DX12 == 1
				LOG(LOGSEVERITY_INFO_SUCCESS, LOG_CATEGORY_DX12, "Created DXGI factory.");
#endif // LOG_DX!2

				DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
				swapChainDesc.Width = m_vSize.x;
				swapChainDesc.Height = m_vSize.y;
				swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swapChainDesc.Stereo = FALSE;
				swapChainDesc.SampleDesc = { 1, 0 };
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = g_iBufferCount;
				swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
				swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
				// It is recommended to always allow tearing if tearing support is available.
				swapChainDesc.Flags = m_bIsTearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
				ID3D12CommandQueue* pCommandQueue = GetCommandQueue()->GetCommandQueue().Get();

				Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
				if (FAILED(dxgiFactory4->CreateSwapChainForHwnd(
					pCommandQueue,
					m_hWnd,
					&swapChainDesc,
					nullptr,
					nullptr,
					&swapChain1)))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating swap chain.");
					return false;
				}

				// Disable the Alt+Enter full screen toggle feature. Switching to full screen
				// will be handled manually.
				if (FAILED(dxgiFactory4->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER)))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed associating window.");
					return false;
				}

				if (FAILED(swapChain1.As(&m_pSwapChain)))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed casting swap chain.");
					return false;
				}

				m_iCurrentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

				return m_pSwapChain != nullptr;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::CreateRTV()
			{
				size_t numBuffers = g_iBufferCount;

				D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
				rtvHeapDesc.NumDescriptors = static_cast<UINT>(numBuffers);
				rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				m_RTV = HeapAllocation(rtvHeapDesc);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::CreateSRV()
			{
				D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
				srvHeapDesc.NumDescriptors = 100;  // Adjust based on how many textures you need
				srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // Important for binding!
				m_SRV = HeapAllocation(srvHeapDesc);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::Finalize()
			{
				std::lock_guard<std::mutex> lock(m_RenderMutex);
#ifndef IMGUI_DISABLE
				m_ImGuiWindow.Destroy();
#endif // IMGUI_DISABLE

				Flush();

				LOG(LOGSEVERITY_SUCCESS, LOG_CATEGORY_DX12, "Destroyed dx12 system.");
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::Flush()
			{
				m_pDirectCommandQueue->Flush();
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::UpdateRenderTargetViews()
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTV.GetCPUDescriptorHandleForHeapStart());

				for (int i = 0; i < g_iBufferCount; ++i)
				{
					Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
					if (FAILED(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer))))
					{
						LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed retrieving buffer.");
						return;
					}

					m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

					m_BackBuffers[i] = backBuffer;

					rtvHandle.Offset(m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
				}
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::CreateCommandQueues()
			{
				m_pDirectCommandQueue = std::make_shared<CommandQueue>(D3D12_COMMAND_LIST_TYPE_DIRECT);

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::CreateViews()
			{
				CreateRTV();
				CreateSRV();

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::BeforeInitialize(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList)
			{
				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12BaseSystem::AfterInitialize(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList)
			{
				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::AfterResize(const glm::ivec2& a_vSize)
			{}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::Resize(const glm::ivec2& a_vPos, const glm::ivec2& a_vSize)
			{
				if (a_vSize.x == 0 || a_vSize.y == 0)
				{
					return;
				}

				Flush();
				for (int i = 0; i < g_iBufferCount; ++i)
				{
					m_BackBuffers[i].Reset();  // This will release the old resource properly
				}

#ifndef IMGUI_DISABLE
				m_ImGuiWindow.OnRenderTargetCleaned();
#endif // IMGUI_DISABLE

				DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
				if (FAILED(m_pSwapChain->GetDesc(&swapChainDesc)))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed getting desc from swap chain.");
					return;
				}
				if (FAILED(m_pSwapChain->ResizeBuffers(g_iBufferCount, a_vSize.x,
					a_vSize.y, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags)))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed resizing buffers.");
					return;
				}

				AfterResize(a_vSize);

				m_iCurrentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

				UpdateRenderTargetViews();

				m_eOnResize(a_vPos, a_vSize);
#ifndef IMGUI_DISABLE
				m_ImGuiWindow.Resize(a_vPos, a_vSize);
#endif // IMGUI_DISABLE

				m_vSize = glm::vec2(a_vSize.x, a_vSize.y);
			}

			//-----------------------------------------------------------------------------------------------------
			const Microsoft::WRL::ComPtr<ID3D12Resource>& DX12BaseSystem::GetCurrentBackBuffer() const
			{
				return m_BackBuffers[m_iCurrentBackBufferIndex];
			}

			//-----------------------------------------------------------------------------------------------------
			UINT DX12BaseSystem::GetCurrentBackBufferIndex() const
			{
				return m_iCurrentBackBufferIndex;
			}

			//-----------------------------------------------------------------------------------------------------
			D3D12_CPU_DESCRIPTOR_HANDLE DX12BaseSystem::GetCurrentRenderTargetView()
			{
				size_t backBufferStart = 0;
				return m_RTV.GetCPUHandle(backBufferStart + m_iCurrentBackBufferIndex);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::Loop()
			{
				ProcessWindowEvents();

				std::shared_ptr<CommandQueue> commandQueue = GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
				std::shared_ptr<CommandList> commandList = commandQueue->GetCommandList();

				D3D12_CPU_DESCRIPTOR_HANDLE rtv = GetCurrentRenderTargetView();

				Render3D(commandQueue, commandList, rtv);
#ifndef IMGUI_DISABLE
				RenderUI(commandQueue, commandList, rtv);
#endif // IMGUI_DISABLE

				Present(commandQueue, commandList);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::ProcessWindowEvents()
			{
				std::lock_guard<std::mutex> lock(m_RenderMutex);

				{
					std::lock_guard<std::mutex> lock(m_pWindow->g_EventMutex);

					while (!m_pWindow->GetEventQueue().empty())
					{
						const win32::WindowsMsg& event = m_pWindow->GetEventQueue().front();
						m_pWindow->GetEventQueue().pop();

						if (event.msg == WM_EXITSIZEMOVE || event.msg == WM_SIZE)
						{
							Resize(m_pWindow->GetPosition(), m_pWindow->GetRealSize());
						}
#ifndef IMGUI_DISABLE
						m_ImGuiWindow.WndProcHandler(event.hwnd, event.msg, event.wParam, event.lParam);
#endif // IMGUI_DISABLE
					}
				}
			}

			//-----------------------------------------------------------------------------------------------------
#ifndef IMGUI_DISABLE
			void DX12BaseSystem::RenderUI(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE a_RTVHandle)
			{
				FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				a_pCommandList->GetCommandList()->ClearRenderTargetView(a_RTVHandle, clearColor, 0, nullptr);

				m_ImGuiWindow.Render(a_pCommandList);
			}
#endif // IMGUI_DISABLE

			void DX12BaseSystem::Render3D(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE a_RTVHandle)
			{
				UINT currentBackBufferIndex = GetCurrentBackBufferIndex();
				const Microsoft::WRL::ComPtr<ID3D12Resource>& backBuffer = GetCurrentBackBuffer();

				a_pCommandList->TransitionResource(backBuffer,
					D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

				FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				a_pCommandList->GetCommandList()->ClearRenderTargetView(a_RTVHandle, clearColor, 0, nullptr);
				a_pCommandList->GetCommandList()->OMSetRenderTargets(1, &a_RTVHandle, FALSE, nullptr);

				ID3D12DescriptorHeap* descriptorHeaps[] = { m_SRV.GetHeap().Get() };
				a_pCommandList->GetCommandList()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

				m_eOnRender(a_pCommandList);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12BaseSystem::Present(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList)
			{
				UINT currentBackBufferIndex = GetCurrentBackBufferIndex();
				const Microsoft::WRL::ComPtr<ID3D12Resource>& backBuffer = GetCurrentBackBuffer();

				// Present
				{
					a_pCommandList->TransitionResource(backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

					m_aFenceValues[currentBackBufferIndex] = a_pCommandQueue->ExecuteCommandList(a_pCommandList);

					UINT syncInterval = g_bVSync ? 1 : 0;
					UINT presentFlags = m_bIsTearingSupported && !g_bVSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
					if (FAILED(m_pSwapChain->Present(syncInterval, presentFlags)))
					{
						LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed presenting.");
						return;
					}
					m_iCurrentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

					a_pCommandQueue->WaitForFenceValue(m_aFenceValues[m_iCurrentBackBufferIndex]);
				}
			}

			//-----------------------------------------------------------------------------------------------------
			std::shared_ptr<CommandQueue> DX12BaseSystem::GetCommandQueue(D3D12_COMMAND_LIST_TYPE a_Type)
			{
				std::shared_ptr<CommandQueue> commandQueue = nullptr;
				switch (a_Type)
				{
					case D3D12_COMMAND_LIST_TYPE_DIRECT:
					{
						commandQueue = m_pDirectCommandQueue;
						break;
					}
					default:
					{
						break;
					}
				}

				return commandQueue;
			}
		}
	}
}