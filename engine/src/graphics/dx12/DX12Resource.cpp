#include "DX12Resource.h"

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
			// DX12Resource
			//-----------------------------------------------------------------------------
			DX12Resource::DX12Resource(const D3D12_RESOURCE_DESC& a_ResourceDesc, const std::wstring& a_sName)
			{
				CreateResource(a_ResourceDesc, a_sName);
			}

			//-----------------------------------------------------------------------------------------------------
			DX12Resource::DX12Resource(const std::wstring& a_sName) : EngineResource(a_sName)
			{}

			//-----------------------------------------------------------------------------------------------------
			DX12Resource::~DX12Resource()
			{}

			//-----------------------------------------------------------------------------------------------------
			bool DX12Resource::CreateResource(const D3D12_RESOURCE_DESC& a_ResourceDesc, const std::wstring& a_sName, const D3D12_HEAP_PROPERTIES& a_Heap, const D3D12_RESOURCE_STATES a_ResourceState, const D3D12_CLEAR_VALUE* a_pOptimizedClearValue)
			{
				if (m_pResource)
				{
					// Resources that cannot be destroyed cannot be overridden.
					if (!m_bIsDestroyable)
					{
						return false;
					}
					m_pResource.Reset();
				}

				Microsoft::WRL::ComPtr<ID3D12Device2>& device = core::TOOL->GetDX12().GetDevice();

				m_sName = a_sName;
				if (device->CreateCommittedResource(
					&a_Heap,
					D3D12_HEAP_FLAG_NONE,
					&a_ResourceDesc,
					a_ResourceState,
					a_pOptimizedClearValue,
					IID_PPV_ARGS(&m_pResource)
					))
				{
					LOGF(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed creating committed resource: \"%s\".", a_sName.c_str());
					return false;
				}
				m_pResource->SetName(m_sName.c_str());

				CheckFeatureSupport();

				return true;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12Resource::IsValid() const
			{
				return (m_pResource != nullptr);
			}

			//-----------------------------------------------------------------------------------------------------
			Microsoft::WRL::ComPtr<ID3D12Resource>& DX12Resource::GetResource()
			{
				return m_pResource;
			}

			//-----------------------------------------------------------------------------------------------------
			D3D12_RESOURCE_DESC DX12Resource::GetResourceDesc() const
			{
				D3D12_RESOURCE_DESC resDesc = {};
				if (m_pResource)
				{
					resDesc = m_pResource->GetDesc();
				}

				return resDesc;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12Resource::CheckFormatSupport(D3D12_FORMAT_SUPPORT1 a_FormatSupport) const
			{
				return (m_FormatSupport.Support1 & a_FormatSupport) != 0;
			}

			//-----------------------------------------------------------------------------------------------------
			bool DX12Resource::CheckFormatSupport(D3D12_FORMAT_SUPPORT2 a_FormatSupport) const
			{
				return (m_FormatSupport.Support2 & a_FormatSupport) != 0;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12Resource::SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> a_pResource)
			{
				// Resources that cannot be destroyed cannot be overridden.
				if (!m_bIsDestroyable)
				{
					return;
				}

				if (m_pResource)
				{
					m_pResource.Reset();
					m_pResource = nullptr;
				}

				m_pResource = a_pResource;
			}

			//-----------------------------------------------------------------------------------------------------
			void DX12Resource::CheckFeatureSupport()
			{
				Microsoft::WRL::ComPtr<ID3D12Device2>& device = core::TOOL->GetDX12().GetDevice();

				auto desc = m_pResource->GetDesc();
				m_FormatSupport.Format = desc.Format;
				if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &m_FormatSupport,
					sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT))))
				{
					LOGF(LOGSEVERITY_ERROR, LOG_CATEGORY_DX12, "Failed checking feature support.");
					return;
				}
			}
		}
	}
}