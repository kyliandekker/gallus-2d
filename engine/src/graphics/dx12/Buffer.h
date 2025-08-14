#pragma once

#include "graphics/dx12/DX12PCH.h"
#include "graphics/dx12/DX12Resource.h"

#include <string>

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			//---------------------------------------------------------------------
			// Buffer
			//---------------------------------------------------------------------
			class Buffer : public DX12Resource
			{
			public:
				Buffer(const std::string& a_sName);
				Buffer(const D3D12_RESOURCE_DESC& a_ResDesc,
					size_t a_iNumElements, size_t a_iElementSize,
					const std::string& a_sName);
				virtual void CreateViews(size_t a_iNumElements, size_t a_iElementSize);
			};
		}
	}
}