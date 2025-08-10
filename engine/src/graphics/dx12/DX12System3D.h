#pragma once

#include "DX12BaseSystem.h"

#include "DX12Resource.h"

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			//-----------------------------------------------------------------------------
			// DX12System3D
			//-----------------------------------------------------------------------------
			/// <summary>
			/// Represents a DirectX 12 rendering window, managing device resources, rendering, and synchronization. 3D rendering, but has pure virtual functions.
			/// </summary>
			class DX12System3D : public DX12BaseSystem
			{
			protected:
				bool CreateCommandQueues() override;
				bool CreateViews() override;
				bool BeforeInitialize(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList) override;
				bool AfterInitialize(std::shared_ptr<CommandQueue> a_pCommandQueue, std::shared_ptr<CommandList> a_pCommandList) override;

				void AfterResize(const glm::ivec2& a_vSize) override;

				void CreateDSV();
				void ResizeDepthBuffer(const glm::ivec2& a_vSize);

				/// <summary>
				/// Ensures all previously submitted GPU commands are completed before continuing execution.
				/// </summary>
				/// <remarks>
				/// This method blocks the CPU until the GPU has finished processing all commands up to this point. 
				/// It is useful for synchronization, resource cleanup, or preparing for device destruction.
				/// </remarks>
				void Flush() override;

				virtual bool CreateRootSignature() = 0;

				std::shared_ptr<CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE a_Type = D3D12_COMMAND_LIST_TYPE_DIRECT) override;

				std::shared_ptr<CommandQueue> m_pComputeCommandQueue = nullptr;
				std::shared_ptr<CommandQueue> m_pCopyCommandQueue = nullptr;

				HeapAllocation m_DSV;
				DX12Resource m_DepthBuffer;

				Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;

				D3D12_VIEWPORT m_Viewport;
				D3D12_RECT m_ScissorRect;
			};
		}
	}
}