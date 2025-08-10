#include "DX12System3D.h"

#include "CommandQueue.h"
#include "CommandList.h"
#include "logger/Logger.h"

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			//-----------------------------------------------------------------------------------------------------
			bool DX12System3D::CreateCommandQueues()
			{
				DX12BaseSystem::CreateCommandQueues();

				m_pComputeCommandQueue = std::make_shared<CommandQueue>(D3D12_COMMAND_LIST_TYPE_COMPUTE);
				m_pCopyCommandQueue = std::make_shared<CommandQueue>(D3D12_COMMAND_LIST_TYPE_COPY);

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12System3D::CreateViews()
			{
				DX12BaseSystem::CreateViews();
				CreateDSV();

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12System3D::BeforeInitialize(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList)
			{
				if (!CreateRootSignature())
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating root signature.");
					return false;
				}
				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12System3D::AfterInitialize(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList)
			{
				AfterResize(m_vSize);

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12System3D::AfterResize(const glm::ivec2& a_vSize)
			{
				ResizeDepthBuffer(a_vSize);

				m_Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(a_vSize.x), static_cast<float>(a_vSize.y));
				m_ScissorRect = CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12System3D::CreateDSV()
			{
				// Create the descriptor heap for the depth-stencil view.
				D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
				dsvHeapDesc.NumDescriptors = 1;
				dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				m_DSV = HeapAllocation(dsvHeapDesc);
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12System3D::ResizeDepthBuffer(const glm::ivec2& a_vSize)
			{
				// Flush any GPU commands that might be referencing the depth buffer.
				Flush();
				m_vSize = a_vSize;

				m_DSV.Deallocate(0);

				// Resize screen dependent resources.
				// Create a depth buffer.
				D3D12_CLEAR_VALUE optimizedClearValue = {};
				optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
				optimizedClearValue.DepthStencil = { 1.0f, 0 };

				CD3DX12_HEAP_PROPERTIES heapType = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				CD3DX12_RESOURCE_DESC tex = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_vSize.x, m_vSize.y,
					1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

				if (!m_DepthBuffer.CreateResource(tex, L"Depth Buffer", CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_RESOURCE_STATE_DEPTH_WRITE, &optimizedClearValue))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating committed resource.");
					return;
				}

				// Update the depth-stencil view.
				D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
				dsv.Format = DXGI_FORMAT_D32_FLOAT;
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsv.Texture2D.MipSlice = 0;
				dsv.Flags = D3D12_DSV_FLAG_NONE;

				m_pDevice->CreateDepthStencilView(m_DepthBuffer.GetResource().Get(), &dsv,
					m_DSV.GetCPUHandle(m_DSV.Allocate()));
			}

			std::shared_ptr<CommandQueue> dx12::DX12System3D::GetCommandQueue(D3D12_COMMAND_LIST_TYPE a_Type)
			{
				std::shared_ptr<CommandQueue> commandQueue = nullptr;
				switch (a_Type)
				{
					case D3D12_COMMAND_LIST_TYPE_DIRECT:
					{
						commandQueue = m_pDirectCommandQueue;
						break;
					}
					case D3D12_COMMAND_LIST_TYPE_COMPUTE:
					{
						commandQueue = m_pComputeCommandQueue;
						break;
					}
					case D3D12_COMMAND_LIST_TYPE_COPY:
					{
						commandQueue = m_pCopyCommandQueue;
						break;
					}
					default:
					{
						break;
					}
				}

				return commandQueue;
			}

			void DX12System3D::Flush()
			{
				DX12BaseSystem::Flush();
				m_pComputeCommandQueue->Flush();
				m_pCopyCommandQueue->Flush();
			}
		}
	}
}