#include "DX12System3DGame.h"

#include "logger/Logger.h"
#include "graphics/dx12/CommandQueue.h"
#include "graphics/dx12/CommandList.h"

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			bool DX12System3DGame::CreateRootSignature()
			{
				// Define descriptor ranges
				CD3DX12_DESCRIPTOR_RANGE1 descriptorRanges[1]{};

				// SRV for the texture at register t0
				descriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

				// Define root parameters
				CD3DX12_ROOT_PARAMETER1 rootParameters[RootParameters::NumRootParameters]{};

				// CBV at register b0 (Model-View-Projection Matrix)
				rootParameters[RootParameters::CBV].InitAsConstants(sizeof(DirectX::XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

				// Texture SRV at register t0 (binds a texture)
				rootParameters[RootParameters::TEX_SRV].InitAsDescriptorTable(1, &descriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);

				rootParameters[RootParameters::MATERIAL].InitAsConstantBufferView(1);
				rootParameters[RootParameters::GLOBAL_SETTINGS].InitAsConstantBufferView(2);
				rootParameters[RootParameters::LIGHT].InitAsConstantBufferView(3);

				// Define static sampler at register s0 (replaces the removed descriptor table sampler)
				CD3DX12_STATIC_SAMPLER_DESC staticSamplers[] = {
					CD3DX12_STATIC_SAMPLER_DESC(
						0,  // Register s0
						D3D12_FILTER_MIN_MAG_MIP_LINEAR,  // Bilinear filtering
						D3D12_TEXTURE_ADDRESS_MODE_WRAP,
						D3D12_TEXTURE_ADDRESS_MODE_WRAP,
						D3D12_TEXTURE_ADDRESS_MODE_WRAP
					)
				};

				// Define root signature flags
				D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
					D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

				// Create root signature descriptor
				CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
				rootSignatureDescription.Init_1_1(
					_countof(rootParameters), rootParameters,
					_countof(staticSamplers), staticSamplers, // Use static samplers
					rootSignatureFlags
				);

				// Serialize the root signature
				Microsoft::WRL::ComPtr<ID3DBlob> rootSignatureBlob;
				Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
				HRESULT hr = D3DX12SerializeVersionedRootSignature(
					&rootSignatureDescription,
					D3D_ROOT_SIGNATURE_VERSION_1_1,
					&rootSignatureBlob,
					&errorBlob
				);

				if (FAILED(hr))
				{
					std::string errorMessage(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
					LOGF(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed serializing root signature: \"%s\".", errorMessage.c_str());
					return false;
				}

				// Create the root signature
				if (FAILED(m_pDevice->CreateRootSignature(0,
					rootSignatureBlob->GetBufferPointer(),
					rootSignatureBlob->GetBufferSize(),
					IID_PPV_ARGS(&m_pRootSignature))))
				{
					LOG(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating root signature.");
					return false;
				}

				return true;
			}

			void DX12System3DGame::Loop()
			{
				ProcessWindowEvents();

				std::shared_ptr<CommandQueue> commandQueue = GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
				std::shared_ptr<CommandList> commandList = commandQueue->GetCommandList();

				D3D12_CPU_DESCRIPTOR_HANDLE rtv = GetCurrentRenderTargetView();

				Render3D(commandQueue, commandList, rtv);

				rtv = GetCurrentRenderTargetView();
#ifndef IMGUI_DISABLE
				RenderUI(commandQueue, commandList, rtv);
#endif // IMGUI_DISABLE

				Present(commandQueue, commandList);
			}
		}
	}
}