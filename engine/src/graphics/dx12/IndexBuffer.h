#pragma once

#include "graphics/dx12/Buffer.h"

namespace gallus
{
	namespace graphics
	{
		namespace dx12
		{
			class IndexBuffer : public Buffer
			{
			public:
				IndexBuffer(const std::string& string = "");
				virtual ~IndexBuffer();

				void CreateViews(size_t a_iNumElements, size_t a_iElementSize) override;
				D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView();
				size_t GetNumIndicies() const;
				DXGI_FORMAT GetIndexFormat() const;
			private:
				size_t m_iNumIndicies = 0;
				DXGI_FORMAT m_IndexFormat;

				D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
			};
		}
	}
}