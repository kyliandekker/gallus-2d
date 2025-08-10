#include "CommandList.h"

#include "core/Tool.h" 
#include "graphics/dx12/DX12BaseSystem.h"
#include "logger/Logger.h"

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			//-----------------------------------------------------------------------------
			// CommandList
			//-----------------------------------------------------------------------------
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>& CommandList::GetCommandList()
			{
				return m_pCommandList;
			}

			//-----------------------------------------------------------------------------------------------------
			bool CommandList::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& a_pAllocator, D3D12_COMMAND_LIST_TYPE a_CommandListType)
			{
				Microsoft::WRL::ComPtr<ID3D12Device2>& device = core::TOOL->GetDX12().GetDevice();
				if (FAILED(device->CreateCommandList(0, a_CommandListType, a_pAllocator.Get(), nullptr, IID_PPV_ARGS(&m_pCommandList))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating command list.");
					return false;
				}

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			void CommandList::UpdateBufferResource(
				ID3D12Resource** a_pDestinationResource,
				ID3D12Resource** a_pIntermediateResource,
				size_t a_iNumElements, size_t a_iElementSize, const void* a_pBufferData,
				D3D12_RESOURCE_FLAGS a_Flags)
			{
				size_t bufferSize = a_iNumElements * a_iElementSize;

				CD3DX12_HEAP_PROPERTIES defaultHeaptype = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				CD3DX12_RESOURCE_DESC buffer = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, a_Flags);

				// Create a committed resource for the GPU resource in a default heap.
				Microsoft::WRL::ComPtr<ID3D12Device2>& device = core::TOOL->GetDX12().GetDevice();
				if (FAILED(device->CreateCommittedResource(
					&defaultHeaptype,
					D3D12_HEAP_FLAG_NONE,
					&buffer,
					D3D12_RESOURCE_STATE_COMMON,
					nullptr,
					IID_PPV_ARGS(a_pDestinationResource))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating committed resource.");
					return;
				}

				// Create an committed resource for the upload.
				if (a_pBufferData)
				{
					CD3DX12_HEAP_PROPERTIES uploadHeaptype = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
					CD3DX12_RESOURCE_DESC buff = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
					Microsoft::WRL::ComPtr<ID3D12Device2>& device = core::TOOL->GetDX12().GetDevice();
					if (FAILED(device->CreateCommittedResource(
						&uploadHeaptype,
						D3D12_HEAP_FLAG_NONE,
						&buff,
						D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr,
						IID_PPV_ARGS(a_pIntermediateResource))))
					{
						LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating committed resource.");
						return;
					}

					D3D12_SUBRESOURCE_DATA subresourceData = {};
					subresourceData.pData = a_pBufferData;
					subresourceData.RowPitch = bufferSize;
					subresourceData.SlicePitch = subresourceData.RowPitch;

					UpdateSubresources(m_pCommandList.Get(),
						*a_pDestinationResource, *a_pIntermediateResource,
						0, 0, 1, &subresourceData);
				}
			}

			//-----------------------------------------------------------------------------------------------------
			void CommandList::TransitionResource(Microsoft::WRL::ComPtr<ID3D12Resource> a_pResource, D3D12_RESOURCE_STATES a_BeforeState, D3D12_RESOURCE_STATES a_AfterState)
			{
				CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					a_pResource.Get(),
					a_BeforeState, a_AfterState);

				m_pCommandList->ResourceBarrier(1, &barrier);
			}
		}
	}
}