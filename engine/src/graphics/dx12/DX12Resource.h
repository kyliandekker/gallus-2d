#pragma once

#include "core/EngineResource.h"

#include "DX12PCH.h"

#include <string>
#include <filesystem>

namespace tool
{
	namespace graphics
	{
		namespace dx12
		{
			//-----------------------------------------------------------------------------
			// DX12Resource
			//-----------------------------------------------------------------------------
			/// <summary>
			/// Wrapper for a DX12 resource, providing utility functions.
			/// </summary>
			class DX12Resource : public core::EngineResource
			{
			public:
				/// <summary>
				/// Constructs a dx12 resource without any details.
				/// </summary>
				DX12Resource() = default;

				/// <summary>
				/// Constructs a dx12 resource with a given name.
				/// </summary>
				/// <param name="a_ResourceDesc">Resource description.</param>
				/// <param name="a_sName">Name of the resource.</param>
				DX12Resource(const D3D12_RESOURCE_DESC& a_ResourceDesc, const std::wstring& a_sName);

				/// <summary>
				/// Constructs a dx12 resource with a given name.
				/// </summary>
				/// <param name="a_sName">Name of the resource.</param>
				DX12Resource(const std::wstring& a_sName);
				virtual ~DX12Resource();

				/// <summary>
				/// Constructs a dx12 resource with a given name.
				/// </summary>
				/// <param name="a_ResourceDesc">Resource description.</param>
				/// <param name="a_sName">Name of the resource.</param>
				/// <param name="a_Heap">Heap property options.</param>
				/// <param name="a_ResourceState">Resource state it will transition into.</param>
				/// <param name="a_pOptimizedClearValue">Clear value of the resource.</param>
				bool CreateResource(const D3D12_RESOURCE_DESC& a_ResourceDesc, const std::wstring& a_sName, const D3D12_HEAP_PROPERTIES& a_Heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), const D3D12_RESOURCE_STATES a_ResourceState = D3D12_RESOURCE_STATE_COMMON, const D3D12_CLEAR_VALUE* a_pOptimizedClearValue = nullptr);

				/// <summary>
				/// Returns whether the resource is a valid resource.
				/// </summary>
				/// <returns>True if the resource was valid, false otherwise.</returns>
				bool IsValid() const override;

				/// <summary>
				/// Returns the DX12 resource.
				/// </summary>
				/// <returns>Pointer to the DX12 Resource.</returns>
				Microsoft::WRL::ComPtr<ID3D12Resource>& GetResource();

				/// <summary>
				/// Returns the DX12 resource description.
				/// </summary>
				/// <returns>The resource DX12 description.</returns>
				D3D12_RESOURCE_DESC GetResourceDesc() const;

				/// <summary>
				/// Checks the resource if a format is supported.
				/// </summary>
				/// <param name="a_FormatSupport">Format to check.</param>
				/// <returns>True if the resource supports format, false otherwise.</returns>
				bool CheckFormatSupport(D3D12_FORMAT_SUPPORT1 a_FormatSupport) const;

				/// <summary>
				/// Checks the resource if a format is supported.
				/// </summary>
				/// <param name="a_FormatSupport">Format to check.</param>
				/// <returns>True if the resource supports format, false otherwise.</returns>
				bool CheckFormatSupport(D3D12_FORMAT_SUPPORT2 a_FormatSupport) const;

				/// <summary>
				/// Sets a DX12 resource.
				/// </summary>
				/// <param name="a_pResource">Resource that will be wrapped.</param>
				void SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> a_pResource);

				/// <summary>
				/// Sets the format support.
				/// </summary>
				void CheckFeatureSupport();
			protected:
				D3D12_FEATURE_DATA_FORMAT_SUPPORT m_FormatSupport{};
				Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource = nullptr;

				friend class ResourceAtlas;
			};
		}
	}
}